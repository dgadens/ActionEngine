//**********************************
// Wrapper da engine para ser usando no editor
// creator: Daniel Gadens
// data: 15/05/2011
//**********************************

#pragma once
#ifndef __ENGINEWRAPPER_H
#define __ENGINEWRAPPER_H

#include <map>
#include "EngineMain.h"
#include "EngineCommon.h"

std::map<HWND, PACCAMERA>		mapCamerasWrapper;
std::map<HWND, PACSCENEMANAGER>	mapSceneManagerWrapper;
HWND							mActiveWnd; //janela ativa para manipulacao nao renderizacao a renderizacao sempre renderiza todas
std::vector<HWND>               vecDropVpList;
PACRENDERER						pRendererWrapper;
PACRENDERDEVICE					pDeviceWrapper;

//engine methods
DLLEXPORT HRESULT InitWrapper(HINSTANCE hInst, char *chAPI, HWND hWnd, BOOL saveLog);
DLLEXPORT HRESULT AddViewportWrapper(HWND hWnd);
DLLEXPORT void DropViewportWrapper(HWND);
DLLEXPORT void SetWindowSizeWrapper(HWND hWnd, int width, int height);
DLLEXPORT void SetClearColorWrapper(float r, float g, float b);
DLLEXPORT void ActiveViewportWrapper(HWND);
DLLEXPORT void LockWrapper();
DLLEXPORT void UnlockWrapper();
DLLEXPORT void UpdateWrapper();
DLLEXPORT void RenderWrapper();
DLLEXPORT void ReleaseWrapper();
//fim engine methods

//model methods
DLLEXPORT void AddModelWrapper(char* name);
DLLEXPORT void ShowNormalsWrapper(char* name, bool value);
DLLEXPORT void ShowBBsWrapper(char* name, bool value);
DLLEXPORT void ShowBonesWrapper(char* name, bool value);
//fim model methods

//metodo q remove tudo q tem na pulha de vps
void VerifyDropViews();

#endif