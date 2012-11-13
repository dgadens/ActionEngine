#include "StdAfx.h"
#include "ACMouseW.h"

using namespace EngineWrapper;

ACMouseW::ACMouseW(ACMouse* mouse)
{
	pMouse = mouse;
}

ACMouseW::~ACMouseW()
{
	Release();
}

void ACMouseW::Release()
{
	delete pMouse;
}