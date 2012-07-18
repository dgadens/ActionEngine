#include <stddef.h>        // #def NULL
#include <memory.h>        // memcpy()
#include "ACQueue.h"

// aloca a memoria e copia os dados
ACQueueElem::ACQueueElem(const char *pData, unsigned int nSize) 
{
	mpData = nullptr;
	mpNext = nullptr;
	mpData = new char[nSize];
	mSize = nSize;
	memcpy(mpData, pData, nSize);
};

//libera a memoria
ACQueueElem::~ACQueueElem(void) 
{
	SAFE_DELETE_A(mpData);

	mpNext = nullptr;
};

ACQueue::ACQueue(void) 
{
   mpTail = nullptr;
   mpHead = nullptr;
   mCount = 0;
};


// libera todos os elementos da file e reseta os membros
ACQueue::~ACQueue(void) 
{
	while (mpHead) Dequeue();
	mpTail = nullptr;
	mpHead = nullptr;
	mCount = 0;
};

// delete the head element
void ACQueue::Dequeue(void) 
{
	ACQueueElem *pTemp;

	// already empty
	if (mCount == 0) return;

	// exactly one element
	else if (mCount == 1) 
	{
		SAFE_DELETE(mpHead);
		mpTail = nullptr;
	}
	else 
	{
		pTemp = mpHead;
		mpHead = mpHead->mpNext;
		delete pTemp;
	}

	mCount--;
};

// coloca um elemento novo no final da cauda
void ACQueue::Enqueue(const void *pData, unsigned int nSize) 
{
	ACQueueElem *pNew = new ACQueueElem((const char*)pData, nSize);

	// esse é o primeiro pacote
	if (mCount == 0) 
	{
		mpHead = pNew;
		mpTail = pNew;
	}
	else if(mCount == 1) // esse é o segundo pacote
	{
		mpHead->mpNext = pNew;
		mpTail = pNew;
	}
	else 
	{
		mpTail->mpNext = pNew;
		mpTail = pNew;
	}

	mCount++;
};

//pega o dado do elemento do cabecalho e deleta da fila
void ACQueue::Front(void *pData, bool bDequeue) 
{
	if (pData) 
	{
		if (mpHead) 
		{
			memcpy(pData, mpHead->mpData, mpHead->mSize);
        }
    }

	if (bDequeue) Dequeue();	
};