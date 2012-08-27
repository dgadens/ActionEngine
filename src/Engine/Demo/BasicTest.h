#include "ACVertexBuffer.h"
#include "ACVertexFormat.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"
#include "ACShaders.h"

#ifndef __BASICTEST_H
#define __BASICTEST_H

class ACCamera;
struct Matrix;

class BasicTest
{
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	ACVertexShader* mpVS;
	ACPixelShader*  mpPS;
	ACVertexBuffer* mpVB;

	Matrix World;

	Vector3 mRotation;
	Vector3 mScale;
	Vector3 mPosition;

public:
	BasicTest(ACRenderDevice* gDevice, ACContentManager* cManager);
	~BasicTest();

	void Update(FLOAT elapsedTime);
	void Draw(ACCamera* camera);

	void Release();
};

#endif