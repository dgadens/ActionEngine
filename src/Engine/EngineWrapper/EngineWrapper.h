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

std::map<HWND, ACCamera*>		mapCameras;
HWND							mActiveWnd; //janela ativa para manipulacao nao renderizacao a renderizacao sempre renderiza todas
std::vector<HWND>               vecDropVpList;
ACRenderer*						pRenderer;
ACRenderDevice*					pRenderDevice;	

//engine methods
DLLEXPORT HRESULT InitializeGraphicsDevice(HINSTANCE hInst, char *chAPI, HWND hWnd, BOOL saveLog);
DLLEXPORT HRESULT AddViewport(HWND hWnd);
DLLEXPORT void DropViewport(HWND);
DLLEXPORT void SetWindowSize(HWND hWnd, int width, int height);
DLLEXPORT void SetClearColor(float r, float g, float b);
DLLEXPORT void ActiveViewport(HWND);
DLLEXPORT void Lock();
DLLEXPORT void Unlock();
DLLEXPORT void Update();
DLLEXPORT void Render();
DLLEXPORT void Release();
//fim engine methods

//metodo q remove tudo q tem na pulha de vps
void VerifyDropViews();

#endif