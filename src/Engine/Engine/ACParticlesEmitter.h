//************************
// Classe que emite as particulas
// Creator: Daniel Gadens
// Date: 21/08/2012
//************************

#ifndef __ACPARTICLEEMITTER_H
#define __ACPARTICLEEMITTER_H

#include "ACCamera.h"
#include "ACRenderDevice.h"

class ACParticleEmitter
{
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

public:
	ACParticleEmitter(ACRenderDevice* gDevice, ACContentManager* cManager);
	~ACParticleEmitter();

	void Update(float elapsedTime);
	void Draw(const ACCamera* camera);
};

#endif