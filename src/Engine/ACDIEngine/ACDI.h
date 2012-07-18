// *************************
// Implementa a interface inputdevice para o directinput
// Creator: Daniel Gadens
// Date: 23/07/2011
//*************************

#ifndef __ACDI_H
#define __ACDI_H

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800

#include "ACInputDevice.h"
#include "EngineCommon.h"
#include <dinput.h>
#include <string>

class ACJoystick;

class ACDI : public ACInputDevice 
{
private:
    LPDIRECTINPUT8  mpDI; 
    ACJoystick*     mpJoy;

public:
	ACDI(HINSTANCE hDLL);
    ~ACDI(void);
    
    // initialize the engine stuff
    HRESULT Init(HWND, const RECT*);
    
    //// Interface functions
    void    Release(void);
    //bool    HasJoystick(std::string name);
    //HRESULT Update(void);
    //
    //// query input data and states
    //HRESULT GetPosition(InputDevType idType, POINT *pPt);
    //HRESULT GetJoyDeflection(float*, float*);
    //POINT   GetMouseDelta(void);
    //bool    IsPressed(InputDevType idType, UINT nBtn);
    //bool    IsReleased(InputDevType idType, UINT nBtn);
}; 

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACDI.h dentro da dll
DLLEXPORT HRESULT CreateInputDevice(HINSTANCE hDLL, ACInputDevice **pInterface);
DLLEXPORT HRESULT ReleaseInputDevice(ACInputDevice **pInterface);

#endif
