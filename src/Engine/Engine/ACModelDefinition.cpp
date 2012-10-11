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

	//ja cria o vertexbuffer na api
	/*mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexSkinnedMesh, 
								  mNumVertices, 
								  mNumIndices, 
								  mpVSMCache, 
								  mpIndices, 
								  mpSkin,
								  &pVertexBuffer);*/
};

void ACModelDefinition::SetSkin(ACSkin* skin)
{
	//se ja existe um skin entao ele deleta para remover todos os resource e tb referencias depois adiciona outro
	if (mpSkin != nullptr)
		SAFE_DELETE(mpSkin);

	mpSkin = mpCManager->CreateSkin();
	mpCManager->CloneSkin(mpSkin, skin);

	//pVertexBuffer->Skin = mpSkin;
};

const ACSkin const * ACModelDefinition::GetSkin()
{
	return mpSkin;
};

void ACModelDefinition::RenderBones(ACCamera* camera, Matrix& world)
{


	//seta para nao ter culling nos triangulos
	ACRASTERIZESTATE cr = mpGDevice->GetRasterizeState();
	ACDEPTHBUFFERSTATE ds = mpGDevice->GetDepthBufferState();
	mpGDevice->SetRasterizeState(ACRASTERIZESTATE::ACRS_SolidCullNone);
	mpGDevice->SetDepthBufferState(ACDEPTHBUFFERSTATE::ACDBS_WriteDisable);

	mpModel->pJoints[0]->BindMatrix = mRootJointMatrix * world;
	mpModel->pJoints[0]->MatrixAbsolute = mpModel->pJoints[0]->BindMatrix;
	RenderBonesTree(camera, mpModel->pJoints[0]);

	//seta a rasterizacao anterior
	mpGDevice->SetRasterizeState(cr);
	mpGDevice->SetDepthBufferState(ds);


			for (int i = 0; i < mNumVertices; i++)
	{
		Matrix skinTransform;
		skinTransform.M41 = 0;
		skinTransform.M42 = 0;
		skinTransform.M43 = 0;
		skinTransform.M44 = 0;

		int indexBoneA = mpModel->pVertices[i]->BoneID_A;
		int indexBoneB = mpModel->pVertices[i]->BoneID_B;
		int indexBoneC = mpModel->pVertices[i]->BoneID_C;
		int indexBoneD = mpModel->pVertices[i]->BoneID_D;

		Matrix pqp1 = mpModel->pOriginalJoints[indexBoneA]->InverseBindMatrix * mpModel->pOriginalJoints[indexBoneA]->MatrixAbsolute;
		Matrix pqp2 = mpModel->pOriginalJoints[indexBoneB]->InverseBindMatrix * mpModel->pOriginalJoints[indexBoneB]->MatrixAbsolute;
		Matrix pqp3 = mpModel->pOriginalJoints[indexBoneC]->InverseBindMatrix * mpModel->pOriginalJoints[indexBoneC]->MatrixAbsolute;
		Matrix pqp4 = mpModel->pOriginalJoints[indexBoneD]->InverseBindMatrix * mpModel->pOriginalJoints[indexBoneD]->MatrixAbsolute;
		
		skinTransform = skinTransform + pqp1 * mpModel->pVertices[i]->BoneWeight_A;
		skinTransform = skinTransform + pqp2 * mpModel->pVertices[i]->BoneWeight_B;
		skinTransform = skinTransform + pqp3 * mpModel->pVertices[i]->BoneWeight_C;
		skinTransform = skinTransform + pqp4 * mpModel->pVertices[i]->BoneWeight_D;
		
		Vector3::Transform(&mpModel->pVertices[i]->Position, &skinTransform , &mpVSMCache[i].position);
	}
	
	mpGDevice->SetWorldMatrix(world);
	mpGDevice->Render(VertexFormat::VF_VertexSkinnedMesh, 
								  mNumVertices, 
								  mNumIndices, 
								  mpVSMCache, 
								  mpIndices, 
								  mpSkin);

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
	SAFE_RELEASE(mpSkin);
	SAFE_DELETE(mpJointMark);

	//remove o vb e ib da api
	if (pVertexBuffer!=nullptr)
	{
		mpGDevice->ReleaseBuffer(pVertexBuffer);
		pVertexBuffer = nullptr;
	}
};
