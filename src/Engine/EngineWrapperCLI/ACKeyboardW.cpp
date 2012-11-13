#include "StdAfx.h"
#include "ACKeyboardW.h"

using namespace EngineWrapper;

ACKeyboardW::ACKeyboardW(ACKeyboard* keyboard)
{
	pKeyboard = keyboard;
}

ACKeyboardW::~ACKeyboardW()
{
	Release();
}

void ACKeyboardW::Release()
{
	delete pKeyboard;
}

