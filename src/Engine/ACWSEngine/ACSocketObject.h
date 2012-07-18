//****************************
// Classe com o objeto socket
// Creator: Daniel Gadens
// Date: 05/08/2011
//*************************


#ifndef __ACSOCKETOBJECT_H
#define __ACSOCKETOBJECT_H

#include "ACQueue.h"
#include "ACPackage.h"
#include <windows.h>

class ACSocketObject 
{
private:
    ACQueue*   mpInbox;
    SOCKET     mskSocket;
    char*      mBuffer;
	FILE*	   mpLOG;

public:
	bool    IsRunning;
    ACSocketObject(FILE *pLOG);
    ~ACSocketObject(void);

	void Log(char* message, ...);
	void LogLastWSAError(const char*);

    // socket misc operations
    HRESULT CreateSocket(void);
    HRESULT Bind(int nPort);
    HRESULT Listen(void);
    HRESULT Accept(SOCKET *skToNewClient);
    HRESULT Connect(char *chServer, int nPort);
    void    Disconnect(void);

    // send and receive data
    int     Send(const char*,UINT);
    int     Send(const char*,UINT,SOCKET);
    HRESULT Receive(SOCKET sk);

    void    FeedByHand(ACPACKAGE *pPkg);

    // for informational purposes
    SOCKET  GetSocket(void) { return mskSocket; }

    // information about inbox
    bool    IsPkgWaiting(void)   
	{ 
		return (mpInbox->GetCount() > 0); 
	}
    UINT    GetNextPkgSize(void) 
	{ 
		return (mpInbox->GetFrontSize()); 
	}
    HRESULT GetNextPkg(ACPACKAGE*);
};

#endif