#include "ACD3D10VertexCache.h"
#include "ACD3D10VertexManager.h"
#include "ACD3D10.h"

ACD3D10VertexCache::ACD3D10VertexCache(ACD3D10VertexManager* vManager,
									   ID3D10Device* gDevice,
									   UINT vertexMax, 
									   UINT indicesMax, 
									   UINT nStride,  
									   VertexFormat vFormat, 
									   FILE* log)
{
	HRESULT hr;

	IsEmpty				= true;
	mpSkin              = nullptr;
	NumVertices			= 0;
	NumIndices			= 0;

	mpVManager			= vManager;
	mpGDevice			= gDevice;
	mpLOG				= log;

	mNumVerticesMax		= vertexMax;
	mNumIndicesMax		= indicesMax;
	mStride				= nStride;

	//cria o vb
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DYNAMIC;
	vbd.ByteWidth =  mStride * vertexMax; 
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vbInitData;
	vbInitData.pSysMem = nullptr;
	
	hr = gDevice->CreateBuffer( &vbd, nullptr, &mpVB );
	if (FAILED(hr)) mpVB = nullptr;

	//cria o indexbuffer
	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof ( UINT) * indicesMax; 
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA ibInitData;
	ibInitData.pSysMem = nullptr;

	hr = gDevice->CreateBuffer(&ibd, nullptr, &mpIB);
	if (FAILED(hr)) mpIB = nullptr;
};

ACD3D10VertexCache::~ACD3D10VertexCache()
{
	SAFE_RELEASE(mpVB);
	SAFE_RELEASE(mpIB);
};

BOOL ACD3D10VertexCache::UseSkin(ACSkin* skin)
{
	return mpSkin == skin;
};

HRESULT ACD3D10VertexCache::Add(UINT numVertices, UINT numIndices, void* vertices, UINT* indices, ACSkin* skin)
{
	HRESULT hr;

	BYTE* tmpVertices = nullptr;        // pointer to VB memory
	UINT* tmpIndices = nullptr;         // pointer to IB memory
   
	INT   sizeV = mStride * numVertices;
	INT   sizeI = sizeof( UINT ) * numIndices;

	INT   posV;
	INT   posI;
	D3D10_MAP flag;

	//se passar mais vertices doq o buffer suporta fudeu
	if (numVertices > mNumVerticesMax || numIndices > mNumIndicesMax)
		return AC_BUFFERSIZE;

	//se nao conseguir adicionar mais vertices no buffer entao ele renderiza usando o fluh para limpar o buffer
	UINT totalVertices = numVertices + NumVertices;
	UINT totalIndices = numIndices + NumIndices;
	if (totalVertices > mNumVerticesMax || totalIndices  > mNumIndicesMax)
	{
		if (FAILED(Flush()))
		{
			MessageBoxA(nullptr, "[ERROR] Erro no metodo flush do VertexCache", "Error", MB_OK | MB_ICONERROR);
			Log("[ERROR] Erro no metodo flush do VertexCache");
			return AC_FAIL;
		}
	}

	//se nao for o mesmo skin tb ele da o flush
	if (skin != mpSkin)
	{
		if (FAILED(Flush()))
		{
			MessageBoxA(nullptr, "[ERROR] Erro no metodo flush do VertexCache", "Error", MB_OK | MB_ICONERROR);
			Log("[ERROR] Erro no metodo flush do VertexCache");
			return AC_FAIL;
		}
	}

	//seta o skin atual
	mpSkin = skin;

	// se nao tem nada no buffer entao ele descarta tudo
	if (NumVertices == 0) 
	{
		posV = posI = 0;
		flag = D3D10_MAP_WRITE_DISCARD;
	}
	else  //senao ele apenas concatena sem sobrepor oq ja tem
	{
		posV = mStride * NumVertices;
		posI = sizeof( UINT ) * NumIndices;
		flag = D3D10_MAP_WRITE_NO_OVERWRITE;
	}

	//da um lock no vb e ib, se der erro no lock do IB entao ele ja fecha o VB tb
	if (FAILED(mpVB->Map(flag, 0, (void**)&tmpVertices)))
		return AC_BUFFERLOCK;
	if (FAILED(mpIB->Map(flag, 0, (void**)&tmpIndices))) 
	{
		mpVB->Unmap();
		return AC_BUFFERLOCK;
	}

	// copias os vertices para dentro do buffer partindo da posicao atual
	memcpy(tmpVertices + posV, vertices, sizeV);

	// copia os indices para o buffer, ele copia para as posicoes certas, pq pode ter partes sem indices e com saca
	UINT base = NumVertices;
	if (!indices) numIndices = numVertices;
	for (UINT i = 0; i < numIndices; i++) 
	{
		if (indices != nullptr)
			tmpIndices[NumIndices] = indices[i] + base;
		else
			tmpIndices[NumIndices] = i + base;

		NumIndices++;
	}

	// add to count
	NumVertices += numVertices;

	// unlock buffers
	mpVB->Unmap();
	mpIB->Unmap();

	IsEmpty = false;

	return AC_OK;
};

HRESULT ACD3D10VertexCache::Flush()
{
	//se nao tem vertices no cache entao nao renderiza nada
	if  (NumVertices <= 0) 
		return AC_OK;

	//pega a implementacao da interface do render para comecar a setar os valores
	//se existir um skin entao ele aplica, pode nao haver no caso de renderizar pontos ou linhas
	ACD3D10* acd3d10 = mpVManager->GetACD3D10();

	ACSHADEMODE shadeMode = acd3d10->GetShadeMode();

	//se for wireframe entao ele passa a cor do wire para o shader
	if (mpVManager->IsWire())
	{
		//manda todo o skin para a gpu para renderizar
		Vector4 wc(&mpVManager->GetWireColor(), 1);
		acd3d10->SetMaterialAmbient(mpVManager->GetWireColor());
		acd3d10->SetMaterialDiffuse(wc);
		acd3d10->SetMaterialSpecular(mpVManager->GetWireColor());
		acd3d10->SetMaterialSpecularPower(1);
		acd3d10->SetMaterialEmissive(mpVManager->GetWireColor());
		for (int i = 0; i < NUM_TEXTURES; i++)
			acd3d10->SetTexture(nullptr, i);

		acd3d10->ApplyConstants();
	}
	else
	{
		if (mpSkin != nullptr)
		{
			////manda todo o skin para a gpu para renderizar
			if (mpSkin->UseMaterial)
			{
				acd3d10->SetMaterialAmbient(mpSkin->Material.AmbientColor);
				acd3d10->SetMaterialDiffuse(mpSkin->Material.DiffuseColor);
				acd3d10->SetMaterialSpecular(mpSkin->Material.SpecularColor);
				acd3d10->SetMaterialSpecularPower(mpSkin->Material.SpecularPower);
				acd3d10->SetMaterialEmissive(mpSkin->Material.EmissiveColor);
			}

			if (shadeMode == ACSHADEMODE::ACSM_TriangleList ||
				shadeMode == ACSHADEMODE::ACSM_TriangleStrip ||
				shadeMode == ACSHADEMODE::ACSM_PointSprite) //sprites usam points no directx 10+
			{
				//se for pointsprite ele usa apenas a primeira textura
				if (acd3d10->GetShadeMode() == ACSHADEMODE::ACSM_PointSprite)
				{
					if (mpSkin->Textures[0] != nullptr)
						acd3d10->SetTexture(mpSkin->Textures[0], 0);
				}
				else
				{
					for (int i = 0; i < NUM_TEXTURES; i++)
					{
						if (mpSkin->Textures[i] != nullptr)
							acd3d10->SetTexture(mpSkin->Textures[i], i);
					}
				}
			}
			else
			{
				for (int i = 0; i < NUM_TEXTURES; i++)
				{
					if (mpSkin->Textures[i] != nullptr)
						acd3d10->SetTexture(nullptr, i);
				}
			}

			acd3d10->ApplyConstants();
		}
		else
		{
			//quando nao tem skin entao ele passa a cor do material diffuso 1
			acd3d10->SetMaterialDiffuse(Vector4(1));
			//zera as texturas
			for (int i = 0; i < NUM_TEXTURES; i++)
				acd3d10->SetTexture(nullptr, i);
			acd3d10->ApplyConstants();
		}
	}

	//renderiza
	UINT offset = 0;
	mpGDevice->IASetVertexBuffers(0, 1, &mpVB, &mStride, &offset);

	switch (acd3d10->GetShadeMode())
	{
		case ACSHADEMODE::ACSM_TriangleList: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); break;
		case ACSHADEMODE::ACSM_TriangleStrip: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ); break;
		case ACSHADEMODE::ACSM_LineList: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST ); break;
		case ACSHADEMODE::ACSM_LineStrip: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP ); break;
		case ACSHADEMODE::ACSM_Point:
		case ACSHADEMODE::ACSM_PointSprite: 
						  mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST ); 
						  break;
	};

	//se existir indices e nao for pontos entao usa os indices
	if (NumIndices > 0 && 
		shadeMode != ACSHADEMODE::ACSM_Point &&
		shadeMode != ACSHADEMODE::ACSM_PointSprite)
	{
		mpGDevice->IASetIndexBuffer(mpIB, DXGI_FORMAT_R32_UINT, 0);
		mpGDevice->DrawIndexed(NumIndices, 0, 0);
	}
	else
		mpGDevice->Draw(NumVertices, 0);

	//seta os indices pra 0 pq quando ele der um add ele vai dar um discard nos vertices e indices
	NumIndices = 0;
	NumVertices = 0;
	IsEmpty = true;

	//invalido o buffer pq senao ele nao da o iasetvertexbuffer ou iasetindexbuffer  do statico
	mpVManager->InvalidateStaticBuffer();

	return AC_OK;
};

void ACD3D10VertexCache::Log(char* message, ...)
{
	if (mpLOG!=nullptr)
	{
	   char ch[256];
	   char *pArgs;

	   pArgs = (char*) &message + sizeof(message);
	   vsprintf(ch, message, pArgs);
	   fprintf(mpLOG, ch);
	   fprintf(mpLOG, "\n");
	   fflush(mpLOG);
	}
};