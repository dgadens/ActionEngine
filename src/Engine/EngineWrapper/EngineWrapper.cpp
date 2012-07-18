#include "EngineWrapper.h"

//engine methods
DLLEXPORT HRESULT InitWrapper(HINSTANCE hInst, char *chAPI, HWND hWnd, BOOL saveLog)
{
	ACGlobals::ENABLE_LOG = saveLog;
	ACGlobals::EDITOR_MODE = true;

	pRendererWrapper = CreateRenderer(hInst);

	//cria o dispositivo grafico
	if (FAILED(pRendererWrapper->CreateDevice(chAPI)))
		return E_FAIL;

	//salva o ponteiro para o graphicsdevice
	pDeviceWrapper = pRendererWrapper->GetDevice();

	if (pDeviceWrapper == nullptr) return E_FAIL;

	RECT rcWnd;
	//pega a area do cliente
	GetClientRect(hWnd, &rcWnd);

	//inicializa a vp principal
	pDeviceWrapper->Init(hWnd, false, false);	
	
	mapSceneManagerWrapper.insert(std::pair<HWND, PACSCENEMANAGER>(hWnd,new ACSCENEMANAGER()));

	PACCAMERA pCamera = new ACCAMERA();
	pCamera->SetWidth(rcWnd.right - rcWnd.left);
	pCamera->SetHeight(rcWnd.bottom - rcWnd.top);
	pCamera->ResetCamera();

	mapCamerasWrapper.insert(std::pair<HWND, PACCAMERA>(hWnd, pCamera));

	mActiveWnd = hWnd;

	return S_OK;
};
DLLEXPORT HRESULT AddViewportWrapper(HWND hWnd)
{
	RECT rcWnd;
	//pega a area do cliente
	GetClientRect(hWnd, &rcWnd);

	//adiciona uma nova vp
	pDeviceWrapper->AddViewport(hWnd, false);	

	mapSceneManagerWrapper.insert(std::pair<HWND, PACSCENEMANAGER>(hWnd,new ACSCENEMANAGER()));

	PACCAMERA pCamera = new ACCAMERA();
	pCamera->SetWidth(rcWnd.right - rcWnd.left);
	pCamera->SetHeight(rcWnd.bottom - rcWnd.top);
	pCamera->ResetCamera();

	mapCamerasWrapper.insert(std::pair<HWND, PACCAMERA>(hWnd, pCamera));

	mActiveWnd = hWnd;

	return S_OK;
};

DLLEXPORT void DropViewportWrapper(HWND hWnd)
{
	//adiciona uma nova vp
	vecDropVpList.push_back(hWnd);
};

DLLEXPORT void SetWindowSizeWrapper(HWND hWnd, int width, int height)
{
	PACCAMERA pCamera = mapCamerasWrapper[hWnd];
	pCamera->SetWidth(width);
	pCamera->SetHeight(height);
};

DLLEXPORT void ActiveViewportWrapper(HWND hWnd)
{
	mActiveWnd = hWnd;
	pDeviceWrapper->SetActiveViewport(mActiveWnd);
};

DLLEXPORT void SetClearColorWrapper(float r, float g, float b)
{
	pDeviceWrapper->SetClearColor(r, g, b);
};
DLLEXPORT void LockWrapper()
{
	pDeviceWrapper->IsRunning = false;
};
DLLEXPORT void UnlockWrapper()
{
	pDeviceWrapper->IsRunning = true;
};

//update
DLLEXPORT void UpdateWrapper()
{
	ACTimeControl::Update();
	if (pDeviceWrapper->IsRunning)
	{
		typedef std::map<HWND, PACCAMERA>::const_iterator It; 
		for(It i = mapCamerasWrapper.begin(); i != mapCamerasWrapper.end(); ++i)
		{
			PACCAMERA pCamera = i->second;
			pCamera->Update();

			for (int j = 0; j < mapSceneManagerWrapper[i->first]->GetModelCount(); j++)
			{
				PACMODEL m = mapSceneManagerWrapper[i->first]->GetModel(j);
				m->SetAbsolutePosition(0, 0, -200);
				m->SetAbsoluteScale(200.0f,200.0f,200.0f);
				m->AddToRotation(0, ACTimeControl::GetFElapsedTime()*3, 0);
			}

			mapSceneManagerWrapper[i->first]->Update(ACTimeControl::GetFElapsedTime());
		}
	}
};

//draw
DLLEXPORT void RenderWrapper()
{
	//render
	if (pDeviceWrapper->IsRunning)
	{
		//percorre as vps e renderiza todas, como existe uma camera para cada cena entao ele usa pra renderizar a scena certa na vp certa
		typedef std::map<HWND, PACCAMERA>::const_iterator It; 
		for(It i = mapCamerasWrapper.begin(); i != mapCamerasWrapper.end(); ++i)
		{
			PACCAMERA pCamera = i->second;
			pDeviceWrapper->SetActiveRenderingViewport(i->first);
			pDeviceWrapper->BeginRendering();
			mapSceneManagerWrapper[i->first]->RenderAllModels(pCamera);
			pDeviceWrapper->EndRendering();
		}
	}

	VerifyDropViews();
};
DLLEXPORT void ReleaseWrapper()
{
	delete pRendererWrapper;
	pRendererWrapper = nullptr;
};
//fim engine methods


// Model methods
DLLEXPORT void AddModelWrapper(char* name)
{
	//MessageBoxA(nullptr, "Foi", "abc", 0);
	PACMODEL model = new ACMODEL(pDeviceWrapper);
	model->Load(name, ET_Textured);
	pDeviceWrapper->AddModel(model);
	mapSceneManagerWrapper[mActiveWnd]->AddModel(model);
};
DLLEXPORT void ShowNormalsWrapper(char* name, bool value)
{
	mapSceneManagerWrapper[mActiveWnd]->GetModel(0)->SetRenderNormals(value);
};

DLLEXPORT void ShowBBsWrapper(char* name, bool value)
{
	mapSceneManagerWrapper[mActiveWnd]->GetModel(0)->SetRenderBB(value);
};

DLLEXPORT void ShowBonesWrapper(char* name, bool value)
{
	mapSceneManagerWrapper[mActiveWnd]->GetModel(0)->SetRenderBones(value);
};
//fim model methods



//verifica se alguma vp foi deletada
void VerifyDropViews()
{
	pDeviceWrapper->IsRunning = false;
	for ( int i = 0; i < vecDropVpList.size(); i++)
	{
		HWND hWnd = vecDropVpList[i];
		pDeviceWrapper->DropViewport(hWnd);	
		
		mapCamerasWrapper[hWnd]->Release();
		mapCamerasWrapper.erase(hWnd);
		
		mapSceneManagerWrapper[hWnd]->ReleaseAll();
		mapSceneManagerWrapper.erase(hWnd);
	}
	vecDropVpList.clear();
	pDeviceWrapper->IsRunning = true;
};
