//************************
// Classe q controla qual o device vai ser usando apenas retorna o ponteiro para o dispositivo
// Creator: Daniel Gadens
// Date: 23/07/2011
//***********************

#ifndef __ACINPUT_H
#define __ACINPUT_H

#include "ACInputDevice.h"
#include "ACKeyboard.h"
#include "ACMouse.h"

class ACInput 
{
private:
	ACInputDevice*	mpDevice;
	ACKeyboard*     mpKeyboard;
	ACMouse*        mpMouse;
	HINSTANCE       mhInst;
	HMODULE         mhDLL;
public:
	ACInput(HINSTANCE hInst);
	~ACInput(void);
      
	HRESULT          CreateDevice(const std::string& inputLibraryName);
	void             CreateKeyboard();
	void  			 CreateMouse();

	ACInputDevice*   GetDevice(void) { return mpDevice; }
	ACKeyboard*      GetKeyboard() { return mpKeyboard; }
	ACMouse*		 GetMouse() { return mpMouse; }
	HINSTANCE        GetModule(void) { return mhDLL;    }
	void             Release(void);
};

#endif