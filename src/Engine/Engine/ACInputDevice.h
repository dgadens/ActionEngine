//************************************
// Interface para o dispositivo de input
// Creator: Daniel Gadens
// Date: 23/07/2011
//************************************

#ifndef __ACINPUTDEVICE_H
#define __ACINPUTDEVICE_H

#include <Windows.h>
#include <string>
#include <stdio.h>
#include "EngineCommon.h"

class ACInputDevice 
{
protected:
	HWND		mActiveWnd;		   //janela ativa para manipulacao
    HINSTANCE	mhDLL;             // dll module handle
      
public:
	bool IsRunning;
    ACInputDevice(void) {};
    virtual ~ACInputDevice(void) {};
      
    virtual HRESULT Init(HWND, const RECT*)=0;
      
    //// release das coisas da api
    virtual void Release(void)=0;

    //// joystick esta disponivel?
    //virtual bool HasJoystick(std::string name)=0;

    //// atualiza os dipositivos
    //virtual HRESULT Update(void)=0;

    //// so funciona para o mouse e joystick 
    //virtual HRESULT GetPosition(InputDevType, POINT*)=0;

    //// so para joysticks (intervalo da intesidade de -1 a +1)
    //virtual HRESULT GetJoyDeflection(float*, float*)=0;

    //// pega a diferenca na posicao do mouse
    //virtual POINT GetMouseDelta(void)=0;

    //// funciona para keyboard, mouse e joystick
    //virtual bool IsPressed(InputDevType, UINT)=0;

    //// funciona para keyboard, mouse e joystick
    //virtual bool IsReleased(InputDevType, UINT)=0;
}; 

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACDI.h dentro da dll caso use joystick
typedef HRESULT (*CREATEINPUTDEVICE) (HINSTANCE hDLL, ACInputDevice **pInterface);
typedef HRESULT (*RELEASEINPUTDEVICE) (ACInputDevice **pInterface);

#endif
