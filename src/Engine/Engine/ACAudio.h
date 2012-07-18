//************************
// Classe q controla qual o device vai ser usando 
// Creator: Daniel Gadens
// Date: 20/08/2011
//***********************

#ifndef __ACAUDIO_H
#define __ACAUDIO_H

#include "ACAudioDevice.h"

class ACAudio
{
private:
	ACAudioDevice* mpDevice;
	HINSTANCE      mhInst;
	HMODULE        mhDLL;
public:
	ACAudio(HINSTANCE hInst);
	~ACAudio(void);
      
	HRESULT          CreateDevice(const std::string &audioLibraryName);
	ACAudioDevice*	 GetDevice(void) { return mpDevice; }
	HINSTANCE        GetModule(void) { return mhDLL;    }
	void             Release(void);
};

#endif