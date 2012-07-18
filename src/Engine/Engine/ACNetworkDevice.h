//************************************
// Interface para o dispositivo de network
// Creator: Daniel Gadens
// Date: 04/08/2011
//************************************

#ifndef __ACNETWORKDEVICE_H
#define __ACNETWORKDEVICE_H

#include <windows.h>
#include <stdio.h>
#include "ACPackage.h"
#include "EngineCommon.h"

#define WM_ACSERVER (WM_USER + 1)
#define WM_ACCLIENT (WM_USER + 2)

class ACNetworkDevice 
{
protected:
	HWND		mActiveWnd;		   //janela ativa para manipulacao
    HINSTANCE	mhDLL;             // dll module handle
	int			mPort;             // port for communication
	char		mpIP[256];         // IP address (valid for clients)
	UINT		mMaxSize;          // max package size for send/receive 
	FILE*		mpLOG;			   //arquivo de log
      
public:
	bool IsRunning;
    ACNetworkDevice(void) {};
    virtual ~ACNetworkDevice(void) {};

	// feed: main window, server or client, port, ip (for clients only)
	//       max size of packages send over network, flush log
	virtual HRESULT Init(HWND, NetModeType, int nPort, const char *pIP, UINT maxPkgSize, UINT NumMax, BOOL log)=0;
	virtual void Release(void)=0;
	virtual void Log(char* message, ...) =0;
      
	// proceed incoming notifications from windows
	virtual HRESULT MsgProc(WPARAM, LPARAM)=0;

	// send methods for clients and servers
	virtual HRESULT SendToServer(const ACPACKAGE*)=0;
	virtual HRESULT SendToClients(const ACPACKAGE*)=0;
	virtual HRESULT SendToClient(const ACPACKAGE*,UINT)=0;

	// get information about queued packages
	virtual bool    IsPkgWaiting(void)=0;
	virtual UINT    GetNextPkgSize(void)=0;
	virtual HRESULT GetNextPkg(ACPACKAGE*)=0;
}; 

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACDI.h dentro da dll caso use joystick
typedef HRESULT (*CREATENETWORKDEVICE) (HINSTANCE hDLL, ACNetworkDevice **pInterface);
typedef HRESULT (*RELEASENETWORKDEVICE) (ACNetworkDevice **pInterface);

#endif
