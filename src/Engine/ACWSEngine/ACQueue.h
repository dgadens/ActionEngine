//***********************************
// Classe controladora das filas para processamento
// Creator: Daniel Gadens
// Date: 05/08/2011
//********************************

#ifndef __ACQUEUE_H
#define __ACQUEUE_H

#include "EngineCommon.h"

//Dados genericos controlados pela fila
class ACQueueElem 
{
public:
	ACQueueElem(const char *pData, UINT nSize);
	~ACQueueElem(void);

	ACQueueElem  *mpNext;
	char         *mpData;
	UINT  mSize;
};


//uma fila simples FIFO
class ACQueue 
{
private:
	ACQueueElem *mpHead;
    ACQueueElem *mpTail;
    UINT  mCount;

public:
    ACQueue(void);
    ~ACQueue(void);

    void Dequeue(void);
    void Enqueue(const void *pData, unsigned int nSize);
    void Front(void *pData, bool bDequeue);

    UINT GetCount(void) { return mCount; }

    UINT GetFrontSize(void) 
    { 
        if (mpHead) 
			return mpHead->mSize;
        else 
			return 0;
    }
};

#endif