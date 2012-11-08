#include "ACD3DVertexManager.h"
#include "ACD3D.h"

ACD3DVertexManager::ACD3DVertexManager(ACD3D* pACD3D, 
									   ID3D11Device* gDevice, 
									   UINT maxVertices, 
									   UINT maxIndices, 
									   FILE* log)
{
	mpACD3D = pACD3D;
	mpLOG = log;
	mpCurrentVertexBuffer = nullptr;
	mpGDevice = gDevice;
	mpGDevice->GetImmediateContext(&mpContext);
	mIsWire = false;
	mChangeAnyCache = false;

	mMaxVertices = maxVertices;
	mMaxIndices = maxIndices;

	for (int i = 0; i < NUM_CACHES; i++)
	{
		//position 
		mpCacheP[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPosition ), VertexFormat::VF_VertexPosition, log);
		//position colored
		mpCachePC[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionColored ), VertexFormat::VF_VertexPositionColored, log);
		//position textured	
		mpCachePT[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionTextured ), VertexFormat::VF_VertexPositionTextured, log);
		//position normal textured
		mpCachePNT[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionNormalTextured ), VertexFormat::VF_VertexPositionNormalTextured, log);
		//position textured extra info
		mpCachePTE[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionTexturedExtraInfo ), VertexFormat::VF_VertexPositionTexturedExtraInfo, log);
		//skinedmesh
		mpCacheSM[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexSkinnedMesh ), VertexFormat::VF_VertexSkinnedMesh, log);
		//sprite
		mpCacheSP[i] = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexSprite ), VertexFormat::VF_VertexSprite, log);
	}

	//dynamicbuffer para as linhas
	mpCacheLines = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionColored ), VertexFormat::VF_VertexPositionColored, log);
	//dynamicbuffer para os pontos
	mpCachePoints = new ACD3DVertexCache(this, gDevice, mMaxVertices, mMaxIndices, sizeof ( ACVertexPositionColored ), VertexFormat::VF_VertexPositionColored, log);
};

ACD3DVertexManager::~ACD3DVertexManager()
{
	Release();
};

HRESULT ACD3DVertexManager::CreateStaticBuffer(VertexFormat vertexFormat, 
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
	ID3D11Buffer* pVB = nullptr;

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
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = byteWidth; 
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vbInitData;
	vbInitData.pSysMem = pVertices;

	hr = mpGDevice->CreateBuffer( &vbd, &vbInitData, &pVB );
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
		ID3D11Buffer* pIB = nullptr;
		byteWidth = sizeof( UINT ) * numIndices;

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = byteWidth; 
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA ibInitData;
		ibInitData.pSysMem = pIndices;

		hr = mpGDevice->CreateBuffer(&ibd, &ibInitData, &pIB);
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

void ACD3DVertexManager::InvalidateStaticBuffer()
{
	mpCurrentVertexBuffer = nullptr;
	mpCurrentSkin = nullptr;
};

void ACD3DVertexManager::ReleaseBuffer(ACVertexBuffer* vertexBuffer)
{
	ID3D11Buffer* v = static_cast<ID3D11Buffer*>(vertexBuffer->pVB);

	if (vertexBuffer->pIB != nullptr)
	{
		ID3D11Buffer* i = static_cast<ID3D11Buffer*>(vertexBuffer->pIB);
		SAFE_RELEASE(i);
	}

	SAFE_RELEASE(v);
};

HRESULT ACD3DVertexManager::Render(ACVertexBuffer* vertexBuffer)
{
	//se nao for o skin corrente entao ele tem q setar os dados
	if (vertexBuffer->Skin!=mpCurrentSkin)
	{
		mpCurrentSkin = vertexBuffer->Skin;

		ACSHADEMODE shadeMode = mpACD3D->GetShadeMode();

		//se for modo wireframe entao ele usa a cor pra setar o material e ja tira todas as texturas pra nao dar zebra
		if (mIsWire)
		{
			////manda todo o skin para a gpu para renderizar
			Vector4 wc(&mWireColor, 1);
			mpACD3D->SetMaterialAmbient(mWireColor);
			mpACD3D->SetMaterialDiffuse(wc);
			mpACD3D->SetMaterialSpecular(mWireColor);
			mpACD3D->SetMaterialSpecularPower(1);
			mpACD3D->SetMaterialEmissive(mWireColor);
			for (int i = 0; i < NUM_TEXTURES; i++)
				mpACD3D->SetTexture(nullptr, i);

			mpACD3D->ApplyConstants();
		}
		else
		{
			if (mpCurrentSkin != nullptr)
			{
				////manda todo o skin para a gpu para renderizar
				if (mpCurrentSkin->UseMaterial)
				{
					mpACD3D->SetMaterialAmbient(mpCurrentSkin->Material.AmbientColor);
					mpACD3D->SetMaterialDiffuse(mpCurrentSkin->Material.DiffuseColor);
					mpACD3D->SetMaterialSpecular(mpCurrentSkin->Material.SpecularColor);
					mpACD3D->SetMaterialSpecularPower(mpCurrentSkin->Material.SpecularPower);
					mpACD3D->SetMaterialEmissive(mpCurrentSkin->Material.EmissiveColor);
				}

				//se for triangulo entao ele adiciona as texturas senao ele tira todas
				if (shadeMode == ACSHADEMODE::ACSM_TriangleList ||
					shadeMode == ACSHADEMODE::ACSM_TriangleStrip ||
					shadeMode == ACSHADEMODE::ACSM_PointSprite) //sprites usam points no directx 10+
				{
					//se for pointsprite ele usa so a primeira textura
					if (shadeMode == ACSHADEMODE::ACSM_PointSprite)
						mpACD3D->SetTexture(vertexBuffer->Skin->Textures[0], 0);
					else
					{
						for (UINT i = 0; i < NUM_TEXTURES; i++)
							mpACD3D->SetTexture(mpCurrentSkin->Textures[i], i);
					}
				}
				else
				{
					for (UINT i = 0; i < NUM_TEXTURES; i++)
						mpACD3D->SetTexture(nullptr, i);
				}

				mpACD3D->ApplyConstants();
			}
		}
	}

	//se nao for o vb corrente ele seta
	if (vertexBuffer != mpCurrentVertexBuffer)
	{
		mpCurrentVertexBuffer = vertexBuffer;

		UINT offset = 0;
	
		ID3D11Buffer* v = static_cast<ID3D11Buffer*>(mpCurrentVertexBuffer->pVB);
		mpContext->IASetVertexBuffers( 0, 1, &v, &mpCurrentVertexBuffer->Stride, &offset );

		switch (mpACD3D->GetShadeMode())
		{
			case ACSHADEMODE::ACSM_TriangleList:	mpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );		break;
			case ACSHADEMODE::ACSM_TriangleStrip:	mpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );	break;
			case ACSHADEMODE::ACSM_LineList:		mpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );			break;
			case ACSHADEMODE::ACSM_LineStrip:		mpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );		break;
			case ACSHADEMODE::ACSM_Point: 
			case ACSHADEMODE::ACSM_PointSprite:		mpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );		break;
		};

		//se existe IB entao ele seta
		if (mpCurrentVertexBuffer->pIB != nullptr)
		{
			ID3D11Buffer* i = static_cast<ID3D11Buffer*>(mpCurrentVertexBuffer->pIB);
			mpContext->IASetIndexBuffer(i, DXGI_FORMAT_R32_UINT, 0);

			mpContext->DrawIndexed( mpCurrentVertexBuffer->NumIndices, 0, 0);
		}
		else
			mpContext->Draw( mpCurrentVertexBuffer->NumVertices, 0);
	}
	else
	{
		//se existe IB entao ele seta
		if (mpCurrentVertexBuffer->pIB != nullptr)
		{
			mpContext->DrawIndexed( mpCurrentVertexBuffer->NumIndices, 0, 0);
		}
		else
			mpContext->Draw( mpCurrentVertexBuffer->NumVertices, 0);
	}

	return AC_OK;
};


HRESULT ACD3DVertexManager::RenderLines(UINT numVertices, 
										UINT numIndices, 
										ACVertexPositionColored* vertices, 
										UINT* indices,
										BOOL strip)
{
	//invalida o staticbuffer e tb o skin corrente
	InvalidateStaticBuffer();

	// se precisar ele ja executa o flush antes de trocar o estado
	mpACD3D->SetBlendState(ACBLENDSTATE::ACBS_Opaque);

	//pega o atual para setar no final
	ACSHADEMODE currentSM = mpACD3D->GetShadeMode();
	
	if (strip)
		mpACD3D->SetShadeMode(ACSHADEMODE::ACSM_LineStrip);
	else
		mpACD3D->SetShadeMode(ACSHADEMODE::ACSM_LineList);

	mpCacheLines->Add(numVertices, numIndices, vertices, indices, nullptr);
	mpCacheLines->Flush();

	mpACD3D->SetShadeMode(currentSM);

	return AC_OK;
};


HRESULT ACD3DVertexManager::RenderPoints(UINT numVertices, 
										 ACVertexPositionColored* vertices)
{
	//invalida o staticbuffer e tb o skin corrente
	InvalidateStaticBuffer();

	// se precisar ele ja executa o flush antes de trocar o estado
	mpACD3D->SetBlendState(ACBLENDSTATE::ACBS_Opaque);

	//pega o atual para setar no final
	ACSHADEMODE currentSM = mpACD3D->GetShadeMode();
	
	mpACD3D->SetShadeMode(ACSHADEMODE::ACSM_Point);

	mpCachePoints->Add(numVertices, 0, vertices, nullptr, nullptr);
	mpCachePoints->Flush();

	mpACD3D->SetShadeMode(currentSM);

	return AC_OK;
};

HRESULT ACD3DVertexManager::Render(VertexFormat vertexFormat, 
									 UINT numVertices, 
									 UINT numIndices, 
									 void* vertices, 
									 UINT* indices, 
									 ACSkin* skin)
{
	ACD3DVertexCache **pCache=nullptr,
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

HRESULT ACD3DVertexManager::ForcedFlushAll()
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

HRESULT ACD3DVertexManager::ForcedFlush(VertexFormat vertexFormat)
{
	ACD3DVertexCache **pCache = nullptr;
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

BOOL ACD3DVertexManager::IsWire()
{
	return mIsWire;
};

void ACD3DVertexManager::SetIsWire(BOOL value)
{
	mIsWire = value;
};

Vector3 ACD3DVertexManager::GetWireColor()
{
	return mWireColor;
};

void ACD3DVertexManager::SetWireColor(const Vector3& color)
{
	mWireColor = color;
};

void ACD3DVertexManager::Release()
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

void ACD3DVertexManager::Log(char* message, ...)
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