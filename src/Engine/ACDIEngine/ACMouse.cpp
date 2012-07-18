#include "ACMouse.h"


ACMouse::ACMouse(LPDIRECTINPUT8 pDI, HWND hWnd)
{
	Create(pDI, hWnd);
};

ACMouse::~ACMouse()
{
	Release();
};

HRESULT ACMouse::Init()
{
	// limpa as estruturas do mouse
	memset(mbPressed,  0, sizeof(bool)*3);
	memset(mbReleased, 0, sizeof(bool)*3);
	mlX = mlY = 0;

	mrcCage.left   = -1;
	mrcCage.right  = -1;
	mrcCage.top    = -1;
	mrcCage.bottom = -1;

	//setup do mouse
	if (FAILED(CrankUp(GUID_SysMouse, &c_dfDIMouse)))
		return AC_FAIL;

	// ativa eventos de notificacao
	if (!(mhEvent = CreateEvent(NULL, FALSE, FALSE, NULL))) 
	{
		MessageBoxA(NULL, "Erro ao inicializar o mouse.", "ACMouse Error", MB_OK);
		return AC_FAIL;
	}

	if (FAILED( mpDevice->SetEventNotification(mhEvent))) 
	{
		MessageBoxA(NULL, "Erro ao inicializar evento de notificacao.", "ACMouse Error", MB_OK);
		return AC_FAIL;
	}

	// create buffered input for the mouse
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = BUFFER_SIZE;

	if (FAILED( mpDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))) 
	{
		MessageBoxA(NULL, "Erro SetProperty(Mouse) falhou.", "ACMouse Error", MB_OK);
		return AC_FAIL;
	}

	// acquire the device to make it work
	mpDevice->Acquire();
	return AC_OK;
};

HRESULT ACMouse::Update()
{
	DIDEVICEOBJECTDATA	od[BUFFER_SIZE];
	DWORD				dwNumElem = BUFFER_SIZE;

	bool bCage = (mrcCage.right != -1);

	mDelta.x = mDelta.y = 0;
      
	// tenta pegar os dados do mouse
	if (FAILED(GetData(IDV_Mouse, &od[0], &dwNumElem))) 
	{
		MessageBoxA(NULL, "Erro GetData(Mouse) falhou.", "ACMouse Error", MB_OK);
		return AC_FAIL;
	}

	// clear those
	mbReleased[0] = mbReleased[1] = mbReleased[2] = false;

	// now we have 'dwNumElem' of mouse events
	for (DWORD i=0; i<dwNumElem; i++) 
	{
		switch (od[i].dwOfs) 
		{
			// MOVEMENT
			case DIMOFS_X: 
				{
					mlX += od[i].dwData;
					mDelta.x = od[i].dwData;

					if (bCage & (mlX < mrcCage.left))
						mlX = mrcCage.left;
					else if (bCage & (mlX > mrcCage.right))
						mlX = mrcCage.right;
				} break;

			case DIMOFS_Y: 
				{
					mlY += od[i].dwData;
					mDelta.y = od[i].dwData;
            
					if (bCage & (mlY < mrcCage.top))
						mlY = mrcCage.top;
					else if (bCage & (mlY > mrcCage.bottom))
						mlY = mrcCage.bottom;
				} break;

			// BUTTON STATES
			case DIMOFS_BUTTON0: 
				{
					if (od[i].dwData & 0x80) 
					{
						mbPressed[0] = true;
					}
					else 
					{
						if (mbPressed[0])
							mbReleased[0] = true;

						mbPressed[0] = false;
					}
				} break;

			case DIMOFS_BUTTON1: 
				{
					if (od[i].dwData & 0x80)
						mbPressed[1] = true;
					else 
					{
						if (mbPressed[1])
							mbReleased[1] = true;

						mbPressed[1] = false;
					}
				} break;

			case DIMOFS_BUTTON2: 
				{
					if (od[i].dwData & 0x80)
						mbPressed[2] = true;
					else 
					{
						if (mbPressed[2])
							mbReleased[2] = true;

						mbPressed[2] = false;
					}
				} break;
		}; 
	} 

	return AC_OK;
};