//*****************************
// Ambiente para testar tudo q é coisa
// Creator: Daniel Gadens
// Date: 27/08/2012
//****************************

#ifndef __FLOOR_H
#define __FLOOR_H

#include "EngineMain.h"

class Floor
{
	ACRenderDevice*		mpGDevice;
	ACContentManager*	mpCManager;

	ACVertexShader* mpVS;
	ACPixelShader* mpPS;

	ACVertexBuffer* mpVB;
	ACVertexPositionNormalTextured* mpVertices;

	ACSkin* mpSkin;

	Matrix mWorld;

public:
	Floor(ACRenderDevice* gDevice, ACContentManager* cManager);
	~Floor();

	void Update(float elapsedTime);
	void Draw(ACCamera* camera);
};

#endif