#include "Globals.h"

HWND				Globals::MainWnd;
ACRenderDevice*		Globals::pRenderDevice = nullptr;
ACInputDevice*		Globals::pInputDevice = nullptr;
ACNetworkDevice*	Globals::pNetworkDevice = nullptr;
ACAudioDevice*		Globals::pAudioDevice = nullptr;
ACKeyboard*			Globals::pKeyboard = nullptr;
ACMouse*			Globals::pMouse = nullptr;

ACCamera*			Globals::pCamera = nullptr;
INT					Globals::MainWndWidth = 1024;
INT					Globals::MainWndHeight = 768;

BOOL			    Globals::IsRunning = TRUE;

NetModeType			Globals::NetMode;
UINT				Globals::NetID = -1;    