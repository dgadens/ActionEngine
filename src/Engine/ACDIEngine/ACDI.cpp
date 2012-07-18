#include <windows.h>   
#include "ACDI.h"  
#include "ACJoystick.h"

//methodo externo a classe q cria o inputdevice
HRESULT CreateInputDevice(HINSTANCE hDLL, ACInputDevice **pDevice)
{
	if (!*pDevice)
	{
		*pDevice = new ACDI(hDLL);
		return AC_OK;
	}

	return AC_FAIL;
};

//metodo externo q libera o inputdevice
HRESULT ReleaseInputDevice(ACInputDevice **pDevice)
{
	if (!*pDevice)
	{
		return AC_FAIL;
	}

	SAFE_DELETE(*pDevice);

	return AC_OK;
};

ACDI::ACDI(HINSTANCE hDLL) {
   mhDLL        = hDLL;
   mpDI         = nullptr;
   mpJoy        = nullptr;
};

ACDI::~ACDI() 
{
   Release();
};

void ACDI::Release() 
{
	SAFE_DELETE(mpJoy);

	if (mpDI) 
	{
		mpDI->Release();
		mpDI = nullptr;
	}
};

HRESULT ACDI::Init(HWND hWnd, const RECT *prcCage) 
{
	HRESULT hr;

	mActiveWnd = hWnd;

	// create main DirectInput object
	if (FAILED (hr = DirectInput8Create(mhDLL, DIRECTINPUT_VERSION, 
							IID_IDirectInput8, (void**)&mpDI,
							nullptr)) ) 
	{ 
		MessageBoxA(nullptr, "Erro no DirectInput8Create()", "ACDI Error", MB_OK | MB_ICONERROR);
		return AC_FAIL;
	} 

	// cria todos os objetos de inputdevice
	mpJoy   = new ACJoystick(mpDI, mActiveWnd);

	//se nao tiver controle plugado entao nao precisa dar erro
	if (FAILED( mpJoy->Init() )) 
	{
		SAFE_DELETE(mpJoy);
	}

	IsRunning = true;
	return AC_OK;
};

////verifica se existe joystick
//bool ACDI::HasJoystick(std::string name) 
//{
//	if (mpJoy) 
//	{
//		if (name == "") 
//			mpJoy->GetName(name);
//
//		return true;
//	}
//
//	return false;
//} 
//
////atualiza todos os dispositivos
//HRESULT ACDI::Update(void) 
//{
//	HRESULT hr;
//
//	if (!IsRunning) return AC_FAIL;
//
//	if (mpKB) 
//	{
//		if ( FAILED( hr=mpKB->Update() ) )
//			return hr;
//	}
//
//	if (mpMouse) 
//	{
//		if ( FAILED( hr=mpMouse->Update() ) )
//			return hr;
//	}
//
//	if (mpJoy) 
//	{
//		if ( FAILED( hr=mpJoy->Update() ) )
//			return hr;
//	}
//
//	return AC_OK;
//}; 
//
////se é mouse ou joystick retorna a posicao
//HRESULT ACDI::GetPosition(InputDevType idType, POINT *pPt) 
//{
//	if (idType == IDV_Mouse) 
//	{
//		mpMouse->GetPosition(pPt);
//		return AC_OK;
//	}
//	else if (idType==IDV_Joystick) 
//	{
//		if (mpJoy)
//			mpJoy->GetPosition(pPt);
//		else 
//		{
//			(*pPt).x = 0;
//			(*pPt).y = 0;
//		}
//
//		return AC_OK;
//	}
//	else 
//		return AC_FAIL;
//};
//
////retorna a intesidade do eixo
//HRESULT ACDI::GetJoyDeflection(float *pfX, float *pfY) 
//{
//   if (mpJoy)
//      return mpJoy->GetJoyDeflection(pfX, pfY);
//   else 
//   {
//      (*pfX) = 0.0f;
//      (*pfY) = 0.0f;
//      return AC_OK;
//    }
//};
//
////retorna a mudanca no cursor do mouse desde o ultimo update
//POINT ACDI::GetMouseDelta(void) 
//{ 
//	return mpMouse->GetMouseDelta(); 
//};
//
////pede pelo estado do botao
//bool ACDI::IsPressed(InputDevType idType, UINT nBtn) 
//{
//	if (idType == IDV_Mouse)
//		return mpMouse->IsPressed(nBtn);
//	else if (idType==IDV_Keyboard)
//		return mpKB->IsPressed(nBtn);
//	else if ( (idType==IDV_Joystick) && (mpJoy) )
//		return mpJoy->IsPressed(nBtn);
//	else 
//		return false;
//};
//
//bool ACDI::IsReleased(InputDevType idType, UINT nBtn) 
//{
//   if (idType == IDV_Mouse)
//      return mpMouse->IsReleased(nBtn);
//   else if (idType==IDV_Keyboard)
//      return mpKB->IsReleased(nBtn);
//   else if ( (idType==IDV_Joystick) && (mpJoy) )
//      return mpJoy->IsReleased(nBtn);
//   else 
//      return false;
//};