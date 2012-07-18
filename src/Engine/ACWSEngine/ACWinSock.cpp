#include "ACSocketObject.h"
#include "ACWinSock.h"

//methodo externo a classe q cria o gdevice
HRESULT CreateNetworkDevice(HINSTANCE hDLL, ACNetworkDevice **pDevice)
{
	if (!*pDevice)
	{
		*pDevice = new ACWinSock(hDLL);
		return AC_OK;
	}

	return AC_FAIL;
};

//metodo externo q libera o gdevice
HRESULT ReleaseNetworkDevice(ACNetworkDevice **pDevice)
{
	if (!*pDevice)
	{
		return AC_FAIL;
	}

	SAFE_DELETE(*pDevice);

	return AC_OK;
};

bool g_bLF=false;
int  g_PkgSize=sizeof(ACPACKAGE);

ACWinSock::ACWinSock(HINSTANCE hDLL) 
{
   mhDLL         = hDLL;
   mpSockObj     = nullptr;
   mBuffer       = nullptr;
   mpLOG		 = nullptr;
   mPort         = 0;
   mClID         = 1;        // 0 reserved for server
   mClCount      = 0;
   mClMax        = 0;        // 0 = unlimited
   IsRunning     = false;
};

ACWinSock::~ACWinSock() 
{
   Release();
};


//release de tudo
void ACWinSock::Release() 
{
	Log("shutting down WinSock");

	if (mMode == NetModeType::NMT_Server) 
	{
		for (int i=0; i<mClCount; i++)
		{
			shutdown(mClients[i].skToClient,0x02);
			closesocket(mClients[i].skToClient);
			mClients[i].skToClient = INVALID_SOCKET;
		}
	}

	SAFE_DELETE(mpSockObj);

	SAFE_DELETE_A(mBuffer);

	// clean up winsock
	WSACleanup();

	Log("offline (ok)");

	if (mpLOG!=nullptr)
	{
		fclose(mpLOG);
		mpLOG = nullptr;
	}

	IsRunning = false;
};


/**
 * Initialize WInSock first, then the appropriate socketobjects.
 * -> IN: HWND       - handle to application main window
 *        ACNETMODE - put NMD_CLIENT or NMD_SERVER here
 *        int        - number of port to use
 *        char       - ip address needed for client construction
 *        UINT       - maximum size of one package
 *        UINT       - maximum number of clients for one server
 *        bool       - flush log at each write operation
 */
HRESULT ACWinSock::Init(HWND hWnd, NetModeType nmd, int nPort, const char *pIP, UINT nMaxPkgSize, UINT nMaxClients, BOOL log) 
{
	if (log)
		mpLOG = fopen("NetworkLOG.txt", "w");

	Log("Begin Network Init");

	WSADATA wsaData;
	UINT    nEvents=0;
	WORD    wVersion;
	int     nRes;

	mMaxSize    = nMaxPkgSize;
	mBuffer     = new char[mMaxSize+g_PkgSize];
	mActiveWnd  = hWnd;
	mPort       = nPort;
	mMode       = nmd;
	mClMax      = nMaxClients;

	if (pIP) sprintf(mpIP, "%s", pIP);

	wVersion = MAKEWORD(2,0);

	// startup WinSock
	if ((nRes=WSAStartup(wVersion, &wsaData))!=0) 
	{
		if (nRes==WSAEINPROGRESS)
			Log("[ERROR]  blocking WinSock app is running");
		else if (nRes==WSAVERNOTSUPPORTED)
			Log("[ERROR]  WinSock version not supported");
		else if (nRes==WSASYSNOTREADY)
			Log("[ERROR]  network subsystem not ready");
		else if (nRes==WSAEPROCLIM)
			Log("[ERROR]  WinSock too buisy for another task");
		return AC_FAIL;
	}
	
	Log("WinSock 2.0 initialized (NMD: %d)", mMode);

	// create SockectObject as server listening to port
	if (mMode==NetModeType::NMT_Server) 
	{ 
		if (FAILED(CreateServer(&mpSockObj))) 
		{
			Log("[ERROR]  CreateServer(Port:%d) failed", mPort);
			return AC_FAIL;
		}
	}
	// create SocketObject as client sending data to IP::Port
	else if (mMode==NetModeType::NMT_Client) 
	{
		if (strcmp(mpIP, "")==0)
			sprintf(mpIP, "LOCALHOST");
		if (FAILED(CreateClient(&mpSockObj))) 
		{
			Log("[ERROR]  CreateClient(Port:%d to Server-IP:%s) failed", mPort, mpIP);
			return AC_FAIL;
		}
	}
	else
		return AC_INVALIDPARAM;

	Log("initialized (online and ready)");
	IsRunning = true;

	return AC_OK;
};

void ACWinSock::Log(char* message, ...)
{
	if (mpLOG!=nullptr)
	{
	   char ch[256];
	   char *pArgs;

	   pArgs = (char*) &message + sizeof(message);
	   vsprintf(ch, message, pArgs);
	   fprintf(mpLOG, ch);
	   fprintf(mpLOG, "\n");
	   fflush(mpLOG);
	}
};

/**
 * Create socketobject as client and make it ready for traffic. A
 * client needs to be connected to the server IP at the port the
 * server is listening to.
 */
HRESULT ACWinSock::CreateClient(ACSocketObject **ppSkObject) 
{
	UINT nEvents=0;

	//Log("firing up client");
	(*ppSkObject) = new ACSocketObject(mpLOG);

	if (!(*ppSkObject)) 
	{
		Log("[ERROR]  ACSocketObject construction failed in ACWinSock::CreateClient");
		return AC_FAIL;
	}
	Log("ACSocketObject constructed");

	// 1. step: create a socket object
	if (FAILED( (*ppSkObject)->CreateSocket() ))
		return AC_FAIL;
	Log("socket created");

	if (mpIP == nullptr) 
		gethostname(mpIP, 10);
   
	// 2. step: try to connect to the server
	if (FAILED( (*ppSkObject)->Connect(mpIP, mPort) ))
		return AC_FAIL;
	Log("ACSocketObject connected as client to port: %d", mPort);

	nEvents |= FD_READ | FD_CLOSE;

	// 3. step: notification from windows WM_ about incoming events
	if (WSAAsyncSelect( (*ppSkObject)->GetSocket(), mActiveWnd,	WM_ACCLIENT, nEvents)==SOCKET_ERROR) 
	{
		Log("[ERROR]  WSAAsyncSelect(WM_ACCLIENT) failed");
		mpSockObj->Disconnect();

		return AC_FAIL;
	}

	return AC_OK;
};


/**
 * Create socketobjects as server and make them ready for traffic.
 * The server just needs to listen to the port the client is sending
 * its data to.
 */
HRESULT ACWinSock::CreateServer(ACSocketObject **ppSkObject) 
{
   UINT nEvents=0;

	//Log("firing up server");
	(*ppSkObject) = new ACSocketObject(mpLOG);
   
	if (!(*ppSkObject)) 
	{
		Log("[ERROR]  m_pSkObject construction failed in ACWinSock::CreateServer");
		return AC_FAIL;
	}
	Log("ACSocketObjects constructed");

	// 1. step: create a socket object
	if (FAILED( (*ppSkObject)->CreateSocket() ))
		return AC_FAIL;
	Log("socket created");
   
	// 2. step: name the socket cause it's a server
	if (FAILED( (*ppSkObject)->Bind(mPort) ))
		return AC_FAIL;
	Log("server bind to port: %d", mPort);

	// 3. step: listen for incoming connection requests
	if (FAILED( (*ppSkObject)->Listen() ))
		return AC_FAIL;
	Log("server is now listening for incoming connection...");

	nEvents |= FD_READ | FD_WRITE | FD_CONNECT | FD_ACCEPT | FD_CLOSE;

	// 4. step: notification from windows WM_ about incoming events
	if (WSAAsyncSelect((*ppSkObject)->GetSocket(), mActiveWnd, WM_ACSERVER, nEvents)==SOCKET_ERROR) 
	{
		Log("[ERROR]  WSAAsyncSelect(WM_ACSERVER) failed");
		mpSockObj->Disconnect();
		return AC_FAIL;
	}

	// initialize all client slots as invalid
	for (int i=0; i<256; i++) 
	{
		mClients[i].skToClient = INVALID_SOCKET;
		mClients[i].nID = 0;
	}

	return AC_OK;
};

/**
 * Send a data package from the server to all connected clients.
 * We need to loop through all active connections and send it.
 */
HRESULT ACWinSock::SendToClients(const ACPACKAGE *pPkg) 
{
	HRESULT hr=AC_OK;
	int     nBytes=0;
	int     nSize=g_PkgSize+pPkg->nLength;

	// security checks
	if (mMode != NetModeType::NMT_Server) return AC_FAIL;
	if (nSize > mMaxSize) return AC_OUTOFMEMORY;

	// serialize data in order to send from one mem segment
	memcpy(mBuffer, pPkg, g_PkgSize);
	memcpy(mBuffer+g_PkgSize, pPkg->pData, pPkg->nLength);

	// now go send it actually
	for (UINT i=0; i<mClCount; i++) 
	{
		if (mClients[i].skToClient != INVALID_SOCKET) 
		{
			nBytes = mpSockObj->Send(mBuffer, nSize, mClients[i].skToClient);
			// if failed report but go on with next clients
			if ( (nBytes==SOCKET_ERROR) || (nBytes<nSize) )
				hr = AC_FAIL;
		}
	}

	return hr;
};


/**
 * Send a data package from the server to the given client. We
 * need to loop through all active connections to find that one.
 */
HRESULT ACWinSock::SendToClient(const ACPACKAGE *pPkg, UINT nCID) 
{
	HRESULT hr=AC_OK;
	int     nBytes=0;
	int     nSize=g_PkgSize+pPkg->nLength;

	// security checks
	if (mMode != NetModeType::NMT_Server) return AC_FAIL;
	if (nSize > mMaxSize) return AC_OUTOFMEMORY;

	// serialize data in order to send from one mem segment
	memcpy(mBuffer, pPkg, g_PkgSize);
	memcpy(mBuffer+g_PkgSize, pPkg->pData, pPkg->nLength);

	// now go send it actually
	for (UINT i=0; i<mClCount; i++) 
	{
		if ( (mClients[i].skToClient != INVALID_SOCKET) &&	(mClients[i].nID == nCID) ) 
		{
			nBytes = mpSockObj->Send(mBuffer, nSize, mClients[i].skToClient);
			// if failed report but go on with next clients
			if ( (nBytes==SOCKET_ERROR) || (nBytes<nSize) )
				hr = AC_FAIL;
		}
	}

	return hr;
};


/**
 * A client just sends the package over its socket that is
 * connected to the one and only server. No problems here.
 */
HRESULT ACWinSock::SendToServer(const ACPACKAGE *pPkg) 
{
	int nBytes=0;
	int nSize=g_PkgSize+pPkg->nLength;

	// security checks
	if (nSize > mMaxSize) return AC_OUTOFMEMORY;
   
	// serialize data in order to send from one mem segment
	memcpy(mBuffer, pPkg, g_PkgSize);
	memcpy(mBuffer+g_PkgSize, pPkg->pData, pPkg->nLength);

	// if pkg was sent by server then 
	// feed it directly into its own queue
	if (mMode == NetModeType::NMT_Server) 
	{
		mpSockObj->FeedByHand( (ACPACKAGE*)mBuffer );
		return AC_OK;
	}

	// now go send it actually
	nBytes = mpSockObj->Send(mBuffer, nSize);

	if ( (nBytes==SOCKET_ERROR) || (nBytes<nSize) ) 
	{
		Log("[ERROR]  SendToServer() failed");
		return AC_FAIL;
	}

	return AC_OK;
};

/**
 * Application must call this if a WM_ is coming from network.
 */
HRESULT ACWinSock::MsgProc(WPARAM wp, LPARAM lp) 
{
	WORD  wEvent, wError;
   
	// get data
	wError = HIWORD(lp);
	wEvent = LOWORD(lp);
   
	// which event
	switch (wEvent) 
	{
		// confirmacao depois de uma conexao bem sucedida
		case FD_CONNECT: break;
         
		// cliente quer aceitacao
		case FD_ACCEPT:  { return OnAccept(); } break;
         
		// recebemos algo
		case FD_READ:    { return OnReceive(wp); } break;
         
		// um cliente ficou offline
		case FD_CLOSE:   { return OnDisconnect(wp); } break;
         
		// nos mandamos algo?
		case FD_WRITE:   break;
	}

	return AC_OK;
};


/**
 * There is someone is the queue of our server waiting to be accepted
 * so do him the favor.
 */
HRESULT ACWinSock::OnAccept(void) 
{
	int nSize=0, nBytes=0, i=mClCount;

	// maximum of 255 clients at all
	if (mClCount >= 255) return AC_OUTOFMEMORY;

	// application defined maximum number
	if ( (mClMax > 0) && (mClCount >= mClMax) ) 
		return AC_OUTOFMEMORY;

	if (FAILED(mpSockObj->Accept( &(mClients[i].skToClient) )))
		return AC_FAIL;

	// SEND ITS ID TO THE NEW CLIENT:
	ACPACKAGE *pPkg = (ACPACKAGE*)mBuffer;
	pPkg->pData = &mBuffer[g_PkgSize];
	pPkg->nLength = sizeof(UINT);
	pPkg->nType   = 0;   // ID Msg
	pPkg->nSender = 0;   // Server
	memcpy(pPkg->pData, &mClID, sizeof(UINT));

	// add counters
	mClients[i].nID = mClID;
	mClCount++;
	mClID++;

	nSize = g_PkgSize + pPkg->nLength;
	nBytes = mpSockObj->Send(mBuffer, nSize, mClients[i].skToClient);
	if ( (nBytes==SOCKET_ERROR) || (nBytes<nSize) )
		return AC_FAIL;

	// INFORM ALL CLIENTS AND THE SERVER ABOUT THE NEW KID IN TOWN
	pPkg->nType = 1;
	SendToClients(pPkg);

	// feed msg into server msg-queue
	mpSockObj->FeedByHand(pPkg);

	return AC_OK;
};


/**
 * The giving socket has data to be received in its queue. Just do it!
 */
HRESULT ACWinSock::OnReceive(SOCKET skReceiving) 
{
	if (IsRunning) 
	{
		return mpSockObj->Receive(skReceiving);
    }
	else 
	{
		Log("[ERROR]  iSRunning=0 in ACWinSock::OnReceive");
		return AC_FAIL;
    }
};


/**
 * Close the given socket if he wants to disconnect.
 */
HRESULT ACWinSock::OnDisconnect(SOCKET skDisconnecting) 
{
	UCHAR      i=0;

	if (skDisconnecting==INVALID_SOCKET) 
		return AC_FAIL;

	if (mMode== NetModeType::NMT_Server) 
	{
		// delete from the list of active clients
		for (i=0; i<mClCount; i++) 
		{
			if (mClients[i].skToClient == skDisconnecting)
			break;
		}
     
		if (i>=mClCount) 
		{
			Log("[ERROR]  not listed client wants to be disconnected");
			return AC_FAIL;
		}
      
		// shutdown and close socket
		if ( shutdown(mClients[i].skToClient,0x02)==SOCKET_ERROR )
			mpSockObj->LogLastWSAError("shutdown() in ACWinSock::OnDisconnect");
		if ( closesocket(mClients[i].skToClient)==SOCKET_ERROR )
			mpSockObj->LogLastWSAError("closesocket() in ACWinSock::OnDisconnect");
		mClients[i].skToClient = INVALID_SOCKET;
      
		// inform clients and the server about disconnect
		ACPACKAGE *pPkg = (ACPACKAGE*)mBuffer;
		pPkg->pData   = &mBuffer[g_PkgSize];
		pPkg->nLength = sizeof(UINT);
		pPkg->nType   = 2;   // ID Msg disconnecting
		pPkg->nSender = 0;   // Server
		memcpy(pPkg->pData, &mClients[i].nID, sizeof(UINT));
		SendToClients(pPkg);
      
		// feed msg into server msg-queue
		mpSockObj->FeedByHand(pPkg);

		// copy last struct to deleted field, adjust counter
		memcpy(&mClients[i], &mClients[mClCount-1], sizeof(ACCLIENT));
		mClCount--;
	}
	else 
	{
		if ( shutdown(mpSockObj->GetSocket(),0x02)==SOCKET_ERROR )
			mpSockObj->LogLastWSAError("shutdown() in ACWinSock::OnDisconnect");
		if ( closesocket(mpSockObj->GetSocket())==SOCKET_ERROR )
			mpSockObj->LogLastWSAError("closesocket() in ACWinSock::OnDisconnect");
	}

	Log("one client disconnected");
	return AC_OK;
};

