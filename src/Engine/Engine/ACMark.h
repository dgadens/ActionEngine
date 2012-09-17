//*********************************
// Utilizado para mostrar alguma coisa na engine
// creator: Daniel Gadens
// date: 16/09/2012
//********************************

#ifndef __ACMARK_H
#define __ACMARK_H

#include "ACVertexBuffer.h"
#include "ACVertexFormat.h"
#include "ACShaders.h"
#include "ACCamera.h"

class ACContentManager;
class ACRenderDevice;

class ACMark
{
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	//model 3d
	ACVertexPosition* mpVertices;

	ACVertexShader* mpVS;
	ACPixelShader* mpPS;

	ACVertexBuffer* mpVB;

public:
	ACMark(ACRenderDevice* gDevice, ACContentManager* cManager);
	~ACMark();

	void Render(ACCamera* camera, Matrix& worldMatrix);
};

#endif