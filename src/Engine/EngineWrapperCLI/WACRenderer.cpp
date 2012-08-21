#include "StdAfx.h"
#include "WACRenderer.h"


WACRenderer::WACRenderer(IntPtr hInst)
{
	pRenderer = new ACRenderer((HINSTANCE)hInst.ToPointer());
};
