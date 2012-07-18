
#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include "ACModel.h"
#include "Globals.h"

class Projectile : public ACModel
{
private:
	PVECTOR2 mVelocity;
	ACParticles* mpParticles;
	ACParticles* mpParticlesExplosion;

	UINT mSoundBufferID;
	UINT mSoundSourceID;

public:
	FLOAT TimeToLive;

	Projectile(ACRenderDevice* gDevice, ACContentManager* cManager, VECTOR2* velocity) : ACModel(gDevice, cManager)
	{
		SetAbsoluteScale(5,5,5);
		mVelocity = new Vector2(velocity->X, velocity->Y);
		TimeToLive = 2.0f;

		mpParticles = new ACParticles(gDevice, cManager, 10, "Particle_explosion.png", ACBLENDSTATE::ACBS_Additive, false);
		mpParticles->CreateTrail(1);
		mpParticles->SetAbsoluteScale(70,70,70);
		Globals::g_pSceneManager->AddParticles(mpParticles);

		mpParticlesExplosion = new ACParticles(gDevice, cManager, 10, "Particle_fire.png", ACBLENDSTATE::ACBS_Additive, true);
		mpParticlesExplosion->CreateExplosion(1.0f, 100, 200);
		mpParticlesExplosion->SetAbsoluteScale(1,1,1);
		Globals::g_pSceneManager->AddParticles(mpParticlesExplosion);
		mpParticlesExplosion->IsActive = false;

		Globals::g_pAudioDevice->LoadSound("bomb.ogg", &mSoundBufferID);
		Globals::g_pAudioDevice->AddSource(&mSoundSourceID);
		Globals::g_pAudioDevice->BindSourceToBuffer(mSoundSourceID, mSoundBufferID);
		Globals::g_pAudioDevice->SetSourceLoop(mSoundSourceID, false);
	};

	~Projectile()
	{
		delete mVelocity;
	}
	
	void Hit();
	void Update(FLOAT elapsedTime);
};

#endif