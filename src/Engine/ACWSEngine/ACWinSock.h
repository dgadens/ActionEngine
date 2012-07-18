//*****************************
// Implementacao da interface acnetwork com winsocks
// Creator: Daniel Gadens
// Date: 25/08/2011
//*****************************

#ifndef __ACWINSOCK_H
#define __ACWINSOCK_H

#pragma comment(lib, "ws2_32.lib")

#include "ACNetworkDevice.h"
#include "ACWSStructs.h"
#include <windows.h>

class ACSocketObject;

class ACWinSock : public ACNetworkDevice 
{
private:
    ACSocketObject *mpSockObj;
    NetModeType     mMode;
    ACCLIENT        mClients[256];
    char            mClCount;
    UINT            mClID;
    UINT            mClMax;
    char           *mBuffer;

    // initialize socketobjects
    HRESULT CreateServer(ACSocketObject **ppSkObject);
    HRESULT CreateClient(ACSocketObject **ppSkObject);

    // during message processing
    HRESULT OnAccept(void);
    HRESULT OnReceive(SOCKET skReceiving);
    HRESULT OnDisconnect(SOCKET skDisconnecting);

public:
    ACWinSock(HINSTANCE hDLL);
    ~ACWinSock(void);
      
    // initialize the engine stuff
    HRESULT Init(HWND, NetModeType, int,const char*, UINT, UINT, BOOL);
    void    Release(void);
	void	Log(char* message, ...);

    HRESULT MsgProc(WPARAM wp, LPARAM lp);
    HRESULT SendToServer(const ACPACKAGE*);
    HRESULT SendToClients(const ACPACKAGE*);
    HRESULT SendToClient(const ACPACKAGE*,UINT);
    bool    IsPkgWaiting(void)           
	{ 
		return mpSockObj->IsPkgWaiting();   
	}
    UINT    GetNextPkgSize(void)         
	{ 
		return mpSockObj->GetNextPkgSize(); 
	}
    HRESULT GetNextPkg(ACPACKAGE *pPkg) 
	{ 
		return mpSockObj->GetNextPkg(pPkg); 
	}
}; 

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACWS.h dentro da dll
DLLEXPORT HRESULT CreateNetworkDevice(HINSTANCE hDLL, ACNetworkDevice **pInterface);
DLLEXPORT HRESULT ReleaseNetworkDevice(ACNetworkDevice **pInterface);

#endif