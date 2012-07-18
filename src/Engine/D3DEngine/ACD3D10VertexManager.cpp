#include "ACD3D10VertexManager.h"
#include "ACD3D10.h"

ACD3D10VertexManager::ACD3D10VertexManager(ACD3D10* pACD3D10, 
										   ID3D10Device* gDevice, 
										   UINT maxVertices, 
										   UINT maxIndices, 
										   FILE* log)
{
	mpACD3D10 = pACD3D10;
	mpLOG = log;
	mpCurrentVertexBuffer = nullptr;
	mpGDevice = gDevice;
	mIsWire = false;
	mChangeAnyCache = false;

	mMaxVertices = maxVertices;
	mMaxIndices = maxIndices;

	for (int i = 0; i < NUM_CACHES; i++)
	{
		//position 
		mpCacheP[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPosition ), VertexFormat::VF_VertexPosition, log);
		//position colored
		mpCachePC[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionColored ), VertexFormat::VF_VertexPositionColored, log);
		//position textured	
		mpCachePT[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionTextured ), VertexFormat::VF_VertexPositionTextured, log);
		//position normal textured
		mpCachePNT[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionNormalTextured ), VertexFormat::VF_VertexPositionNormalTextured, log);
		//position textured extra info
		mpCachePTE[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionTexturedExtraInfo ), VertexFormat::VF_VertexPositionTexturedExtraInfo, log);
		//skinedmesh
		mpCacheSM[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexSkinnedMesh ), VertexFormat::VF_VertexSkinnedMesh, log);
		//sprite
		mpCacheSP[i] = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexSprite ), VertexFormat::VF_VertexSprite, log);
	}

	//dynamicbuffer para as linhas
	mpCacheLines = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionColored ), VertexFormat::VF_VertexPositionColored, log);
	//dynamicbuffer para os pontos
	mpCachePoints = new ACD3D10VertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionColored ), VertexFormat::VF_VertexPositionColored, log);
};

ACD3D10VertexManager::~ACD3D10VertexManager()
{
	Release();
};

HRESULT ACD3D10VertexManager::CreateStaticBuffer(VertexFormat vertexFormat, 
												 UINT numVertices, 
												 UINT numIndices, 
												 const void* pVertices, 
												 const UINT* pIndices, 
												 ACSkin* pSkin,
												 ACVertexBuffer** ppOutVertexBufferPtr)
{
	HRESULT hr;

	(*ppOutVertexBufferPtr) = new ACVertexBuffer();
	(*ppOutVertexBufferPtr)->VFormat = vertexFormat;
	(*ppOutVertexBufferPtr)->NumVertices = numVertices;
	(*ppOutVertexBufferPtr)->NumIndices = numIndices;
	(*ppOutVertexBufferPtr)->Skin = pSkin;

	if (pIndices != nullptr)
		(*ppOutVertexBufferPtr)->NumPrimitives = INT(numIndices / 3);
	else
		(*ppOutVertexBufferPtr)->NumPrimitives = INT(numVertices / 3);

	//***** CRIA VB *********
	ID3D10Buffer* pVB = nullptr;

	//seleciona a estrutura correta de vertice
	UINT byteWidth = 0;
	switch (vertexFormat)
	{
		case VertexFormat::VF_VertexPosition: byteWidth = sizeof ( ACVertexPosition ); break;
		case VertexFormat::VF_VertexPositionColored: byteWidth = sizeof ( ACVertexPositionColored ); break;
		case VertexFormat::VF_VertexPositionNormalTextured: byteWidth = sizeof ( ACVertexPositionNormalTextured ); break;
		case VertexFormat::VF_VertexPositionTextured: byteWidth = sizeof ( ACVertexPositionTextured ); break;
		case VertexFormat::VF_VertexPositionTexturedExtraInfo: byteWidth = sizeof ( ACVertexPositionTexturedExtraInfo ); break;
		case VertexFormat::VF_VertexSkinnedMesh: byteWidth = sizeof ( ACVertexSkinnedMesh ); break;
		case VertexFormat::VF_VertexSprite: byteWidth = sizeof ( ACVertexSprite ); break;
	};

	//tamanho da estrutura
	(*ppOutVertexBufferPtr)->Stride = byteWidth;

	byteWidth *= numVertices;

	//cria a descricao do vertexbuffer
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = byteWidth; 
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vbInitData;
	vbInitData.pSysMem = pVertices;

	hr = ACD3D10Globals::G_pD3dDevice->CreateBuffer( &vbd, &vbInitData, &pVB );
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create vb error. CreateStaticBuffer()","Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create vb error. CreateStaticBuffer()");
		return AC_FAIL;
	}

	(*ppOutVertexBufferPtr)->pVB = pVB;
	//******* FIM VB ********


	//***** CRIA IB ****
	// se passar a lista de indices entao ele cria senao nao
	if (pIndices!=nullptr)
	{
		ID3D10Buffer* pIB = nullptr;
		byteWidth = sizeof( UINT ) * numIndices;

		D3D10_BUFFER_DESC ibd;
		ibd.Usage = D3D10_USAGE_DEFAULT;
		ibd.ByteWidth = byteWidth; 
		ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA ibInitData;
		ibInitData.pSysMem = pIndices;

		hr = ACD3D10Globals::G_pD3dDevice->CreateBuffer(&ibd, &ibInitData, &pIB);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, "[ERROR] Create ib error. CreateStaticBuffer()","Error", MB_OK | MB_ICONERROR);
			Log("[ERROR] Create ib error. CreateStaticBuffer()");
			return AC_FAIL;
		}

		(*ppOutVertexBufferPtr)->pIB = pIB;
	}
	//***** FIM IB **********

};

void ACD3D10VertexManager::InvalidateStaticBuffer()
{
	mpCurrentVertexBuffer = nullptr;
	mpCurrentSkin = nullptr;
};

void ACD3D10VertexManager::ReleaseBuffer(ACVertexBuffer* vertexBuffer)
{
	ID3D10Buffer* v = static_cast<ID3D10Buffer*>(vertexBuffer->pVB);

	if (vertexBuffer->pIB != nullptr)
	{
		ID3D10Buffer* i = static_cast<ID3D10Buffer*>(vertexBuffer->pIB);
		SAFE_RELEASE(i);
	}

	SAFE_RELEASE(v);
};

HRESULT ACD3D10VertexManager::Render(ACVertexBuffer* vertexBuffer)
{
	//se nao for o skin corrente entao ele tem q setar os dados
	if (vertexBuffer->Skin!=mpCurrentSkin)
	{
		mpCurrentSkin = vertexBuffer->Skin;

		ACSHADEMODE shadeMode = mpACD3D10->GetShadeMode();

		//se for modo wireframe entao ele usa a cor pra setar o material e ja tira todas as texturas pra nao dar zebra
		if (mIsWire)
		{
			////manda todo o skin para a gpu para renderizar
			Vector4 wc(&mWireColor, 1);
			mpACD3D10->SetMaterialAmbient(mWireColor);
			mpACD3D10->SetMaterialDiffuse(wc);
			mpACD3D10->SetMaterialSpecular(mWireColor);
			mpACD3D10->SetMaterialSpecularPower(1);
			mpACD3D10->SetMaterialEmissive(mWireColor);
			for (int i = 0; i < NUM_TEXTURES; i++)
				mpACD3D10->SetTexture(nullptr, i);

			mpACD3D10->ApplyConstants();
		}
		else
		{
			if (mpCurrentSkin != nullptr)
			{
				////manda todo o skin para a gpu para renderizar
				if (mpCurrentSkin->UseMaterial)
				{
					mpACD3D10->SetMaterialAmbient(mpCurrentSkin->Material.AmbientColor);
					mpACD3D10->SetMaterialDiffuse(mpCurrentSkin->Material.DiffuseColor);
					mpACD3D10->SetMaterialSpecular(mpCurrentSkin->Material.SpecularColor);
					mpACD3D10->SetMaterialSpecularPower(mpCurrentSkin->Material.SpecularPower);
					mpACD3D10->SetMaterialEmissive(mpCurrentSkin->Material.EmissiveColor);
				}

				//se for triangulo entao ele adiciona as texturas senao ele tira todas
				if (shadeMode == ACSHADEMODE::ACSM_TriangleList ||
					shadeMode == ACSHADEMODE::ACSM_TriangleStrip ||
					shadeMode == ACSHADEMODE::ACSM_PointSprite) //sprites usam points no directx 10+
				{
					//se for pointsprite ele usa so a primeira textura
					if (shadeMode == ACSHADEMODE::ACSM_PointSprite)
						mpACD3D10->SetTexture(mpCurrentSkin->Textures[0], 0);
					else
					{
						for (int i = 0; i < NUM_TEXTURES; i++)
							mpACD3D10->SetTexture(mpCurrentSkin->Textures[i], i);
					}
				}
				else
				{
					for (int i = 0; i < NUM_TEXTURES; i++)
						mpACD3D10->SetTexture(nullptr, i);
				}

				mpACD3D10->ApplyConstants();
			}
		}
	}

	//se nao for o vb corrente ele seta
	if (vertexBuffer != mpCurrentVertexBuffer)
	{
		mpCurrentVertexBuffer = vertexBuffer;

		UINT offset = 0;
	
		ID3D10Buffer* v = static_cast<ID3D10Buffer*>(mpCurrentVertexBuffer->pVB);
		ACD3D10Globals::G_pD3dDevice->IASetVertexBuffers( 0, 1, &v, &mpCurrentVertexBuffer->Stride, &offset );

		switch (mpACD3D10->GetShadeMode())
		{
			case ACSHADEMODE::ACSM_TriangleList: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); break;
			case ACSHADEMODE::ACSM_TriangleStrip: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ); break;
			case ACSHADEMODE::ACSM_LineList: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST ); break;
			case ACSHADEMODE::ACSM_LineStrip: mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP ); break;
			case ACSHADEMODE::ACSM_Point: 
			case ACSHADEMODE::ACSM_PointSprite:	mpGDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST ); break;
		};

		//se existe IB entao ele seta
		if (mpCurrentVertexBuffer->pIB != nullptr)
		{
			ID3D10Buffer* i = static_cast<ID3D10Buffer*>(mpCurrentVertexBuffer->pIB);
			ACD3D10Globals::G_pD3dDevice->IASetIndexBuffer(i, DXGI_FORMAT_R32_UINT, 0);

			ACD3D10Globals::G_pD3dDevice->DrawIndexed( mpCurrentVertexBuffer->NumIndices, 0, 0);
		}
		else
			ACD3D10Globals::G_pD3dDevice->Draw( mpCurrentVertexBuffer->NumVertices, 0);
	}
	else
	{
		//se existe IB entao ele seta
		if (mpCurrentVertexBuffer->pIB != nullptr)
		{
			ACD3D10Globals::G_pD3dDevice->DrawIndexed( mpCurrentVertexBuffer->NumIndices, 0, 0);
		}
		else
			ACD3D10Globals::G_pD3dDevice->Draw( mpCurrentVertexBuffer->NumVertices, 0);
	}

	return AC_OK;
};


HRESULT ACD3D10VertexManager::RenderLines(UINT numVertices, 
										  UINT numIndices, 
										  ACVertexPositionColored* vertices, 
										  UINT* indices,
										  BOOL strip)
{
	//invalida o staticbuffer e tb o skin corrente
	InvalidateStaticBuffer();

	// se precisar ele ja executa o flush antes de trocar o estado
	mpACD3D10->SetBlendState(ACBLENDSTATE::ACBS_Opaque);

	//pega o atual para setar no final
	ACSHADEMODE currentSM = mpACD3D10->GetShadeMode();
	
	if (strip)
		mpACD3D10->SetShadeMode(ACSHADEMODE::ACSM_LineStrip);
	else
		mpACD3D10->SetShadeMode(ACSHADEMODE::ACSM_LineList);

	mpCacheLines->Add(numVertices, numIndices, vertices, indices, nullptr);
	mpCacheLines->Flush();

	mpACD3D10->SetShadeMode(currentSM);

	return AC_OK;
};


HRESULT ACD3D10VertexManager::RenderPoints(UINT numVertices, 
										   ACVertexPositionColored* vertices)
{
	//invalida o staticbuffer e tb o skin corrente
	InvalidateStaticBuffer();

	// se precisar ele ja executa o flush antes de trocar o estado
	mpACD3D10->SetBlendState(ACBLENDSTATE::ACBS_Opaque);

	//pega o atual para setar no final
	ACSHADEMODE currentSM = mpACD3D10->GetShadeMode();
	
	mpACD3D10->SetShadeMode(ACSHADEMODE::ACSM_Point);

	mpCachePoints->Add(numVertices, 0, vertices, nullptr, nullptr);
	mpCachePoints->Flush();

	mpACD3D10->SetShadeMode(currentSM);

	return AC_OK;
};

HRESULT ACD3D10VertexManager::Render(VertexFormat vertexFormat, 
									 UINT numVertices, 
									 UINT numIndices, 
									 void* vertices, 
									 UINT* indices, 
									 ACSkin* skin)
{
	ACD3D10VertexCache **pCache=nullptr,
						*pCacheEmpty=nullptr,
						*pCacheFullest=nullptr;
	int emptyVC   = -1;
	int fullestVC = 0;

	// seleciono q tipo de vertice eu vou usar
	switch (vertexFormat)
	{
		case VertexFormat::VF_VertexPosition:					pCache = mpCacheP;		break;
		case VertexFormat::VF_VertexPositionColored:  			pCache = mpCachePC;		break;
		case VertexFormat::VF_VertexPositionTextured:			pCache = mpCachePT;		break;
		case VertexFormat::VF_VertexPositionNormalTextured:		pCache = mpCachePNT;	break;
		case VertexFormat::VF_VertexPositionTexturedExtraInfo:	pCache = mpCachePTE;	break;
		case VertexFormat::VF_VertexSkinnedMesh:				pCache = mpCacheSM;		break;
		case VertexFormat::VF_VertexSprite:						pCache = mpCacheSP;		break;
		default: return AC_INVALIDFORMAT;
	};

	//se entrou aqui é pq ele vai modificar algum cache
	mChangeAnyCache = true;

	//inicializa o cache cheio
	pCacheFullest = pCache[0];

	//ja q agora eu selecionei o array q eu quero vou atras de um q use o mesmo skin q foi passado
	for (int i = 0; i < NUM_CACHES; i++)
	{
		//se ele achar um buffer o o mesmo skin ele manda os vertices, la dentro se ele ja esta cheio ele da um flush para poder armazenar os dados novos
		if (pCache[i]->UseSkin(skin))
			return pCache[i]->Add(numVertices, numIndices, vertices, indices, skin);
     
		//se nao conseguiu achar um buffer q use o skin entao ele vai atras de um q esteja limpo
		if (pCache[i]->IsEmpty)
			pCacheEmpty = pCache[i];

		//se nao achou um limpo entao ele se obriga a procurar o mais cheio com outro skin para esvaziar e colocar os dados novos
		if (pCache[i]->NumVertices > pCacheFullest->NumVertices)
			pCacheFullest = pCache[i];
	}

	//se ele achou um limpo entao ele usa o cara
	if (pCacheEmpty) 
		return pCacheEmpty->Add(numVertices, numIndices, vertices, indices, skin);

	//senao tinha nenhum limpo ai ja eras, nao tinha nenhum com o mesmo skin e nenhum limpo, usa a terceira opcao o mais cheio com outro skin
	pCacheFullest->Flush();
	return pCacheFullest->Add(numVertices, numIndices, vertices, indices, skin);
};

HRESULT ACD3D10VertexManager::ForcedFlushAll()
{
	//se entrou aqui e nao tiver nenhum cache modificado entao ele sai fora logo nao fica passando por todos
	//pra ver se estao limpos
	if (!mChangeAnyCache) return AC_OK;
	mChangeAnyCache = false;

	int i = 0;
	//position
	for (i = 0; i < NUM_CACHES; i++)
	{
		if (!mpCacheP[i]->IsEmpty )
			if (FAILED( mpCacheP[i]->Flush())) 
				return AC_FAIL;
	}

	//position colored
	for (i = 0; i < NUM_CACHES; i++)
	{
		if (!mpCachePC[i]->IsEmpty )
			if (FAILED( mpCachePC[i]->Flush())) 
				return AC_FAIL;
	}

	//position textured
	for (i = 0; i < NUM_CACHES; i++)
	{
		if (!mpCachePT[i]->IsEmpty )
			if (FAILED( mpCachePT[i]->Flush())) 
				return AC_FAIL;
	}

	//position normal textured
	for (i = 0; i < NUM_CACHES; i++)
	{
		if (!mpCachePNT[i]->IsEmpty )
			if (FAILED( mpCachePNT[i]->Flush())) 
				return AC_FAIL;
	}

	//position textured extrainfo
	for (i = 0; i < NUM_CACHES; i++)
	{
		if (!mpCachePTE[i]->IsEmpty )
			if (FAILED( mpCachePTE[i]->Flush())) 
				return AC_FAIL;
	}

	//Skinnedmesh
	for (i = 0; i < NUM_CACHES; i++)
	{
		if (!mpCacheSM[i]->IsEmpty )
			if (FAILED( mpCacheSM[i]->Flush())) 
				return AC_FAIL;
	}

	//Sprite (nos sprites tem q ser ao contrario pra manter a ordem da edição)
	for (i = NUM_CACHES - 1; i >=0 ; i--)
	{
		if (!mpCacheSP[i]->IsEmpty )
			if (FAILED( mpCacheSP[i]->Flush())) 
				return AC_FAIL;
	}
	return AC_OK;
};

HRESULT ACD3D10VertexManager::ForcedFlush(VertexFormat vertexFormat)
{
	ACD3D10VertexCache **pCache = nullptr;
	HRESULT hr = AC_OK;
	int i=0;

   	switch (vertexFormat)
	{
		case VertexFormat::VF_VertexPosition:					pCache = mpCacheP;		break;
		case VertexFormat::VF_VertexPositionColored:			pCache = mpCachePC;		break;
		case VertexFormat::VF_VertexPositionTextured:			pCache = mpCachePT;		break;
		case VertexFormat::VF_VertexPositionNormalTextured:		pCache = mpCachePNT;	break;
		case VertexFormat::VF_VertexPositionTexturedExtraInfo:	pCache = mpCachePTE;	break;
		case VertexFormat::VF_VertexSkinnedMesh:				pCache = mpCacheSM;		break;
		case VertexFormat::VF_VertexSprite:						pCache = mpCacheSP;		break;
		default: return AC_INVALIDFORMAT;
	};

	for (i = 0; i < NUM_CACHES; i++)
		if (FAILED(pCache[i]->Flush()))
			hr = AC_FAIL;

   return hr;
};

BOOL ACD3D10VertexManager::IsWire()
{
	return mIsWire;
};

void ACD3D10VertexManager::SetIsWire(BOOL value)
{
	mIsWire = value;
};

Vector3 ACD3D10VertexManager::GetWireColor()
{
	return mWireColor;
};

void ACD3D10VertexManager::SetWireColor(const Vector3& color)
{
	mWireColor = color;
};

void ACD3D10VertexManager::Release()
{
	SAFE_DELETE(mpCacheLines);
	SAFE_DELETE(mpCachePoints);

	for (int i = 0; i < NUM_CACHES; i++)
	{
		SAFE_DELETE(mpCacheP[i]);
		SAFE_DELETE(mpCachePC[i]);
		SAFE_DELETE(mpCachePT[i]);
		SAFE_DELETE(mpCachePNT[i]);
		SAFE_DELETE(mpCachePTE[i]);
		SAFE_DELETE(mpCacheSM[i]);
		SAFE_DELETE(mpCacheSP[i]);
	}
};

void ACD3D10VertexManager::Log(char* message, ...)
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