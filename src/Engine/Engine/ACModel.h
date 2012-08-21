//***************************************************
// classe que mantem os atributos dos modelos 3d (usa a classe mesh para carregar os vertices)
// creator: Daniel Gadens
// date: 19/05/2011
//***************************************************

#ifndef __ACMODEL_H
#define __ACMODEL_H

#include <string.h>
#include "ActionTypeDef.h"
#include "EngineMath.h"
#include "ACCamera.h"
#include "ACModelDefinition.h"
#include "ACLine.h"
#include "ACSkin.h"
#include "ACContentManager.h"

class ACRenderDevice;

class ACModel
{
protected:
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	Vector3 mPosition;
	Vector3 mScale;
	Vector3 mRotation;

	ACVertexShader*    mpVS;
	ACPixelShader*     mpPS;
	ACGeometryShader*  mpGS;

private:

	BOOL mChangeTransformation;

public:
	ACModelDefinition* pModelDefinition;
	Matrix World;
	BoundingBox BB;
	OrientedBoundingBox OBB;
	EffectType EffectName;

	BOOL IsActive; //executa o update
	BOOL IsVisible; //executa o render
	BOOL IsDead; //esta morto deve ser excluido da lista e da memoria (responsabilidade da aplicao cliente)

	ACModel(ACRenderDevice* gDevice, ACContentManager* cManager);
	virtual ~ACModel();

	//Seta o skin passado como parametro para dentro do model é feita uma copia dos dados
	//mas o ponteiro do parametro continua existindo (remover)
	void SetSkin(ACSkin* skin);
	//Retorna o ponteiro para o skin interno ao modelo
	const ACSkin const* GetSkin();

	//mesh
	void Load(const std::string& path);

	//shaders (o model carrega um VS e um PS padrao estes metodos sao usado apenas se o usuario quiser modificar o shader)
	void SetVertexShader(const ACVertexShader* vs);
	void SetPixelShader(const ACPixelShader* ps);
	void SetGeometryShader(const ACGeometryShader* gs);

	//configurations
	//void SetRenderBB(bool value);
	//void SetRenderNormals(bool value);
	//void SetRenderBones(bool value);

	//update
	void Update(FLOAT elapsedTime);
	void Render(ACCamera* camera);

	//position
	void SetAbsolutePosition(float x, float y, float z);
	void SetAbsolutePosition(Vector3* value);
	void AddToPosition(float x, float y, float z);
	void AddToPosition(Vector3* value);

	//scale
	void SetAbsoluteScale(float value);
	void SetAbsoluteScale(float x, float y, float z);
	void SetAbsoluteScale(Vector3* value);
	void AddToScale(float value);
	void AddToScale(float x, float y, float z);
	void AddToScale(Vector3* value);

	//rotation
	void SetAbsoluteRotation(float x, float y, float z);
	void SetAbsoluteRotation(Vector3* value);
	void AddToRotation(float x, float y, float z);
	void AddToRotation(Vector3* value);

	void Release();

};

#endif