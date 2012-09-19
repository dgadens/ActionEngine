#include "ACModelDefinition.h"
#include "ACContentManager.h"
#include "ACRenderDevice.h"

ACModelDefinition::ACModelDefinition(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	pVertexBuffer = nullptr;

	mNumVertices = 0;
	mpVPNTCache = nullptr;
	mpVSMCache = nullptr;

	mNumIndices = 0;
	mpIndices = nullptr;

	mpJointMark = nullptr;
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
		VFormat = VertexFormat::VF_VertexSkinnedMesh;
		mRootJointMatrix = mpModel->pJoints[0]->BindMatrix; //armazena a matriz root pra usar sempre como base para aplicar a world
		PrepareVSM(mpModel);
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

	ACSkin* pSkin = mpCManager->CreateSkin();
	pSkin->Material.AmbientColor = model->pMaterials[0]->Ambient;
	pSkin->Material.EmissiveColor = model->pMaterials[0]->Emissive;
	pSkin->Material.SpecularColor = model->pMaterials[0]->Specular;
	pSkin->Material.SpecularPower = model->pMaterials[0]->SpecularPower;
	pSkin->Material.DiffuseColor = Vector4(&model->pMaterials[0]->Diffuse, model->pMaterials[0]->Transparency);

	//carrega as texturas
	if (model->pMaterials[0]->DiffuseTexture[0] != '\0')
		pSkin->Textures[0] = mpCManager->LoadTexture(model->pMaterials[0]->DiffuseTexture);
	if (model->pMaterials[0]->SpecularTexture[0]  != '\0')
		pSkin->Textures[1] = mpCManager->LoadTexture(model->pMaterials[0]->SpecularTexture);
	if (model->pMaterials[0]->NormalTexture[0]  != '\0')
		pSkin->Textures[2] = mpCManager->LoadTexture(model->pMaterials[0]->NormalTexture);
	if (model->pMaterials[0]->AnimatedTexture[0]  != '\0')
		pSkin->Textures[3] = mpCManager->LoadTexture(model->pMaterials[0]->AnimatedTexture);

	//ja cria o vertexbuffer na api
	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexPositionNormalTextured, 
								  mNumVertices, 
								  mNumIndices, 
								  mpVPNTCache, 
								  mpIndices, 
								  pSkin,
								  &pVertexBuffer);
};

void ACModelDefinition::PrepareVSM(AMT_MODEL* model)
{
	//cria a estrutura de bones
	//SetupBones(model);

	mNumVertices = model->Head.NumVertices;
	mpVSMCache = new ACVertexSkinnedMesh[mNumVertices];

	//seta os vertices para o array
	for (int i = 0; i < mNumVertices; i++)
	{
		mpVSMCache[i].position = model->pVertices[i]->Position;
		mpVSMCache[i].normal = model->pVertices[i]->Normal;
		mpVSMCache[i].texcoord = model->pVertices[i]->TexCoord1;
		mpVSMCache[i].blendWeights = Vector4(model->pVertices[i]->BoneWeight_A,
											 model->pVertices[i]->BoneWeight_B,
											 model->pVertices[i]->BoneWeight_C,
											 model->pVertices[i]->BoneWeight_D);
		mpVSMCache[i].blendIndices = Vector4(model->pVertices[i]->BoneID_A,
											 model->pVertices[i]->BoneID_B,
											 model->pVertices[i]->BoneID_C,
											 model->pVertices[i]->BoneID_D);

	}

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

	ACSkin* pSkin = mpCManager->CreateSkin();
	pSkin->Material.AmbientColor = model->pMaterials[0]->Ambient;
	pSkin->Material.EmissiveColor = model->pMaterials[0]->Emissive;
	pSkin->Material.SpecularColor = model->pMaterials[0]->Specular;
	pSkin->Material.SpecularPower = model->pMaterials[0]->SpecularPower;
	pSkin->Material.DiffuseColor = Vector4(&model->pMaterials[0]->Diffuse, model->pMaterials[0]->Transparency);

	//carrega as texturas
	if (model->pMaterials[0]->DiffuseTexture[0] != '\0')
		pSkin->Textures[0] = mpCManager->LoadTexture(model->pMaterials[0]->DiffuseTexture);
	if (model->pMaterials[0]->SpecularTexture[0]  != '\0')
		pSkin->Textures[1] = mpCManager->LoadTexture(model->pMaterials[0]->SpecularTexture);
	if (model->pMaterials[0]->NormalTexture[0]  != '\0')
		pSkin->Textures[2] = mpCManager->LoadTexture(model->pMaterials[0]->NormalTexture);
	if (model->pMaterials[0]->AnimatedTexture[0]  != '\0')
		pSkin->Textures[3] = mpCManager->LoadTexture(model->pMaterials[0]->AnimatedTexture);

	//ja cria o vertexbuffer na api
	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexSkinnedMesh, 
								  mNumVertices, 
								  mNumIndices, 
								  mpVSMCache, 
								  mpIndices, 
								  pSkin,
								  &pVertexBuffer);
};

//void ACModelDefinition::SetupBones(AMT_MODEL* model)
//{
	//mpLines = new ACVertexPositionColored[model->Head.NumJoints * 2 - 1];
	//GenerateLines(model->pJoints[0]);
//};

//void ACModelDefinition::GenerateLines(AMT_JOINT* joint)
//{
	//todo: metodo recursivo para buscar a arvore e adicionar as linhas
	//joint->
	//mpLines[i].position
//};

void ACModelDefinition::SetSkin(ACSkin* skin)
{
	//se ja existe um skin entao ele deleta para remover todos os resource e tb referencias depois adiciona outro
	if (pVertexBuffer->Skin != nullptr)
		SAFE_DELETE(pVertexBuffer->Skin);

	pVertexBuffer->Skin = mpCManager->CreateSkin();
	mpCManager->CloneSkin(pVertexBuffer->Skin, skin);
};

const ACSkin const * ACModelDefinition::GetSkin()
{
	return pVertexBuffer->Skin;
};

void ACModelDefinition::RenderBones(ACCamera* camera, Matrix& world)
{
	//seta para nao ter culling nos triangulos
	ACRASTERIZESTATE cr = mpGDevice->GetRasterizeState();
	ACDEPTHBUFFERSTATE ds = mpGDevice->GetDepthBufferState();
	mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullNone);
	mpGDevice->SetDepthBufferState(ACDEPTHBUFFERSTATE::ACDBS_WriteDisable);

	mpModel->pJoints[0]->BindMatrix = mRootJointMatrix * world;
	RenderBonesTree(camera, mpModel->pJoints[0]);

	//seta a rasterizacao anterior
	mpGDevice->SetRasterizeState(cr);
	mpGDevice->SetDepthBufferState(ds);
};

void ACModelDefinition::RenderBonesTree(ACCamera* camera, AMT_JOINT* joint)
{
	if (joint->ParentID != -1)
		joint->MatrixAbsolute = joint->BindMatrix * mpModel->pJoints[joint->ParentID]->MatrixAbsolute;
	else
		joint->MatrixAbsolute = joint->BindMatrix;

	mpJointMark->Render(camera, joint->MatrixAbsolute);

	if (joint->NumChildren <= 0)
		return;

	for (int i = 0; i < joint->NumChildren; i++)
		RenderBonesTree(camera, mpModel->pJoints[joint->JointChildren[i]]);
};

void ACModelDefinition::Release()
{
	//remove o skin e tb as texturas e tudo mais
	SAFE_RELEASE(pVertexBuffer->Skin);
	SAFE_DELETE(mpJointMark);

	//remove o vb e ib da api
	if (pVertexBuffer!=nullptr)
	{
		mpGDevice->ReleaseBuffer(pVertexBuffer);
		pVertexBuffer = nullptr;
	}
};
