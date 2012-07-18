#include "ACSocketObject.h"


extern bool g_bLF;
extern int g_PkgSize;

ACSocketObject::ACSocketObject(FILE *pLOG) 
{
	IsRunning	= false;
	mskSocket   = INVALID_SOCKET;
	mpInbox     = nullptr;
	mBuffer     = nullptr;
	mpLOG		= pLOG;
}; 

ACSocketObject::~ACSocketObject(void) 
{ 
	if (IsRunning) 
	{
		Disconnect();
		IsRunning = false;
    }

	SAFE_DELETE(mpInbox);

	SAFE_DELETE_A(mBuffer);

	mskSocket = INVALID_SOCKET;
};

//cria o socket é necessario no cliente e no server (primeiro passo)
HRESULT ACSocketObject::CreateSocket(void) 
{
	// se o socket ja ta em uso fecho o desgraca
	if (mskSocket != INVALID_SOCKET) 
		Disconnect();

	mskSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mskSocket==INVALID_SOCKET) 
	{	
		Log("[ERROR]  socket() failed");
		return AC_FAIL;
	}

	mpInbox = new ACQueue();
	mBuffer = new char[65536];
	memset(mBuffer,0,65536);

	return AC_OK;
};

//faz o bind do socket a uma porta
HRESULT ACSocketObject::Bind(int nPort) 
{
	sockaddr_in saServerAddress;

	memset(&saServerAddress, 0, sizeof(sockaddr_in));
	saServerAddress.sin_family = AF_INET;
	saServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	saServerAddress.sin_port = htons(nPort);

	if ( bind(mskSocket, (sockaddr*)&saServerAddress, 	sizeof(sockaddr)) == SOCKET_ERROR) 
	{
		LogLastWSAError("bind() failed");
		Disconnect();
		return AC_FAIL;
	}

	return AC_OK;
};

//fica escutando o socket conectado
HRESULT ACSocketObject::Listen(void) 
{
	if (listen(mskSocket, 32) != 0) 
	{
		Log("[ERROR]  listen() failed");
		return AC_FAIL;
    }
	IsRunning = true;

	return AC_OK;
};

//aceita a conexao de uma conexao na fila
HRESULT ACSocketObject::Accept(SOCKET *skToNewClient) 
{
	sockaddr_in saClientAddress;
	int         nClientSize = sizeof(sockaddr_in);

	(*skToNewClient) = accept(mskSocket, (sockaddr*)&saClientAddress, &nClientSize);
	if ((*skToNewClient) == INVALID_SOCKET) 
	{
		Log("[ERROR]  accept() failed");
		return AC_FAIL;
	}
	
	Log("client from %d.%d.%d.%d now succesfully connected to us",
		saClientAddress.sin_addr.S_un.S_un_b.s_b1,
		saClientAddress.sin_addr.S_un.S_un_b.s_b2,
		saClientAddress.sin_addr.S_un.S_un_b.s_b3,
		saClientAddress.sin_addr.S_un.S_un_b.s_b4);

	return AC_OK;
};

//conecta os atributos do socket para informar o server sobre o endereco/nome e a porta
HRESULT ACSocketObject::Connect(char *chServer, int nPort) 
{
	sockaddr_in saServerAddress;
	LPHOSTENT   pHost=nullptr;

	// try to find the server by address or name
	memset(&saServerAddress,0,sizeof(sockaddr_in));
	saServerAddress.sin_port        = htons(nPort);
	saServerAddress.sin_family      = AF_INET;
	saServerAddress.sin_addr.s_addr = inet_addr(chServer);

	if (saServerAddress.sin_addr.s_addr==INADDR_NONE) 
	{
		pHost = gethostbyname(chServer);
		if (pHost != nullptr) 
		{
			saServerAddress.sin_addr.s_addr = 
			((LPIN_ADDR)pHost->h_addr)->s_addr;
		}
		else 
		{
			Log("[ERROR]  server \"%s\" not found", chServer);
			return AC_FAIL;
		}
	}

	// connect the socket to the server address
	if (connect(mskSocket, (sockaddr*)&saServerAddress, sizeof(sockaddr)) == SOCKET_ERROR) 
	{
		LogLastWSAError("connect() in ACSocketObject::Connect failed");
		Disconnect();
		return AC_FAIL;
	}

	// now the SocketObject is active
	IsRunning = true;

	return AC_OK;
};

//desconectar do socket ativo
void ACSocketObject::Disconnect(void) 
{
	if (mskSocket != INVALID_SOCKET) 
	{
		shutdown(mskSocket, 0x02);
		closesocket(mskSocket);
		mskSocket = INVALID_SOCKET;
		Log("socket closed");
    }
};


//envia os dados encapsulado para o winsock
int ACSocketObject::Send(const char *pPkg, UINT nSize) 
{
	UINT nSent=0;
	UINT n=0;

	while (nSent < nSize) 
	{
		n = send(mskSocket, pPkg+nSent, nSize-nSent, 0);
		if (n==SOCKET_ERROR) 
		{
			LogLastWSAError("send() in ACSocketObject");
			return n;
		}
		else 
			nSent += n;
	}

	return nSent;
};

int ACSocketObject::Send(const char *pPkg, UINT nSize, SOCKET skReceiver) 
{
	UINT nSent=0;
	UINT n=0;

	while (nSent < nSize) 
	{
		n = send(skReceiver, pPkg+nSent, nSize-nSent, 0);
		if (n==SOCKET_ERROR) 
		{
			LogLastWSAError("send() in ACSocketObject");
			return n;
		}
		else 
			nSent += n;
	}

	return nSent;
};

//recebe os dados do socket e reage a mensagem FD_READ
HRESULT ACSocketObject::Receive(SOCKET sk) 
{
	HRESULT hr         = AC_OK;
	UINT    nSize      = 65536;   // tamanho maximo para ler em uma chamada
	UINT    nBytesRead = 0;       // byte atual lido em uma chamada
	UINT    nReadHead  = 0;       // lendo a posicao tal no mBuffer
	UINT    n          = 0;       // dado no buffer
	bool    bDone      = false;   // pronto
  
	ACPACKAGE	*pPkg          = nullptr;
	UINT        nPkgSizeTotal	= 0;

	// lendo mais que 65536 bytes a cada chamada e vai fazendo o loop ate acabar
	while (!bDone) 
	{
		nBytesRead = recv(sk, &mBuffer[n], nSize-n, 0);

		if (nBytesRead == SOCKET_ERROR) 
		{
			int WSAError = WSAGetLastError();

			// ignora erros nao criticos
			if ( (WSAError != WSAEMSGSIZE) && (WSAError != WSAEWOULDBLOCK) ) 
			{
				LogLastWSAError("recv() in ACSocketObject::Receive");
				hr = AC_FAIL;
				bDone = true;
				break;
			}
		}

		// novos nBytesRead bytes no mBuffer entao enfilera
		if (nBytesRead <= 0) 
		{
			bDone = true;
		}
		else 
		{
			// take care of old data in the buffer
			nBytesRead += n;

			// loop ate achar outro header completo
			// no buffer, verifica se existe um pacote
			// vai dividindo
			while ( (nBytesRead - nReadHead) > g_PkgSize ) 
			{
				// mask next chunk of data as ACPACKAGE
				pPkg = (ACPACKAGE*)&mBuffer[nReadHead];
				pPkg->pData = &mBuffer[nReadHead] + g_PkgSize;

				// qual o tamanho total do pacote?
				nPkgSizeTotal = g_PkgSize + pPkg->nLength;

				// recebemos todo o pacote?
				if ( (nBytesRead-nReadHead) >= (nPkgSizeTotal) ) 
				{
					mpInbox->Enqueue(pPkg, nPkgSizeTotal);
					nReadHead += nPkgSizeTotal;
				}
				else // nao apenas a parte do pacote que vai voltar para o recv
				{
					// copia ó pacote dividido para o inicio do buffer
					memcpy(mBuffer, &mBuffer[nReadHead], nBytesRead-nReadHead);
					n = nBytesRead-nReadHead;
					break;
				}
			} 

			// se recebeu todos os dados que estavam esperando blz
			if (nBytesRead < nSize) bDone = true;
		}
	}
	return hr;
};

//pegas o proximo elemento esperando para ser processado
HRESULT ACSocketObject::GetNextPkg(ACPACKAGE *pPkg) 
{
   // is there a package at all?
   if (mpInbox->GetCount() > 0) 
   {
		// stream serialized data into our buffer
		mpInbox->Front(mBuffer, true);

		// fill it into the structure
		memcpy(pPkg, mBuffer, g_PkgSize-sizeof(PVOID));
		memcpy(pPkg->pData, mBuffer + g_PkgSize, pPkg->nLength);

		return AC_OK;
    }

	return AC_FAIL;
};

//coloca o pacote na caixa de entrada do socket no braco
void ACSocketObject::FeedByHand(ACPACKAGE *pPkg) 
{
	int nPkgSizeTotal = g_PkgSize + pPkg->nLength;
	mpInbox->Enqueue(pPkg, nPkgSizeTotal);
};


void ACSocketObject::Log(char* message, ...)
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
 * Get the last WSA error an put it out to the log file together
 * with the given string an prefix "[ERROR]  ".
 */
void ACSocketObject::LogLastWSAError(const char *error) 
{
   int n=WSAGetLastError();

   if (n==WSANOTINITIALISED)
      Log("[ERROR]  %s | WSANOTINITIALISED", error);
   else if (n==WSAENETDOWN)
      Log("[ERROR]  %s | WSAENETDOWN", error);
   else if (n==WSAEADDRINUSE)
      Log("[ERROR]  %s | WSAEADDRINUSE", error);
   else if (n==WSAEINTR)
      Log("[ERROR]  %s | WSAEINTR", error);
   else if (n==WSAEINPROGRESS)
      Log("[ERROR]  %s | WSAEINPROGRESS", error);
   else if (n==WSAEALREADY)
      Log("[ERROR]  %s | WSAEALREADY", error);
   else if (n==WSAEADDRNOTAVAIL)
      Log("[ERROR]  %s | WSAEADDRNOTAVAIL", error);
   else if (n==WSAEAFNOSUPPORT)
      Log("[ERROR]  %s | WSAEAFNOSUPPORT", error);
   else if (n==WSAECONNREFUSED)
      Log("[ERROR]  %s | WSAECONNREFUSED", error);
   else if (n==WSAEFAULT)
      Log("[ERROR]  %s | WSAEFAULT", error);
   else if (n==WSAEINVAL)
      Log("[ERROR]  %s | WSAEINVAL", error);
   else if (n==WSAEISCONN)
      Log("[ERROR]  %s | WSAEISCONN", error);
   else if (n==WSAENETUNREACH)
      Log("[ERROR]  %s | WSAENETUNREACH", error);
   else if (n==WSAENOBUFS)
      Log("[ERROR]  %s | WSAENOBUFS", error);
   else if (n==WSAENOTSOCK)
      Log("[ERROR]  %s | WSAENOTSOCK", error);
   else if (n==WSAENOTCONN)
      Log("[ERROR]  %s | WSAENOTCONN", error);
   else if (n==WSAETIMEDOUT)
      Log("[ERROR]  %s | WSAETIMEDOUT", error);
   else if (n==WSAEWOULDBLOCK)
      Log("[ERROR]  %s | WSAEWOULDBLOCK", error);
   else if (n==WSAEACCES)
      Log("[ERROR]  %s | WSAEACCES", error);
   else if (n==WSAEMFILE)
      Log("[ERROR]  %s | WSAEMFILE", error);
   else if (n==WSAEOPNOTSUPP)
      Log("[ERROR]  %s | WSAEOPNOTSUPP", error);
   else
      Log("[ERROR]  %s | unknown error code", error);
};
