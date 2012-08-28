//***************************
// classe que cria Particulas
// Creator: Daniel Gadens
// Date: 30/07/2011
//**************************

#ifndef __ACPARTICLES_H
#define __ACPARTICLES_H

#include "Vector3.h"
#include "ACTexture.h"
#include "ACRandom.h"
#include "ACCamera.h"
#include "ACParticleTypes.h"
#include "ACRenderDeviceEnums.h"
#include "ACContentManager.h"
#include "ACConfigurations.h"

class ACRenderDevice;

class ACParticles
{
private:
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	Vector3 mPosition;
	Vector3 mIndividualPosition; //usado para setar a posicao individual de cada particula, é usando no reset
	Vector3 mScale;
	Vector3 mRotation;

	float* mpCicles; //armazena quantos ciclos de vida ja passou a particula desde sua criacao, usado para resetar a posicao

	float mTimeAccum;
	float mTotalTimeLife;
	float mCurrentTime; //tempo atual
	float mTimeLife; //tempo de vida do emissor

	ACBLENDSTATE mBlendState;

	BOOL mChangeTransformation;

public:
	UINT GlobalID;
	ACVertexPositionTexturedExtraInfo** ppParticles;
	UINT NumberOfParticles;
	ACTexture* pTexture; //ID da textura no contentmanager
	Matrix World;
	Vector4 GenericValues; //mantem valores genericos para cada tipo de particula, é usando dentro do shader como globals para diversas coisas

	void* pAPIBuffer; //armazena o ponteiro para o buffer na api (usando para nao ficar procurando nos maps)
	
	BOOL IsActive; //executa o update ou nao
	BOOL IsVisible; //mostra ou nao

	BOOL EnableDead; //a particula pode morrer
	BOOL IsDead; //deleta o cara 

	ACPARTICLETYPES ParticleType;

	ACParticles(ACRenderDevice* gDevice, ACContentManager* cManager, UINT numberOfParticles, std::string textureName, ACBLENDSTATE blendState, BOOL enableDead = false);
	~ACParticles();

	void Release();

	void SetTexture(std::string textureName);
	void CreateExplosion(float timeLife, float velocity, float maxSize);
	void CreateSmoke(float timeLife, float velocity, float maxSize);
	void CreateTrail(float timeLife);

	//usando quando se quer manter a posicao de cada particula ate o final cada uma tem uma posicao diferente, nem todas saem do mesmo emissor
	//pega as particulas mortas e seta a posicao passada no parametro
	void ResetPosition(Vector3& position);
	//ressucita o emissor
	void Resurrection();

	void Update(float elapsedTime);
	void Render(ACCamera* camera, float totalMilliseconds);

	//individual position
	void SetAbsoluteIndividualPosition(float x, float y, float z);
	void SetAbsoluteIndividualPosition(Vector3* value);

	//position
	void SetAbsolutePosition(float x, float y, float z);
	void SetAbsolutePosition(Vector3* value);
	void AddToPosition(float x, float y, float z);
	void AddToPosition(Vector3* value);
	const Vector3& GetPosition();

	//scale
	void SetAbsoluteScale(float x, float y, float z);
	void SetAbsoluteScale(Vector3* value);
	void AddToScale(float x, float y, float z);
	void AddToScale(Vector3* value);
	const Vector3& GetScale();

	//rotation
	void SetAbsoluteRotation(float x, float y, float z);
	void SetAbsoluteRotation(Vector3* value);
	void AddToRotation(float x, float y, float z);
	void AddToRotation(Vector3* value);
	const Vector3& GetRotation();

};

#endif