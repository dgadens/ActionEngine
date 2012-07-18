#include "ACVertexFormat.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"
#include "ACShaders.h"
#include "ACUtils.h"
#include <math.h>

#ifndef __LINEPOINTTEST_H
#define __LINEPOINTTEST_H

class ACCamera;
struct Matrix;

class LinePointTest
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
	ACVertexPositionColored* bbLines;
	ACVertexPositionColored* bsLines;

	ACVertexPositionColored* points;

public:
	LinePointTest(ACRenderDevice* gDevice, ACContentManager* cManager);
	~LinePointTest();

	void Update(FLOAT elapsedTime);
	void Render(ACCamera* camera);

	void Release();
};

#endif