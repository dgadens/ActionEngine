//**********************************************
// Classe com os ids dos sources provenientes do device
// creator: Daniel Gadens
// date: 25/04/2012
//*********************************************

#ifndef __ACSOUNDSOURCE_H
#define __ACSOUNDSOURCE_H

#include "ActionTypeDef.h"
#include <string>
#include "ACSound.h"
#include "EngineMath.h"

class ACContentManager;
class ACAudioDevice;

class ACSoundSource
{
	ACAudioDevice* mpADevice;
	ACContentManager* mpCManager;

	bool mCoupled; //verifica se esta acoplado a algum sound ou nao se estiver ele desacopla no destrutor

public:
	UINT			SoundSourceID;
	ACSound*        pSound;

	ACSoundSource(ACAudioDevice* aDevice, ACContentManager* cManager);
	~ACSoundSource();

	void Play();
	void Pause();
	void Stop();

	void BindSound(ACSound* sound);
	void UnbindSound();

	void ACSoundSource::SetLoop(const bool&);
	void ACSoundSource::SetPosition(const Vector3*);
	void ACSoundSource::SetDirection(const Vector3*);
	void ACSoundSource::SetVelocity(const Vector3*);
	void ACSoundSource::SetPitch(const float&);
	void ACSoundSource::SetGain(const float&);

};

#endif