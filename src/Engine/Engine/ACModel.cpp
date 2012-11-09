#include "ACModel.h"
#include "ACRenderDevice.h"

ACModel::ACModel(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

    mPosition = Vector3();
	mScale = Vector3(1);
	mRotation = Vector3();

	mRenderBones = FALSE;
	mRenderNormals = FALSE;

	World = Matrix();
	BB = BoundingBox();
	OBB = OrientedBoundingBox();

	pModelDefinition = nullptr;

	mChangeTransformation = FALSE;

	IsActive = TRUE; //faz o update
	IsVisible = TRUE; //renderiza
	IsDead = FALSE; //nao faz porra nenhuma

	mpVS = nullptr;
	mpPS = nullptr;
	mpGS = nullptr;
};

ACModel::~ACModel()
{
	Release();
};

void ACModel::Release()
{
	mpCManager->RemoveVertexShader(mpVS);
	mpCManager->RemovePixelShader(mpPS);
	mpCManager->RemoveGeometryShader(mpGS);

	//remove do contentmanager
	mpCManager->RemoveModel(pModelDefinition);
};

#pragma region SKIN

void ACModel::SetSkin(ACSkin* skin)
{
	pModelDefinition->SetSkin(skin);
};

ACSkin* ACModel::GetSkin()
{
	return pModelDefinition->GetSkin();
}

#pragma endregion

#pragma region AUX DATA

void ACModel::SetRenderBones(BOOL value)
{
	mRenderBones = value;
};

void ACModel::SetRenderNormals(BOOL value)
{
	mRenderNormals = value;
};

#pragma endregion

#pragma region CUSTOM SHADERS

void ACModel::SetVertexShader(const ACVertexShader* vs)
{
	//se ja estiver vinculado um vs entao ele remove para depois setar o novo, dessa forma o cmanager controla o numero de instancias
	if (mpVS != nullptr)
		mpCManager->RemoveVertexShader(mpVS);

	mpVS = mpCManager->LoadVertexShader(vs->Name, vs->Format);
};

void ACModel::SetPixelShader(const ACPixelShader* ps)
{
	//se ja estiver vinculado um ps entao ele remove para depois setar o novo, dessa forma o cmanager controla o numero de instancias
	if (mpPS != nullptr)
		mpCManager->RemovePixelShader(mpPS);

	mpPS = mpCManager->LoadPixelShader(ps->Name);

};

void ACModel::SetGeometryShader(const ACGeometryShader* gs)
{
	//se ja estiver vinculado um gs entao ele remove para depois setar o novo, dessa forma o cmanager controla o numero de instancias
	if (mpGS != nullptr)
		mpCManager->RemoveGeometryShader(mpGS);

	mpGS = mpCManager->LoadGeometryShader(gs->Name);
};

#pragma endregion

#pragma region BUILD GEOMETRY

void ACModel::Load(const std::string& name)
{
	pModelDefinition = mpCManager->LoadModel(name);

	//carrega os shaders padrao para modelos carregados de arquivo, se for skin na gpu entao é outro vs
	if (pModelDefinition->HasSkeleton && ACConfigurations::Instance()->USE_GPU_SKINNING)
		mpVS = mpCManager->LoadVertexShader("SkinnedMesh.VShlsl4", VertexFormat::VF_VertexSkinnedMesh);
	else
		mpVS = mpCManager->LoadVertexShader("LightingTextured.VShlsl4", VertexFormat::VF_VertexPositionNormalTextured);

	mpPS = mpCManager->LoadPixelShader("LightingTextured.PShlsl4");
};

#pragma endregion

#pragma region Animations

void ACModel::ActiveAnimation(const std::string& name)
{
	pModelDefinition->ActiveAnimation(name);
};

#pragma endregion

#pragma region UPDATE

void ACModel::Update(FLOAT elapsedTime)
{
	if (IsActive)
	{
		if (mChangeTransformation)
		{
			Matrix scale;
			Matrix::CreateScale(&mScale, &scale);
			Matrix rotX;
			Matrix::CreateRotationX(mRotation.X, &rotX);
			Matrix rotY;
			Matrix::CreateRotationY(mRotation.Y, &rotY);
			Matrix rotZ;
			Matrix::CreateRotationZ(mRotation.Z, &rotZ);
			Matrix translation;
			Matrix::CreateTranslation(&mPosition, &translation);

			World = scale * rotX * rotY * rotZ * translation;

			//transforma o OBB
	/*		OrientedBoundingBox::TransformOBB(&pModelDefinition->OBBOriginal, &World, &OBB);
			BoundingBox::CreateFromPoints(OrientedBoundingBox::CORNER_COUNT, OBB.pEdges, &BB);*/

			mChangeTransformation = FALSE;
		}

		if (pModelDefinition->HasSkeleton)
			pModelDefinition->Update(elapsedTime, World);
	}
};

#pragma endregion

#pragma region RENDER

void ACModel::Render(ACCamera* camera)
{
	if (IsVisible)
	{
		//ativa os shaders
		mpGDevice->ActiveVS(mpVS);
		mpGDevice->ActivePS(mpPS);

		//se tiver esqueleto entao a matriz world tem q ser a identidade
		if (pModelDefinition->HasSkeleton)
		{
			//se for na gpu skin entao ele passa a lista de matrizes
			//senao ele passa a matriz identidade para world
			if (ACConfigurations::Instance()->USE_GPU_SKINNING)
				mpGDevice->SetSkinMatrizes(pModelDefinition->GetNumberOfBones(),
										   pModelDefinition->GetSkinMatrizes());
			else
			{
				Matrix identity;
				mpGDevice->SetWorldMatrix(identity);
			}
		}
		else
			mpGDevice->SetWorldMatrix(World);

		mpGDevice->ApplyConstants();

		//renderiza
		pModelDefinition->RenderModel(camera);
	}

	if (mRenderBones)
	{
		if (pModelDefinition->HasSkeleton)
			pModelDefinition->RenderBones(camera);
	}

	if (mRenderNormals)
	{
	}
};

#pragma endregion

#pragma region TRANSFORMATIONS
//position
void ACModel::SetAbsolutePosition(float x, float y, float z)
{
	if (mPosition.X != x || mPosition.Y != y || mPosition.Z != z)
	{
		mPosition.X = x;
		mPosition.Y = y;
		mPosition.Z = z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::SetAbsolutePosition(Vector3* value)
{
	if (mPosition.X != value->X || mPosition.Y != value->Y || mPosition.Z != value->Z)
	{
		mPosition.X = value->X;
		mPosition.Y = value->Y;
		mPosition.Z = value->Z;
		
		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToPosition(float x, float y, float z)
{
	if (x!=0 || y!=0 || z!=0)
	{
		mPosition.X += x;
		mPosition.Y += y;
		mPosition.Z += z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToPosition(Vector3* value)
{
	if (value->X !=0 || value->Y !=0 || value->Z !=0)
	{
		mPosition.X += value->X;
		mPosition.Y += value->Y;
		mPosition.Z += value->Z;

		mChangeTransformation = TRUE;
	}
};

//scale
void ACModel::SetAbsoluteScale(float value)
{
	if (mScale.X != value || mScale.Y != value || mScale.Z != value)
	{
		mScale.X = value;
		mScale.Y = value;
		mScale.Z = value;

		mChangeTransformation = TRUE;
	}
};

void ACModel::SetAbsoluteScale(float x, float y, float z)
{
	if (mScale.X != x || mScale.Y != y || mScale.Z != z)
	{
		mScale.X = x;
		mScale.Y = y;
		mScale.Z = z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::SetAbsoluteScale(Vector3* value)
{
	if (mScale.X != value->X || mScale.Y != value->Y || mScale.Z != value->Z)
	{
		mScale.X = value->X;
		mScale.Y = value->Y;
		mScale.Z = value->Z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToScale(float value)
{
	if (value != 0)
	{
		mScale.X += value;
		mScale.Y += value;
		mScale.Z += value;

		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToScale(float x, float y, float z)
{
	if (x!=0 || y!=0 || z!=0)
	{
		mScale.X += x;
		mScale.Y += y;
		mScale.Z += z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToScale(Vector3* value)
{
	if (value->X !=0 || value->Y !=0 || value->Z !=0)
	{
		mScale.X += value->X;
		mScale.Y += value->Y;
		mScale.Z += value->Z;

		mChangeTransformation = TRUE;
	}
};

//rotation
void ACModel::SetAbsoluteRotation(float x, float y, float z)
{
	if (mRotation.X != x || mRotation.Y != y || mRotation.Z != z)
	{
		mRotation.X = x;
		mRotation.Y = y;
		mRotation.Z = z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::SetAbsoluteRotation(Vector3* value)
{
	if (mRotation.X != value->X || mRotation.Y != value->Y || mRotation.Z != value->Z)
	{
		mRotation.X = value->X;
		mRotation.Y = value->Y;
		mRotation.Z = value->Z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToRotation(float x, float y, float z)
{
	if (x!=0 || y!=0 || z!=0)
	{
		mRotation.X += x;
		mRotation.Y += y;
		mRotation.Z += z;

		mChangeTransformation = TRUE;
	}
};

void ACModel::AddToRotation(Vector3* value)
{
	if (value->X !=0 || value->Y !=0 || value->Z !=0)
	{
		mRotation.X += value->X;
		mRotation.Y += value->Y;
		mRotation.Z += value->Z;

		mChangeTransformation = TRUE;
	}
};

#pragma endregion