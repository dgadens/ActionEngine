#include "ACSoundSource.h"
#include "ACContentManager.h"
#include "ACAudioDevice.h"

ACSoundSource::ACSoundSource(ACAudioDevice* aDevice, ACContentManager* cManager)
{
	mpADevice = aDevice;
	mpCManager = cManager;

	mCoupled = false;
};

ACSoundSource::~ACSoundSource()
{
	Release();
};

void ACSoundSource::Release()
{
	//se estiver acoplado a algum som ele primeiro desacopla
	if (mCoupled)
		UnbindSound();

	mpADevice->RemoveSource(SoundSourceID);
};

void ACSoundSource::Play()
{
	mpADevice->PlaySound(SoundSourceID);
};

void ACSoundSource::Pause()
{
	mpADevice->PauseSound(SoundSourceID);
};

void ACSoundSource::Stop()
{
	mpADevice->StopSound(SoundSourceID);
};

//acopla o source a um som
void ACSoundSource::BindSound(ACSound* sound)
{
	//se ele ja esta vinculado a algum som entao ele retira o vinculo do atual e coloca o novo
	if (mCoupled == true)
		UnbindSound();

	mCoupled = true;
	mpADevice->BindSourceToBuffer(SoundSourceID, sound->SoundID);
	sound->SourcesCoupled++;

	pSound = sound;
};

//desacopla o source do som
void ACSoundSource::UnbindSound()
{
	mCoupled = false;
	mpADevice->UnbindSourceBuffer(SoundSourceID, pSound->SoundID);
	pSound->SourcesCoupled--;
};

void ACSoundSource::SetLoop(const bool& value)
{
	mpADevice->SetSourceLoop(SoundSourceID, value);
};

void ACSoundSource::SetPosition(const Vector3* value)
{
	mpADevice->SetSourcePosition(SoundSourceID, value);
};

void ACSoundSource::SetDirection(const Vector3* value)
{
	mpADevice->SetSourceDirection(SoundSourceID, value);
};

void ACSoundSource::SetVelocity(const Vector3* value)
{
	mpADevice->SetSourceVelocity(SoundSourceID, value);
};

void ACSoundSource::SetPitch(const float& value)
{
	mpADevice->SetSourcePitch(SoundSourceID, value);
};

void ACSoundSource::SetGain(const float& value)
{
	mpADevice->SetSourceGain(SoundSourceID, value);
};
