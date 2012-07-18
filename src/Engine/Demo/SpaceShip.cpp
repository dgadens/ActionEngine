#include "SpaceShip.h"

void SpaceShip::AddAcceleration(FLOAT directionX, FLOAT directionY)
{
	Vector2 delta = Vector2(directionX * mAcceleration, directionY * mAcceleration);
	Vector2::Add(mVelocity, &delta, mVelocity);
}

void SpaceShip::Shoot(FLOAT positionX, FLOAT positionY)
{
	
};

void SpaceShip::Aim(FLOAT positionX, FLOAT positionY)
{
	mCannon->Aim(positionX, positionY);
}

void SpaceShip::Update(FLOAT elapsedTime)
{
	//Atualiza a posiçao da nave
	if (IsActive)
	{
		Vector3 delta = Vector3(mVelocity->X, mVelocity->Y, 0);
		Vector3::Multiply(&delta, elapsedTime, &delta);
		AddToPosition(&delta);

		Vector3* weaponPosition = &Vector3(0);
		Vector3::Divide(mCannon->RelativePosition, &mScale, weaponPosition);
		Vector3::Transform(weaponPosition, &World, weaponPosition);
		mCannon->SetAbsolutePosition(weaponPosition);

		//mpParticles->SetAbsoluteIndividualPosition(weaponPosition);

		for (int i = mProjectiles.size() - 1; i >=0; i--)
		{
			mProjectiles[i]->Update(elapsedTime);
			if (mProjectiles[i]->IsDead)
			{
				Projectile* p = mProjectiles[i];
				Globals::g_pSceneManager->RemoveModel(p); //REMOVE DA LISTA
				mProjectiles.erase(mProjectiles.begin() + i);
				delete p;
			}
		}
	}
}

