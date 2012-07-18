
#ifndef __ACJOYSTICK_H
#define __ACJOYSTICK_H

#include "EngineCommon.h"
#include "ACDIDevice.h"
#include <dinput.h>


class ACJoystick : public ACDIDevice 
{
private:
	GUID       mguid;
	std::string  mName;
	bool       mbJoyFound;
	bool       mbPressed[12];
	bool       mbReleased[12];
	DWORD      mdwNumBtns;

public:
    ACJoystick(LPDIRECTINPUT8, HWND);
    ~ACJoystick(void);

    HRESULT Init(void);
    HRESULT Update(void);

    bool JoystickFound(void) { return mbJoyFound; }
    BOOL EnumJoyCallback(const DIDEVICEINSTANCE *pInst);
    
    void GetName(std::string name) 
       { mName = name; }

    bool IsPressed(UINT nBtn) 
       { if (nBtn<mdwNumBtns) return mbPressed[nBtn]; return false; }

    bool IsReleased(UINT nBtn) 
       { if (nBtn<mdwNumBtns) return mbReleased[nBtn]; return false; }

    HRESULT GetJoyDeflection(float *pfX, float *pfY);

}; 

#endif