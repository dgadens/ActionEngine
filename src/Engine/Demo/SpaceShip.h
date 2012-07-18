
#ifndef __SPACESHIP_H
#define __SPACESHIP_H

#include "ACModel.h"
#include "Projectile.h"
#include <vector>
#include "Globals.h"
#include "Cannon.h"
#include "ACParticles.h"

class SpaceShip : public ACModel
{
protected:
	PVECTOR2 mVelocity;
	FLOAT mAcceleration;
	std::vector<Projectile*> mProjectiles;
	Cannon* mCannon;
	ACParticles* mpParticles;

	//carrega alguns sons e resgata o id do buffer
	UINT mSoundBufferID;
	UINT mSoundSourceID;

public:
	SpaceShip(ACRenderDevice* gDevice, ACContentManager* cManager) : ACModel(gDevice, cManager)
	{
		mVelocity = new VECTOR2(0);
		mCannon = new Cannon(gDevice, cManager, &Vector3(0, 20, 20));
	
	/*	mpParticles = new ACParticles(gDevice, 100, "Particle_smoke.png", ACBLENDSTATE::ACBS_Additive, false);
		mpParticles->CreateTrail(3);
		mpParticles->SetAbsoluteScale(150,150,150);
		Globals::g_pSceneManager->AddParticles(mpParticles);*/
	};
	
	void AddAcceleration(FLOAT directionX, FLOAT directionY);
	virtual void Shoot(FLOAT positionX, FLOAT positionY) = 0;
	void Aim(FLOAT positionX, FLOAT positionY);
	void Update(FLOAT elapsedTime);
};

#endif