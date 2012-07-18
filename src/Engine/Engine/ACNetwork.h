//************************
// Classe q controla qual o device vai ser usando 
// Creator: Daniel Gadens
// Date: 23/07/2011
//***********************

#ifndef __ACNETWORK_H
#define __ACNETWORK_H

#include "ACNetworkDevice.h"

class ACNetwork 
{
private:
	ACNetworkDevice* mpDevice;
	HINSTANCE        mhInst;
	HMODULE          mhDLL;
public:
	ACNetwork(HINSTANCE hInst);
	~ACNetwork(void);
      
	HRESULT          CreateDevice(const std::string &mplayerLibraryName);
	ACNetworkDevice* GetDevice(void) { return mpDevice; }
	HINSTANCE        GetModule(void) { return mhDLL;    }
	void             Release(void);
};

#endif