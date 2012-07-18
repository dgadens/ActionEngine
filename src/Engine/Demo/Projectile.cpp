#include "Projectile.h"

void Projectile::Hit()
{

};

void Projectile::Update(FLOAT elapsedTime)
{
	if (IsActive)
	{
		Vector3 delta = Vector3(mVelocity->X, mVelocity->Y, 0);
		Vector3::Multiply(&delta, elapsedTime, &delta);
		AddToPosition(&delta);
	}

	TimeToLive -= elapsedTime;
	if(TimeToLive <= 0)
	{
		if (IsActive)
			Globals::g_pAudioDevice->PlaySound(mSoundSourceID);

		//seta para a posicao individual a posicao atual
		mpParticles->SetAbsoluteIndividualPosition(FLOAT_MAXVALUE,FLOAT_MAXVALUE,FLOAT_MAXVALUE);

		//seta aposicao da explosao
		IsActive = false;
		IsVisible = false;
		mpParticlesExplosion->SetAbsolutePosition(&mPosition);
		mpParticlesExplosion->IsActive = true;
		if (mpParticlesExplosion->IsDead)
		{
			IsDead = true;
			Globals::g_pSceneManager->RemoveParticles(mpParticlesExplosion);
			delete mpParticlesExplosion;
			mpParticlesExplosion = nullptr;
			Globals::g_pSceneManager->RemoveParticles(mpParticles);
			delete mpParticles;
			mpParticles = nullptr;
		}
	}
	else
	{
		//seta para a posicao individual a posicao atual
		mpParticles->SetAbsoluteIndividualPosition(&mPosition);
	}

}