
#ifndef __CANNON_H
#define __CANNON_H

#include "ACModel.h"
#include "Projectile.h"
#include "Globals.h"
#include <math.h>

class Cannon : public ACModel
{
private:


public:
	PVECTOR3 RelativePosition;

	Cannon(ACRenderDevice* gDevice, ACContentManager* cManager, PVECTOR3 relativePosition) : ACModel(gDevice, cManager)
	{
		Load("nave2.amt", ET_NormalTextured);
		SetAbsoluteScale(10,10,10);
		Globals::g_pSceneManager->AddModel(this);
		RelativePosition = new VECTOR3(relativePosition->X, relativePosition->Y, relativePosition->Z);
	};
	
	Projectile* Shoot(FLOAT positionX, FLOAT positionY);
	void Aim(FLOAT positionX, FLOAT positionY);
	void Update();
};

#endif