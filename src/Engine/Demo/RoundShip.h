
#ifndef __ROUNDSHIP_H
#define __ROUNDSHIP_H

#include "ACModel.h"
#include "Projectile.h"
#include <vector>
#include "Globals.h"
#include "SpaceShip.h"

class RoundShip : public SpaceShip
{
private:


public:
	RoundShip(ACRenderDevice* gDevice, ACContentManager* cManager) : SpaceShip(gDevice, cManager)
	{
		Load("cubo.amt", ET_NormalTextured);

		//seta o material
		mpSkin = cManager->CreateSkin();
		mpSkin->MaterialID =  cManager->CreateMaterial(Vector4(1,1,1,1), Vector3(1,1,1), Vector3(1,1,1),0.5f);
		mpSkin->Textures[0] = cManager->LoadTexture("TexturaTeste2.png");
		mpSkin->Textures[1] = cManager->LoadTexture("TexturaTeste2.png");

		Globals::g_pAudioDevice->LoadSound("gameover.ogg", &mSoundBufferID);
		Globals::g_pAudioDevice->AddSource(&mSoundSourceID);
		Globals::g_pAudioDevice->BindSourceToBuffer(mSoundSourceID, mSoundBufferID);
		Globals::g_pAudioDevice->SetSourceLoop(mSoundSourceID, false);

		mAcceleration = 1.0f;
		SetAbsoluteRotation(MathHelper::ToRadians(0),0,0);
		SetAbsoluteScale(150,150,150);
	};
	
	void AddAcceleration(FLOAT directionX, FLOAT directionY);
	void Shoot(FLOAT positionX, FLOAT positionY);
	void Update(FLOAT elapsedTime);
};

#endif