#include "SD_Globals.h"

HWND				SD_Globals::MainWnd;
ACRenderDevice*		SD_Globals::pRenderDevice = nullptr;
ACInputDevice*		SD_Globals::pInputDevice = nullptr;
ACNetworkDevice*	SD_Globals::pNetworkDevice = nullptr;
ACAudioDevice*		SD_Globals::pAudioDevice = nullptr;
ACKeyboard*			SD_Globals::pKeyboard = nullptr;
ACMouse*			SD_Globals::pMouse = nullptr;
					
ACCamera*			SD_Globals::pCamera = nullptr;
INT					SD_Globals::MainWndWidth = 1024;
INT					SD_Globals::MainWndHeight = 768;
					
BOOL			    SD_Globals::IsRunning = TRUE;
					
NetModeType			SD_Globals::NetMode;
UINT				SD_Globals::NetID = -1;  