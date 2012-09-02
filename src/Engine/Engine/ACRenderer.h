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
	__declspec(dllexport) ACRenderer(HINSTANCE hInst);
	~ACRenderer();

	__declspec(dllexport) HRESULT CreateDevice(const std::string &graphicsLibraryName);
	void Release();
	__declspec(dllexport) ACRenderDevice* GetDevice() { return mpDevice; }
	HINSTANCE GetModule() { return mhDLL; }
};

#endif