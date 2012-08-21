//criar o dispositivo utilizando a interface
#include "ACRenderer.h"

ACRenderer::ACRenderer(HINSTANCE hInst)
{
	mhInst = hInst;
	mpDevice = nullptr;
	mhDLL = nullptr;
};

ACRenderer::~ACRenderer()
{
	Release();
};

HRESULT ACRenderer::CreateDevice(const std::string &graphicsLibraryName)
{
	CHAR buffer[300];

	//tem q colocar erra porra de L na frente pq o windows trabalha com Unicode, daria pra usar o nome do Metodo com "A" no final
	//mas eu acho palha usar esse metodo com A
	mhDLL = LoadLibraryA(graphicsLibraryName.c_str());
	if (!mhDLL)
		return E_FAIL;

	CREATERENDERDEVICE _createRenderDevice = 0;
	HRESULT hr;

	//ponteiro para a funcao createdevice
	_createRenderDevice = (CREATERENDERDEVICE)GetProcAddress(mhDLL, "CreateRenderDevice");

	if (!_createRenderDevice) return E_FAIL;

	//chama a funcao da dll q cria o dispositivo
	hr=_createRenderDevice(mhDLL, &mpDevice);

	if (FAILED(hr))
	{
		mpDevice = nullptr;
		return E_FAIL;
	}

	return S_OK;
};

void ACRenderer::Release()
{
	RELEASERENDERDEVICE _releaseRenderDevice = 0;
	HRESULT hr;

	if (mhDLL)
	{
		//obtem o metodo release da dll
		_releaseRenderDevice = (RELEASERENDERDEVICE)GetProcAddress(mhDLL, "ReleaseRenderDevice");
	}

	if (mpDevice)
	{
		hr = _releaseRenderDevice(&mpDevice);

		if (FAILED(hr))
		{
			mpDevice = nullptr;
		}
	}
};