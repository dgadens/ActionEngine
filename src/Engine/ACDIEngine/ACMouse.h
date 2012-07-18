#pragma once
#ifndef __ACMOUSE_H
#define __ACMOUSE_H

#include "EngineCommon.h"
#include "ACDIDevice.h"
#include <dinput.h>

#define BUFFER_SIZE 16

class ACMouse : public ACDIDevice 
{
private:
	HANDLE     mhEvent;
	RECT       mrcCage;
	bool       mbPressed[3];
	bool       mbReleased[3];
	POINT      mDelta;

public:
    ACMouse(LPDIRECTINPUT8, HWND);
    ~ACMouse(void);

    HRESULT Init();
    HRESULT Update();

    void  SetCage(RECT rcCage) { mrcCage = rcCage; }
    POINT GetMouseDelta(void) { return mDelta; }

    bool IsPressed(UINT nBtn) 
       { if (nBtn<3) return mbPressed[nBtn]; return false; }
    bool IsReleased(UINT nBtn)
       { if (nBtn<3) return mbReleased[nBtn]; return false; }

};

#endif