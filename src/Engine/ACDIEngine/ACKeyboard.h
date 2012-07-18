#pragma once
#ifndef __ACKEYBOARD_H
#define __ACKEYBOARD_H

#include "EngineCommon.h"
#include "ACDIDevice.h"
#include <dinput.h>

class ACKeyboard : public ACDIDevice 
{
private:
	char  mKeys[256];
	char  mKeysOld[256];

public:
    ACKeyboard(LPDIRECTINPUT8, HWND);
    ~ACKeyboard(void);

    HRESULT Init(void);
    HRESULT Update(void);

    bool    IsPressed(UINT nID);
    bool    IsReleased(UINT nID);

};

#endif