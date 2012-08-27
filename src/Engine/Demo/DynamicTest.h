#include "ACVertexFormat.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"
#include "ACShaders.h"

#ifndef __DYNAMICTEST_H
#define __DYNAMICTEST_H

class ACCamera;
struct Matrix;

class DynamicTest
{
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	ACVertexShader* mpVS;
	ACPixelShader*  mpPS;
	ACVertexBuffer* mpVB;

	Matrix World1;
	Matrix World2;
	Matrix World3;

	Vector3 mRotation;
	Vector3 mScale;
	Vector3 mPosition;

	ACVertexPositionColored* vertices1;
	ACVertexPositionColored* vertices2;
	ACVertexPositionColored* vertices3;
	UINT* indices;

	ACSkin* skin1;

	ACSkin* skin2;

	ACSkin* skin3;

public:
	DynamicTest(ACRenderDevice* gDevice, ACContentManager* cManager);
	~DynamicTest();

	void Update(FLOAT elapsedTime);
	void Draw(ACCamera* camera);

	void Release();
};

#endif