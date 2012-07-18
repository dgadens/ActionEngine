#include "ACJoystick.h"

ACJoystick::ACJoystick(LPDIRECTINPUT8 lp, HWND hwnd)
{
};

ACJoystick::~ACJoystick(void)
{
};

HRESULT ACJoystick::Init(void)
{
	return AC_OK;
};

HRESULT ACJoystick::Update(void)
{
	return AC_OK;
};

BOOL ACJoystick::EnumJoyCallback(const DIDEVICEINSTANCE *pInst)
{
	return true;
};
    
HRESULT ACJoystick::GetJoyDeflection(float *pfX, float *pfY)
{
	return AC_OK;
};