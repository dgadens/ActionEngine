//************************************
// Interface para o dispositivo de audio
// Creator: Daniel Gadens
// Date: 20/08/2011
//************************************

#ifndef __ACAUDIODEVICE_H
#define __ACAUDIODEVICE_H

#include <stdio.h>
#include <map>
#include <string>
#include "EngineCommon.h"
#include "Vector3.h"
#include "ACContentManager.h"

typedef std::pair<std::string, UINT> AudioBufferPair;
typedef std::pair<UINT, UINT> AudioSourcePair;

class ACAudioDevice 
{
protected:
	HWND       mhWndMain;          // application main window
	HINSTANCE  mhDLL;              // dll module handle
	FILE      *mpLOG;              // log file

	ACContentManager* mpCManager;   //contentmanager

	std::map<std::string, UINT> mAudioBuffer; //contem a lista de buffers, usado para nao repetir a carga
	std::map<UINT, UINT> mBufferSource; //numero de sources por buffer  

public:
	bool IsRunning;

    ACAudioDevice() {};
    virtual ~ACAudioDevice(void) {};

	//seta o contentmanager 
	void SetContentManager(ACContentManager* cManager)
	{
		mpCManager = cManager;
	};
      
	virtual HRESULT Init(HWND, bool)=0;

	virtual void Release(void)=0;
	
	virtual void Log(char* message, ...) =0;

	// Load OGG from file
	// Return sound (buffer) pointer
	virtual HRESULT LoadSound(const std::string& path, ACSound** ppOutSoundPtr)=0;
	// Remove sound (buffer) of memory 
	virtual void ReleaseSound(const ACSound* pSound)=0;

	// Play a specific sound
	virtual void PlaySound(UINT)=0;

	// Stop a specific sound
	virtual void StopSound(UINT)=0;

	// Pause a specific sound
	virtual void PauseSound(UINT)=0;

	// Set listener parameters
	virtual void SetListener(const Vector3& position, const Vector3& direction, const Vector3& up, const Vector3& velocity)=0;

	// Add new source
	// Return source pointer
	virtual void AddSource(UINT*)=0;
	// Remove source with id parameter
	virtual void RemoveSource(const UINT&)=0; 
	// Bind source on the buffer
	virtual void BindSourceToBuffer(const UINT&, const UINT&)=0;
	// Unbind source of buffer (remove all sources before remove buffer)
	virtual void UnbindSourceBuffer(const UINT&, const UINT&)=0;

	virtual void SetSourceLoop(const UINT&, const bool&)=0;
	virtual void SetSourcePosition(const UINT&, const Vector3*)=0;
	virtual void SetSourceDirection(const UINT&, const Vector3*)=0;
	virtual void SetSourceVelocity(const UINT&, const Vector3*)=0;
	virtual void SetSourcePitch(const UINT&, const float&)=0;
	virtual void SetSourceGain(const UINT&, const float&)=0;
}; 

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACDI.h dentro da dll caso use joystick
typedef HRESULT (*CREATEAUDIODEVICE) (HINSTANCE hDLL, ACAudioDevice **pInterface);
typedef HRESULT (*RELEASEAUDIODEVICE) (ACAudioDevice **pInterface);

#endif