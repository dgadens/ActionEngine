#include "StdAfx.h"
#include "ACRendererW.h"


ACRendererW::ACRendererW(IntPtr hInst)
{
	pRenderer = new ACRenderer((HINSTANCE)hInst.ToPointer());
};
