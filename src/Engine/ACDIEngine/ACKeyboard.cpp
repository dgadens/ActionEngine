#include "ACKeyboard.h"

ACKeyboard::ACKeyboard(LPDIRECTINPUT8 lp, HWND hwnd)
{
};

ACKeyboard::~ACKeyboard(void)
{
};

HRESULT ACKeyboard::Init(void)
{
	return AC_OK;
};

HRESULT ACKeyboard::Update(void)
{
	return AC_OK;
};

bool ACKeyboard::IsPressed(UINT nID)
{
	return true;
};

bool ACKeyboard::IsReleased(UINT nID)
{
	return true;
};