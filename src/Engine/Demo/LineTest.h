#include "ACVertexFormat.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"
#include "ACShaders.h"
#include <math.h>

#ifndef __LINETEST_H
#define __LINETEST_H

class ACCamera;
struct Matrix;

class LineTest
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

	ACVertexPositionColored* lines;

public:
	LineTest(ACRenderDevice* gDevice, ACContentManager* cManager);
	~LineTest();

	void Update(FLOAT elapsedTime);
	void Render(ACCamera* camera);

	void Release();
};

#endif