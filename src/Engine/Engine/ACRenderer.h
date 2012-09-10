//******************************************************
// Essa classe é responsavel por manter o dispositivo grafico
// creator: Daniel Gadens
// date: 05/05/2011
//*****************************************************

#ifndef __ACRENDERER_H
#define __ACRENDERER_H

#include "EngineCommon.h"
#include "ACRenderDevice.h"

class ACRenderer
{
private:
	ACRenderDevice *mpDevice;
	HINSTANCE mhInst;
	HMODULE mhDLL;

public:
	ACRenderer(HINSTANCE hInst);
	~ACRenderer();

	HRESULT CreateDevice(const std::string &graphicsLibraryName);
	void Release();
	ACRenderDevice* GetDevice() { return mpDevice; }
	HINSTANCE GetModule() { return mhDLL; }
};

#endif