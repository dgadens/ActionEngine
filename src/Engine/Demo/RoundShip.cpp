#include "RoundShip.h"

void RoundShip::Shoot(FLOAT positionX, FLOAT positionY)
{
	Globals::g_pAudioDevice->SetSourcePosition(mSoundSourceID, &mPosition);
	Globals::g_pAudioDevice->PlaySound(mSoundSourceID);

	Projectile* projectile = mCannon->Shoot(positionX, positionY);
	mProjectiles.push_back(projectile);
};