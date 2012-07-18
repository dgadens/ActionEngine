#include "Cannon.h"

 Projectile* Cannon::Shoot(FLOAT positionX, FLOAT positionY)
{
	//Lança com velocidade 100
	VECTOR2 velocity = VECTOR2(positionX - mPosition.X, positionY - mPosition.Y);
	VECTOR2::Multiply(&velocity, 300 / VECTOR2::Length(&velocity), &velocity);

	Projectile* projectile = new Projectile(mpGDevice, mpCManager, &velocity);
	projectile->Load("Cubo.amt", ET_NormalTextured);
	Globals::g_pSceneManager->AddModel(projectile);

	projectile->SetAbsolutePosition(this->mPosition.X, this->mPosition.Y, this->mPosition.Z);
	return projectile;
}

 void Cannon::Aim(FLOAT positionX, FLOAT positionY)
{
	VECTOR2 direction = VECTOR2(positionX - mPosition.X, positionY - mPosition.Y);
	VECTOR2::Normalize(&direction, &direction);

	SetAbsoluteRotation(0, 0, direction.X > 0 ? -acos(direction.Y) : acos(direction.Y));
}