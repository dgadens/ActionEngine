
#ifndef __ACDIDEVICE_H
#define __ACDIDEVICE_H

#include "EngineCommon.h"
#include <dinput.h>

class ACDIDevice 
{
protected:
	virtual HRESULT GetData(InputDevType Type, void *pData, DWORD *dwNum);

	LPDIRECTINPUT8        mpDI;
	LPDIRECTINPUTDEVICE8  mpDevice;

	HWND                  mhWnd;
	long                  mlX;
	long                  mlY;

public:
	ACDIDevice(void) { /*nothing*/; }
	virtual ~ACDIDevice(void) { /*nothing*/; }

	// base class methods
	virtual void    Create(LPDIRECTINPUT8, HWND);
	virtual void    Release(void);
	virtual HRESULT CrankUp(REFGUID rguid, LPCDIDATAFORMAT lpdf);
      
	// Accessor-Methods
	virtual void    GetPosition(POINT *pPoint)
		{ (*pPoint).x = mlX; (*pPoint).y = mlY; }

	// virtual methods for heirs
	virtual HRESULT Init(void)=0;
	virtual HRESULT Update(void)=0;
};

#endif