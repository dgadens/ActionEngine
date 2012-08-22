//*********************************
// Cubo do poder, eu uso isso pra testar a carga dos objetos 3d
// creator: Daniel Gadens
// date: 23/04/2012
//********************************


#ifndef __POWERCUBE_H
#define __POWERCUBE_H

#include "ACRenderDevice.h"
#include "ACContentManager.h"
#include "ACModel.h"
#include "ACSkin.h"
#include "EngineMath.h"
#include "ACCamera.h"
#include "Globals.h"
#include "ACParticles.h"
#include "ACSound.h"

#include "GEnums.h"

#include <lua.hpp>

class GPowerCube
{
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	//model 3d
	ACModel* mpModels[10000];

	//sound source
	ACSound*		mpSoundBuffer;
	ACSoundSource*	mpSoundSource;

	float mCurrentRotation;

public:
	GPowerCube(ACRenderDevice* gDevice, ACContentManager* cManager);
	~GPowerCube();

	ACModel* GetModel();

	void SetSkin(ACSkin* skin);

	void Update(float elapsedTime);

	void Render(ACCamera* camera);


	//special effects
	void Explode();
	void Broke();
};

#endif