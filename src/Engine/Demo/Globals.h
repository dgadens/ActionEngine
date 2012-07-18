
#ifndef __GLOBALS_H
#define __GLOBALS_H

#include "ACContentManager.h"
#include "ACRenderDevice.h"
#include "ACInputDevice.h"
#include "ACNetworkDevice.h"
#include "ACAudioDevice.h"
#include "ACKeyboard.h"
#include "ACMouse.h"

const UINT MSG_CHAT		   = 3;
const UINT MAX_PKG_SIZE    = 4096;
const UINT MAX_CLIENTS     = 16;

//nao esquecer de declarar no CPP tb
class Globals
{
public:
	static HWND				 MainWnd;
	static ACRenderDevice*   pRenderDevice;
	static ACInputDevice*    pInputDevice;
	static ACNetworkDevice*  pNetworkDevice;
	static ACAudioDevice*    pAudioDevice;
	static ACKeyboard*       pKeyboard;
	static ACMouse*		     pMouse;

	static ACContentManager* pContentManager;
	static ACCamera*	     pCamera;
	static INT			     MainWndWidth;
	static INT			     MainWndHeight;
						     
	static BOOL              IsRunning;

	static NetModeType       NetMode;
	static UINT				 NetID; 

};

#endif