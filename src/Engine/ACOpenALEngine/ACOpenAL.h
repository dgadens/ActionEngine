//************************************
// Interface para o dispositivo de audio OpenAL 1.1
// Creator: Daniel Gadens
// Date: 30/08/2011
//************************************

#ifndef __ACOPENAL_H
#define __ACOPENAL_H

#include "ACAudioDevice.h"
#include "Vector3.h"
#include "al.h"
#include "alc.h"
#include <vector>
#include <windows.h>
#include "OGGLoader.h"

#define NUM_BUFFERS 1 //numero de buffers é usando para fazer o stream
#define BUFFER_SIZE 4096  //tamanho do buffer para o stream

class ACOpenAL : public ACAudioDevice
{
private:
	ALCdevice*	mpDevice;
	ALCcontext* mpContext;

public:
	ALboolean EnableEAX;

    ACOpenAL(HINSTANCE hDLL);
    ~ACOpenAL(void);
      
	HRESULT Init(HWND, bool);

	void Release();

	void Log(char* message, ...);
  
	HRESULT LoadSound(const std::string &path, ACSound** ppOutSoundPtr);
	void ReleaseSound(const ACSound* pSound);

	void PlaySound(UINT);
	void StopSound(UINT);
	void PauseSound(UINT);

	void SetListener(const Vector3& position, const Vector3& direction, const Vector3& up, const Vector3& velocity);

	void AddSource(UINT*);
	void RemoveSource(const UINT&);
	void BindSourceToBuffer(const UINT&, const UINT&);
	void UnbindSourceBuffer(const UINT&, const UINT&);
	void SetSourceLoop(const UINT&, const bool&);
	void SetSourcePosition(const UINT&, const Vector3*);
	void SetSourceDirection(const UINT&, const Vector3*);
	void SetSourceVelocity(const UINT&, const Vector3*);
	void SetSourcePitch(const UINT&, const float&);
	void SetSourceGain(const UINT&, const float&);
}; 

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACDI.h dentro da dll caso use joystick
DLLEXPORT HRESULT CreateAudioDevice(HINSTANCE hDLL, ACAudioDevice **pInterface);
DLLEXPORT HRESULT ReleaseAudioDevice(ACAudioDevice **pInterface);

#endif