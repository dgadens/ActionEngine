#include "EngineWrapper.h"

//engine methods
DLLEXPORT HRESULT InitializeGraphicsDevice(HINSTANCE hInst, char *chAPI, HWND hWnd, BOOL saveLog)
{
	ACGlobals::ENABLE_LOG = saveLog;
	ACGlobals::EDITOR_MODE = true;

	#pragma region CRIA GRAPHICSDEVICE
	//*** CRIA O PONTEIRO PARA O GRAPHICSDEVICE ***
	//cria o objeto renderizador
	pRenderer = new ACRenderer(hInst);
	if (FAILED(pRenderer->CreateDevice(chAPI)))
		return E_FAIL;
	pRenderDevice = pRenderer->GetDevice();
	if (pRenderDevice == nullptr) 
		return E_FAIL;
	if (FAILED(pRenderDevice->Init(hWnd, false, true)))
		return AC_FAIL;
	//ativa a janela principal
	pRenderDevice->SetActiveViewport(hWnd);
	#pragma endregion

	#pragma region CRIA CAMERA
	RECT rcWnd;
	//pega a area do cliente
	GetClientRect(hWnd, &rcWnd);

	ACCamera* pCamera = new ACCamera();
	pCamera->SetWidth(rcWnd.right - rcWnd.left);
	pCamera->SetHeight(rcWnd.bottom - rcWnd.top);
	pCamera->ResetCamera();

	mapCameras.insert(std::pair<HWND, ACCamera*>(hWnd, pCamera));
	#pragma endregion

	mActiveWnd = hWnd;

	return S_OK;
};
DLLEXPORT HRESULT AddViewport(HWND hWnd)
{
	RECT rcWnd;
	//pega a area do cliente
	GetClientRect(hWnd, &rcWnd);

	//adiciona uma nova vp
	pRenderDevice->AddViewport(hWnd, false);

	ACCamera* pCamera = new ACCamera();
	pCamera->SetWidth(rcWnd.right - rcWnd.left);
	pCamera->SetHeight(rcWnd.bottom - rcWnd.top);
	pCamera->ResetCamera();

	mapCameras.insert(std::pair<HWND, ACCamera*>(hWnd, pCamera));

	mActiveWnd = hWnd;

	return S_OK;
};

DLLEXPORT void DropViewport(HWND hWnd)
{
	//adiciona uma nova vp
	vecDropVpList.push_back(hWnd);
};

DLLEXPORT void SetWindowSize(HWND hWnd, int width, int height)
{
	ACCamera* pCamera = mapCameras[hWnd];
	pCamera->SetWidth(width);
	pCamera->SetHeight(height);
};

DLLEXPORT void ActiveViewport(HWND hWnd)
{
	mActiveWnd = hWnd;
	pRenderDevice->SetActiveViewport(mActiveWnd);
};

DLLEXPORT void SetClearColor(float r, float g, float b)
{
	pRenderDevice->SetClearColor(Vector4(r, g, b, 1));
};
DLLEXPORT void Lock()
{
	pRenderDevice->IsRunning = false;
};
DLLEXPORT void Unlock()
{
	pRenderDevice->IsRunning = true;
};

//update
DLLEXPORT void Update()
{
	ACTimeControl::Update();
	if (pRenderDevice->IsRunning)
	{
		for(auto i = mapCameras.begin(); i != mapCameras.end(); ++i)
		{
			ACCamera* pCamera = i->second;
			pCamera->Update();
		}
	}
};

//draw
DLLEXPORT void Render()
{
	//render
	if (pRenderDevice->IsRunning)
	{
		////percorre as vps e renderiza todas, como existe uma camera para cada cena entao ele usa pra renderizar a scena certa na vp certa
		//typedef std::map<HWND, ACCamera*>::const_iterator It; 
		//for(It i = mapCameras.begin(); i != mapCameras.end(); ++i)
		//{
		//	ACCamera* pCamera = i->second;
		//	pRenderDevice->SetActiveRenderingViewport(i->first);
		//	pRenderDevice->BeginRendering();
		//	mapSceneManager[i->first]->RenderAllModels(pCamera);
		//	pRenderDevice->EndRendering();
		//}
	}

	VerifyDropViews();
};
DLLEXPORT void Release()
{
	SAFE_DELETE(pRenderer);
};
//fim engine methods



//verifica se alguma vp foi deletada
void VerifyDropViews()
{
	pRenderDevice->IsRunning = false;
	for ( int i = 0; i < vecDropVpList.size(); i++)
	{
		HWND hWnd = vecDropVpList[i];
		pRenderDevice->DropViewport(hWnd);	
		
		mapCameras[hWnd]->Release();
		mapCameras.erase(hWnd);
	}
	vecDropVpList.clear();
	pRenderDevice->IsRunning = true;
};
