#include "ACParticles.h"
#include "ACRenderDevice.h"

ACParticles::ACParticles(ACRenderDevice* gDevice, ACContentManager* cManager, 
						UINT numberOfParticles, std::string textureName, ACBLENDSTATE blendState, 
						BOOL enableDead)
{
	GlobalID = ACConfigurations::Instance()->GlobalID++;
	ppParticles = nullptr;

	mpGDevice = gDevice;
	mpCManager = cManager;

	NumberOfParticles = numberOfParticles;

	pTexture = mpCManager->LoadTexture(textureName);

	mChangeTransformation = FALSE;
	IsActive = FALSE; //SE FOR ATIVO ENTAO ELE EXECUTA O UPDATE
	IsVisible = TRUE; //SE FOR VISIBLE ENTAO ELE EXIBE
	
	EnableDead = enableDead;
	IsDead = FALSE;

	mTimeAccum = 0;
	
	ppParticles = new ACVertexPositionTexturedExtraInfo*[NumberOfParticles];
	mpCicles = new FLOAT[NumberOfParticles];
	ZeroMemory(mpCicles,sizeof (float) * NumberOfParticles);
	
	mBlendState = blendState;

	mScale.X = 1;
	mScale.Y = 1;
	mScale.Z = 1;

	pAPIBuffer = nullptr;
};

ACParticles::~ACParticles() 
{
	Release();
};

void ACParticles::Release()
{
	//remove a textura utilizada
	mpCManager->RemoveTexture(pTexture);
	pTexture = nullptr;

	//remove todos os objetos particleStruct do array de particulas
	for (int i=0; i < NumberOfParticles; i++)
		delete ppParticles[i];
	delete [] ppParticles;

	//remove os ciclos as particulas, cada 1 tem o seu ciclo para fazer o reset
	delete [] mpCicles;
};

void ACParticles::SetTexture(std::string textureName)
{
	//remove a atual e carrega a nova
	mpCManager->RemoveTexture(textureName);
	pTexture = mpCManager->LoadTexture(textureName);
};

void ACParticles::CreateExplosion(float timeLife, float velocity, float maxSize)
{
	ParticleType = ACPARTICLETYPES::ACPT_Explosion;

	mTimeLife = timeLife;
	mCurrentTime = 0;

	ppParticles = new ACVertexPositionTexturedExtraInfo*[NumberOfParticles];
	GenericValues.X = velocity; //Velocidade
	GenericValues.Y = maxSize; //Tamanho maximo

	for ( int i = 0; i < NumberOfParticles; i++)
	{
		Vector3 startingPos(0,0,0);

		float r1 = ACRandom::NextFloat() - 0.5f;
		float r2 = ACRandom::NextFloat() - 0.5f;
		float r3 = ACRandom::NextFloat() - 0.5f;

		Vector3 moveDirection(r1,r2,r3);
		Vector3::Normalize(&moveDirection, &moveDirection);

		float r4 = ACRandom::NextFloat();
		r4 = r4 /4.0f * 3.0f + 0.25f;


		ACVertexPositionTexturedExtraInfo* p = new ACVertexPositionTexturedExtraInfo();
		////v0		
		//Vector3::Copy(&startingPos, &p->Vertices[0]);
		//Vector4::Copy(&Vector4(0, 0, 0, timeLife), &p->TexCoords[0]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[0]);

		////v1		
		//Vector3::Copy(&startingPos, &p->Vertices[1]);
		//Vector4::Copy(&Vector4(0, 1, 0, timeLife), &p->TexCoords[1]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[1]);

		////v2		
		//Vector3::Copy(&startingPos, &p->Vertices[2]);
		//Vector4::Copy(&Vector4(1, 0, 0, timeLife), &p->TexCoords[2]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[2]);

		////v3		
		//Vector3::Copy(&startingPos, &p->Vertices[3]);
		//Vector4::Copy(&Vector4(1, 0, 0, timeLife), &p->TexCoords[3]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[3]);

		////v4		
		//Vector3::Copy(&startingPos, &p->Vertices[4]);
		//Vector4::Copy(&Vector4(0, 1, 0, timeLife), &p->TexCoords[4]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[4]);

		////v5		
		//Vector3::Copy(&startingPos, &p->Vertices[5]);
		//Vector4::Copy(&Vector4(1, 1, 0, timeLife), &p->TexCoords[5]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[5]);

		ppParticles[i] = p;
	}

	//mpGDevice->AddParticles(this);
};

void ACParticles::CreateSmoke(float timeLife, float velocity, float maxSize)
{
	ParticleType = ACPARTICLETYPES::ACPT_Smoke;

	float randomTime = 0;
	float rangeTime = 500;

	mTotalTimeLife = timeLife;
	mTimeLife = mTotalTimeLife;// + rangeTime;
	mCurrentTime = 0;

	GenericValues.X = velocity; //Velocidade
	GenericValues.Y = maxSize; //Tamanho maximo

	for ( int i = 0; i < NumberOfParticles; i++)
	{
		randomTime = ACRandom::NextFloat(mTimeLife / 4, mTimeLife);
		Vector3 startingPos(5,0,0);

		float r1 = ACRandom::NextFloat() - 0.5f;
		float r2 = ACRandom::NextFloat() - 0.5f;
		float r3 = ACRandom::NextFloat() - 0.5f;

		Vector3 moveDirection(r1,r2,r3);
		Vector3::Normalize(&moveDirection, &moveDirection);

		float r4 = ACRandom::NextFloat();
		r4 = r4 /4.0f * 3.0f + 0.25f;

		ACVertexPositionTexturedExtraInfo* p = new ACVertexPositionTexturedExtraInfo();
		////v0		
		//Vector3::Copy(&startingPos, &p->Vertices[0]);
		//Vector4::Copy(&Vector4(0, 0, randomTime, timeLife), &p->TexCoords[0]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[0]);

		////v1		
		//Vector3::Copy(&startingPos, &p->Vertices[1]);
		//Vector4::Copy(&Vector4(0, 1, randomTime, timeLife), &p->TexCoords[1]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[1]);

		////v2		
		//Vector3::Copy(&startingPos, &p->Vertices[2]);
		//Vector4::Copy(&Vector4(1, 0, randomTime, timeLife), &p->TexCoords[2]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[2]);

		////v3		
		//Vector3::Copy(&startingPos, &p->Vertices[3]);
		//Vector4::Copy(&Vector4(1, 0, randomTime, timeLife), &p->TexCoords[3]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[3]);

		////v4		
		//Vector3::Copy(&startingPos, &p->Vertices[4]);
		//Vector4::Copy(&Vector4(0, 1, randomTime, timeLife), &p->TexCoords[4]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[4]);

		////v5		
		//Vector3::Copy(&startingPos, &p->Vertices[5]);
		//Vector4::Copy(&Vector4(1, 1, randomTime, timeLife), &p->TexCoords[5]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[5]);

		ppParticles[i] = p;
	}

	//mpGDevice->AddParticles(this);
};

void ACParticles::CreateTrail(float timeLife)
{
	ParticleType = ACPARTICLETYPES::ACPT_Trail;

	float timeToEmit = timeLife / NumberOfParticles;
	float emitTime = 0;
	IsActive = TRUE;

	mTotalTimeLife = timeLife;
	mTimeLife = mTotalTimeLife;// + rangeTime;
	mCurrentTime = 0;

	for ( int i = 0; i < NumberOfParticles; i++)
	{
		emitTime = (i + 1) * timeToEmit;

		Vector3 startingPos(FLOAT_MAXVALUE, FLOAT_MAXVALUE, FLOAT_MAXVALUE);
		Vector3 moveDirection(0,0,0);

		float r4 = ACRandom::NextFloat();
		r4 = r4 /4.0f * 3.0f + 0.25f;

		ACVertexPositionTexturedExtraInfo* p = new ACVertexPositionTexturedExtraInfo();
		////v0		
		//Vector3::Copy(&startingPos, &p->Vertices[0]);
		//Vector4::Copy(&Vector4(0, 0, emitTime, timeLife), &p->TexCoords[0]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[0]);

		////v1		
		//Vector3::Copy(&startingPos, &p->Vertices[1]);
		//Vector4::Copy(&Vector4(0, 1, emitTime, timeLife), &p->TexCoords[1]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[1]);

		////v2		
		//Vector3::Copy(&startingPos, &p->Vertices[2]);
		//Vector4::Copy(&Vector4(1, 0, emitTime, timeLife), &p->TexCoords[2]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[2]);

		////v3		
		//Vector3::Copy(&startingPos, &p->Vertices[3]);
		//Vector4::Copy(&Vector4(1, 0, emitTime, timeLife), &p->TexCoords[3]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[3]);

		////v4		
		//Vector3::Copy(&startingPos, &p->Vertices[4]);
		//Vector4::Copy(&Vector4(0, 1, emitTime, timeLife), &p->TexCoords[4]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[4]);

		////v5		
		//Vector3::Copy(&startingPos, &p->Vertices[5]);
		//Vector4::Copy(&Vector4(1, 1, emitTime, timeLife), &p->TexCoords[5]);
		//Vector4::Copy(&Vector4(&moveDirection, r4), &p->AditionaInfo[5]);

		ppParticles[i] = p;
	}

	//mpGDevice->AddParticles(this);
};

//pega as particulas mortas e seta a posicao
void ACParticles::ResetPosition(Vector3& position)
{
	for ( int i = 0; i < NumberOfParticles; i++)
	{
		ACVertexPositionTexturedExtraInfo* p = ppParticles[i];

		//if (mCurrentTime > p->TexCoords->Z)
		//{
		//	float age = fmod(mCurrentTime - p->TexCoords->Z, p->TexCoords->W);	 //faz o resto do tempo com o maxlife
		//	float relAge = age/p->TexCoords->W; //divide * resto da divisao pelo
		//	if ((relAge < 0.5f && mpCicles[i] > 0.5f) ||
		//	    (mCurrentTime >= p->TexCoords->Z && mpCicles[i] == 0)) // se ele reseto a posicao entao ele muda aposicao dos vertices
		//	{
		//		//v0 a 5		
		//		p->Vertices[0] = position;
		//		p->Vertices[1] = position;
		//		p->Vertices[2] = position;
		//		p->Vertices[3] = position;
		//		p->Vertices[4] = position;
		//		p->Vertices[5] = position;
		//	}

		//	mpCicles[i] = relAge;
		//}
	}
};

void ACParticles::Resurrection()
{
	mCurrentTime = 0;
	IsDead = false;
};

void ACParticles::Update(float elapsedTime)
{
	if (IsActive)
	{
		mCurrentTime += elapsedTime;
		if (EnableDead)
		{
			if (mCurrentTime >= mTimeLife)
			{
				IsDead = true;
				return;
			}
		}

		if (ParticleType == ACPARTICLETYPES::ACPT_Trail)
		{
			ResetPosition(mIndividualPosition);
		}

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

			mChangeTransformation = FALSE;
		}
	}
};

void ACParticles::Render(ACCamera* camera, float totalMilliseconds)
{
	if (IsActive && IsVisible && !IsDead)
	{
		//seta o tipo de blend da particula
		mpGDevice->SetBlendState(mBlendState);
		//mpGDevice->RenderParticles(camera, this, mCurrentTime,  ParticleType);
	}
};

//individual position	
void ACParticles::SetAbsoluteIndividualPosition(float x, float y, float z)
{
	mIndividualPosition.X = x;
	mIndividualPosition.Y = y;
	mIndividualPosition.Z = z;
};

void ACParticles::SetAbsoluteIndividualPosition(Vector3* value)
{
	mIndividualPosition.X = value->X;
	mIndividualPosition.Y = value->Y;
	mIndividualPosition.Z = value->Z;
};

//position
void ACParticles::SetAbsolutePosition(float x, float y, float z)
{
	if (mPosition.X != x || mPosition.Y != y || mPosition.Z != z)
	{
		mPosition.X = x;
		mPosition.Y = y;
		mPosition.Z = z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::SetAbsolutePosition(Vector3* value)
{
	if (mPosition.X != value->X || mPosition.Y != value->Y || mPosition.Z != value->Z)
	{
		mPosition.X = value->X;
		mPosition.Y = value->Y;
		mPosition.Z = value->Z;
		
		mChangeTransformation = TRUE;
	}
};

void ACParticles::AddToPosition(float x, float y, float z)
{
	if (x!=0 || y!=0 || z!=0)
	{
		mPosition.X += x;
		mPosition.Y += y;
		mPosition.Z += z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::AddToPosition(Vector3* value)
{
	if (value->X !=0 || value->Y !=0 || value->Z !=0)
	{
		mPosition.X += value->X;
		mPosition.Y += value->Y;
		mPosition.Z += value->Z;

		mChangeTransformation = TRUE;
	}
};

const Vector3& ACParticles::GetPosition()
{
	return mPosition;
};

//scale
void ACParticles::SetAbsoluteScale(float x, float y, float z)
{
	if (mScale.X != x || mScale.Y != y || mScale.Z != z)
	{
		mScale.X = x;
		mScale.Y = y;
		mScale.Z = z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::SetAbsoluteScale(Vector3* value)
{
	if (mScale.X != value->X || mScale.Y != value->Y || mScale.Z != value->Z)
	{
		mScale.X = value->X;
		mScale.Y = value->Y;
		mScale.Z = value->Z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::AddToScale(float x, float y, float z)
{
	if (x!=0 || y!=0 || z!=0)
	{
		mScale.X += x;
		mScale.Y += y;
		mScale.Z += z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::AddToScale(Vector3* value)
{
	if (value->X !=0 || value->Y !=0 || value->Z !=0)
	{
		mScale.X += value->X;
		mScale.Y += value->Y;
		mScale.Z += value->Z;

		mChangeTransformation = TRUE;
	}
};

const Vector3& ACParticles::GetScale()
{
	return mScale;
};

//rotation
void ACParticles::SetAbsoluteRotation(float x, float y, float z)
{
	if (mRotation.X != x || mRotation.Y != y || mRotation.Z != z)
	{
		mRotation.X = x;
		mRotation.Y = y;
		mRotation.Z = z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::SetAbsoluteRotation(Vector3* value)
{
	if (mRotation.X != value->X || mRotation.Y != value->Y || mRotation.Z != value->Z)
	{
		mRotation.X = value->X;
		mRotation.Y = value->Y;
		mRotation.Z = value->Z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::AddToRotation(float x, float y, float z)
{
	if (x!=0 || y!=0 || z!=0)
	{
		mRotation.X += x;
		mRotation.Y += y;
		mRotation.Z += z;

		mChangeTransformation = TRUE;
	}
};

void ACParticles::AddToRotation(Vector3* value)
{
	if (value->X !=0 || value->Y !=0 || value->Z !=0)
	{
		mRotation.X += value->X;
		mRotation.Y += value->Y;
		mRotation.Z += value->Z;

		mChangeTransformation = TRUE;
	}
};

const Vector3& ACParticles::GetRotation()
{
	return mRotation;
};