#include "ACModelDefinition.h"
#include "ACContentManager.h"
#include "ACRenderDevice.h"

ACModelDefinition::ACModelDefinition(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	pVertexBuffer = nullptr;

	mpLines = nullptr;

	mNumVertices = 0;
	mpVPNTCache = nullptr;
	mpVSMCache = nullptr;

	mpSkin = nullptr;

	mNumIndices = 0;
	mpIndices = nullptr;

	mpJointMark = nullptr;

	mpAnimationController = nullptr;
};

ACModelDefinition::~ACModelDefinition()
{
	Release();
};

void ACModelDefinition::Prepare(AMT_MODEL* amtModel)
{
	mpModel = amtModel;

	//cria o minimo bb ao redor do modelo
	Vector3* points = new Vector3[amtModel->Head.NumVertices];
	for(int i=0;i<mpModel->Head.NumVertices;i++)
		points[i] = mpModel->pVertices[i]->Position;

	//cria o bb
	BoundingBox::CreateFromPoints(mpModel->Head.NumVertices, points, &BBOriginal);
	//cria o obb
	OrientedBoundingBox::CreateFromBB(&BBOriginal, &OBBOriginal);

	delete[] points;

	HasSkeleton = mpModel->Head.HasSkeleton;
	HasAnimation = mpModel->Head.NumAnimations > 0;

	//verifica o tipo de vertices se tiver bones entao é um skinnedmesh senao um mesh normal
	if (amtModel->Head.HasSkeleton)
	{
		//cria as marcacoes dos bones
		mpJointMark = new ACMark(mpGDevice, mpCManager); 

		//se for skin na gpu entao ele usa o VB com skin senao usa o padrao e vai pela gpu mesmo
		if (ACConfigurations::Instance()->GetIsGPUSkinning())
			VFormat = VertexFormat::VF_VertexSkinnedMesh;
		else
			VFormat = VertexFormat::VF_VertexPositionNormalTextured;

		PrepareVSM(mpModel);
		mpAnimationController = new ACAnimationController(mpModel);
	}
	else
	{
		VFormat = VertexFormat::VF_VertexPositionNormalTextured;
		PrepareVPNT(mpModel);
	}
};

void ACModelDefinition::PrepareVPNT(AMT_MODEL* model)
{
	mNumVertices = model->Head.NumVertices;
	mpVPNTCache = new ACVertexPositionNormalTextured[mNumVertices];

	//seta os vertices para o array
	for (int i = 0; i < mNumVertices; i++)
	{
		mpVPNTCache[i].position = model->pVertices[i]->Position;
		mpVPNTCache[i].normal = model->pVertices[i]->Normal;
		mpVPNTCache[i].texcoord = model->pVertices[i]->TexCoord1;
	}

	PrepareIndices(model);
	PrepareSkin(model);

	//ja cria o vertexbuffer na api
	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexPositionNormalTextured, 
								  mNumVertices, 
								  mNumIndices, 
								  mpVPNTCache, 
								  mpIndices, 
								  mpSkin,
								  &pVertexBuffer);
};

void ACModelDefinition::PrepareVSM(AMT_MODEL* model)
{
	mNumVertices = model->Head.NumVertices;

	if (VFormat == VertexFormat::VF_VertexSkinnedMesh)
	{
		mpVSMCache = new ACVertexSkinnedMesh[mNumVertices];

		//seta os vertices para o array
		for (int i = 0; i < mNumVertices; i++)
		{
			mpVSMCache[i].position = model->pVertices[i]->Position;
			mpVSMCache[i].normal = model->pVertices[i]->Normal;
			mpVSMCache[i].texcoord = model->pVertices[i]->TexCoord1;
			mpVSMCache[i].blendWeights = Vector4(model->pVertices[i]->BoneWeights[0],
												 model->pVertices[i]->BoneWeights[1],
												 model->pVertices[i]->BoneWeights[2],
												 model->pVertices[i]->BoneWeights[3]);
			mpVSMCache[i].blendIndices = Vector4(model->pVertices[i]->BoneIndices[0],
												 model->pVertices[i]->BoneIndices[1],
												 model->pVertices[i]->BoneIndices[2],
												 model->pVertices[i]->BoneIndices[3]);

		}
	}
	else
	{
		mpVPNTCache = new ACVertexPositionNormalTextured[mNumVertices];

		//seta os vertices para o array
		for (int i = 0; i < mNumVertices; i++)
		{
			mpVPNTCache[i].position = model->pVertices[i]->Position;
			mpVPNTCache[i].normal = model->pVertices[i]->Normal;
			mpVPNTCache[i].texcoord = model->pVertices[i]->TexCoord1;
		}
	}

	PrepareIndices(model);
	PrepareSkin(model);

	//se for skin na gpu entao ele cria um staticVB senao é dinamico pq ele movimenta os vertices no update
	if (ACConfigurations::Instance()->GetIsGPUSkinning())
	{
		mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexSkinnedMesh, 
									  mNumVertices, 
									  mNumIndices, 
									  mpVSMCache, 
									  mpIndices, 
									  mpSkin,
									  &pVertexBuffer);
	}
};

void ACModelDefinition::PrepareIndices(AMT_MODEL* model)
{
	//atribui os indices
	mNumIndices = model->pFaces.size() * 3;
	mpIndices = new UINT[mNumIndices];
	int index = 0;
	for (int i = 0; i< model->pFaces.size(); i++)
	{
		mpIndices[index++] = model->pFaces[i]->Indices[0];
		mpIndices[index++] = model->pFaces[i]->Indices[1];
		mpIndices[index++] = model->pFaces[i]->Indices[2];
	}
};

void ACModelDefinition::PrepareSkin(AMT_MODEL* model)
{
	mpSkin = mpCManager->CreateSkin();
	mpSkin->Material.AmbientColor = model->pMaterials[0]->Ambient;
	mpSkin->Material.EmissiveColor = model->pMaterials[0]->Emissive;
	mpSkin->Material.SpecularColor = model->pMaterials[0]->Specular;
	mpSkin->Material.SpecularPower = model->pMaterials[0]->SpecularPower;
	mpSkin->Material.DiffuseColor = Vector4(&model->pMaterials[0]->Diffuse, model->pMaterials[0]->Transparency);

	//carrega as texturas
	if (model->pMaterials[0]->DiffuseTexture[0] != '\0')
		mpSkin->Textures[0] = mpCManager->LoadTexture(model->pMaterials[0]->DiffuseTexture);
	if (model->pMaterials[0]->SpecularTexture[0]  != '\0')
		mpSkin->Textures[1] = mpCManager->LoadTexture(model->pMaterials[0]->SpecularTexture);
	if (model->pMaterials[0]->NormalTexture[0]  != '\0')
		mpSkin->Textures[2] = mpCManager->LoadTexture(model->pMaterials[0]->NormalTexture);
	if (model->pMaterials[0]->AnimatedTexture[0]  != '\0')
		mpSkin->Textures[3] = mpCManager->LoadTexture(model->pMaterials[0]->AnimatedTexture);
};

void ACModelDefinition::SetSkin(ACSkin* skin)
{
	//se ja existe um skin entao ele deleta para remover todos os resource e tb referencias depois adiciona outro
	if (mpSkin != nullptr)
		SAFE_DELETE(mpSkin);

	mpSkin = mpCManager->CreateSkin();
	mpCManager->CloneSkin(mpSkin, skin);
};

const ACSkin const * ACModelDefinition::GetSkin()
{
	return mpSkin;
};

Matrix* ACModelDefinition::GetSkinMatrizes()
{
	return mpAnimationController->GetSkinMatrizes();
};

UINT ACModelDefinition::GetNumberOfBones()
{
	return mpModel->Head.NumJoints;
};

void ACModelDefinition::ActiveAnimation(const std::string& name)
{
	mpAnimationController->SetAnimation(name);
};

void ACModelDefinition::Update(FLOAT elapsedTime, Matrix& world)
{
	mpAnimationController->Update(elapsedTime, world);
};

void ACModelDefinition::RenderModel(ACCamera* camera)
{
	if (HasSkeleton)
	{

		//renderiza o model (se for skinning na gpu entao ele usa o shader especifico senao o normal mesmo
		if (ACConfigurations::Instance()->GetIsGPUSkinning())
			mpGDevice->Render(pVertexBuffer);
		else
		{
			//CPU SKINNING
			Matrix* skinMatrizes = GetSkinMatrizes();
			for (int i = 0; i < mNumVertices; i++)
			{
				Matrix skinTransform;
				skinTransform.M41 = 0;
				skinTransform.M42 = 0;
				skinTransform.M43 = 0;
				skinTransform.M44 = 0;

				skinTransform = skinTransform + skinMatrizes[mpModel->pVertices[i]->BoneIndices[0]] * mpModel->pVertices[i]->BoneWeights[0];
				skinTransform = skinTransform + skinMatrizes[mpModel->pVertices[i]->BoneIndices[1]] * mpModel->pVertices[i]->BoneWeights[1];
				skinTransform = skinTransform + skinMatrizes[mpModel->pVertices[i]->BoneIndices[2]] * mpModel->pVertices[i]->BoneWeights[2];
				skinTransform = skinTransform + skinMatrizes[mpModel->pVertices[i]->BoneIndices[3]] * mpModel->pVertices[i]->BoneWeights[3];
		
				Vector3::Transform(&mpModel->pVertices[i]->Position, &skinTransform , &mpVPNTCache[i].position);
				Vector3::TransformNormal(&mpModel->pVertices[i]->Normal, &skinTransform , &mpVPNTCache[i].normal);
			}

			mpGDevice->Render(VertexFormat::VF_VertexPositionNormalTextured, 
										  mNumVertices, 
										  mNumIndices, 
										  mpVPNTCache, 
										  mpIndices, 
										  mpSkin);
		}
	}
	else
		mpGDevice->Render(pVertexBuffer);
};

void ACModelDefinition::RenderBones(ACCamera* camera)
{
	//seta para nao ter culling nos triangulos
	ACRASTERIZESTATE cr = mpGDevice->GetRasterizeState();
	mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullNone);
	ACDEPTHBUFFERSTATE ds = mpGDevice->GetDepthBufferState();
	mpGDevice->SetDepthBufferState(ACDEPTHBUFFERSTATE::ACDBS_WriteDisable);

	for (int i = 0; i < mpModel->Head.NumJoints; i++)
		mpJointMark->Render(camera, mpModel->pJoints[i]->MatrixAbsolute);

	//seta a rasterizacao anterior
	mpGDevice->SetDepthBufferState(ds);
	mpGDevice->SetRasterizeState(cr);
};

void ACModelDefinition::Release()
{
	//remove o skin e tb as texturas e tudo mais
	SAFE_RELEASE(mpSkin);
	SAFE_DELETE(mpJointMark);
	SAFE_DELETE(mpAnimationController);
	SAFE_DELETE(mpModel);

	//remove o vb e ib da api
	if (pVertexBuffer!=nullptr)
	{
		mpGDevice->ReleaseBuffer(pVertexBuffer);
		pVertexBuffer = nullptr;
	}
};
