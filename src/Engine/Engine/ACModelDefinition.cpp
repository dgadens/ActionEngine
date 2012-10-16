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
	mNumVertices = model->Head.NumVertices;
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

void ACModelDefinition::UpdateBones(Matrix& world)
{
	//pega o joint root e passa por todos os  joints
	AMT_JOINT* rootJoint = mpModel->pJoints[0];
	rootJoint->BindMatrix = mpModel->pOriginalJoints[0]->BindMatrix * world;
	rootJoint->MatrixAbsolute = rootJoint->BindMatrix;

	for (int i = 0; i < rootJoint->NumChildren; i++)
		UpdateBones(mpModel->pJoints[rootJoint->JointChildren[i]]);
};

void ACModelDefinition::UpdateBones(AMT_JOINT* joint)
{
	if (joint->ParentID != -1)
		joint->MatrixAbsolute = joint->BindMatrix * mpModel->pJoints[joint->ParentID]->MatrixAbsolute;

	if (joint->NumChildren <= 0)
		return;

	for (int i = 0; i < joint->NumChildren; i++)
		UpdateBones(mpModel->pJoints[joint->JointChildren[i]]);
};

void ACModelDefinition::RenderModel(ACCamera* camera)
{
	if (HasSkeleton)
	{
		//renderiza o model
		for (int i = 0; i < mNumVertices; i++)
		{
			Matrix skinTransform;
			skinTransform.M41 = 0;
			skinTransform.M42 = 0;
			skinTransform.M43 = 0;
			skinTransform.M44 = 0;

			int indexBoneA = mpModel->pVertices[i]->BoneIndices[0];
			int indexBoneB = mpModel->pVertices[i]->BoneIndices[1];
			int indexBoneC = mpModel->pVertices[i]->BoneIndices[2];
			int indexBoneD = mpModel->pVertices[i]->BoneIndices[3];

			Matrix pqp1 = mpModel->pOriginalJoints[indexBoneA]->InverseBindMatrix * mpModel->pJoints[indexBoneA]->MatrixAbsolute;
			Matrix pqp2 = mpModel->pOriginalJoints[indexBoneB]->InverseBindMatrix * mpModel->pJoints[indexBoneB]->MatrixAbsolute;
			Matrix pqp3 = mpModel->pOriginalJoints[indexBoneC]->InverseBindMatrix * mpModel->pJoints[indexBoneC]->MatrixAbsolute;
			Matrix pqp4 = mpModel->pOriginalJoints[indexBoneD]->InverseBindMatrix * mpModel->pJoints[indexBoneD]->MatrixAbsolute;
		
			skinTransform = skinTransform + pqp1 * mpModel->pVertices[i]->BoneWeights[0];
			skinTransform = skinTransform + pqp2 * mpModel->pVertices[i]->BoneWeights[1];
			skinTransform = skinTransform + pqp3 * mpModel->pVertices[i]->BoneWeights[2];
			skinTransform = skinTransform + pqp4 * mpModel->pVertices[i]->BoneWeights[3];
		
			Vector3::Transform(&mpModel->pVertices[i]->Position, &skinTransform , &mpVSMCache[i].position);
			Vector3::TransformNormal(&mpModel->pVertices[i]->Normal, &skinTransform , &mpVSMCache[i].normal);
		}

		mpGDevice->Render(VertexFormat::VF_VertexSkinnedMesh, 
									  mNumVertices, 
									  mNumIndices, 
									  mpVSMCache, 
									  mpIndices, 
									  mpSkin);
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

	SAFE_DELETE(mpModel);

	//remove o vb e ib da api
	if (pVertexBuffer!=nullptr)
	{
		mpGDevice->ReleaseBuffer(pVertexBuffer);
		pVertexBuffer = nullptr;
	}
};
