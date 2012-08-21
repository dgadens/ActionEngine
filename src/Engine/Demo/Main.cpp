#include <windows.h>
#include <string.h>
#include "Resource.h"
#include "EngineMain.h"
#include "EngineCommon.h"
#include "EngineMath.h"
#include "Globals.h"
#include "GameInput.h"
#include "Game.h"

//declaracao dos atributos globais
HINSTANCE               HInst = nullptr;
HWND                    HWnd = nullptr;
ACRenderer*				pRenderer = nullptr;
ACInput*				pInput = nullptr;
ACNetwork*				pNetwork = nullptr;
ACAudio*				pAudio = nullptr;

ACContentManager*		pContentManager = nullptr;
Game*					pGame = nullptr;

std::string				Messages;

//declaracao dos metodos
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Update();
void Render();
HRESULT EngineStartup();
void CleanupDevice();

//ponto de entrada do programa usando para renderizar
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

	if (FAILED(EngineStartup()))
	{
		CleanupDevice();
		return 0;
	}

#if DEBUG
	ACGlobals::ENABLE_LOG = true;
#else
	ACGlobals::ENABLE_LOG = false;
#endif
                                
    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			pGame->Update();
			pGame->Draw();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}

//COLOCAR A LOGICA AQUI
void Update()
{
	if (Globals::IsRunning)
	{
	//	if (Globals::g_pNetworkDevice!=nullptr)
	//	{
	//		if (Globals::g_pNetworkDevice->IsRunning)
	//		{
	//			while (Globals::g_pNetworkDevice->IsPkgWaiting()) 
	//			{
	//				ACPACKAGE pack = { 0, 0, 0, 0 };
	//				UINT       nNID=0;

	//				BYTE pBuffer[MAX_PKG_SIZE];
	//				memset(pBuffer, 0, MAX_PKG_SIZE);
	//				pack.pData = pBuffer;

	//				if (FAILED(Globals::g_pNetworkDevice->GetNextPkg(&pack))) 
	//				{
	//					MessageBoxA(nullptr, "ERRO RECEBENDO DADOS", "SEI LA", MB_OK);
	//				}
	//				else 
	//				{
	//					// recebe o id do server
	//					if (pack.nType == 0) 
	//					{
	//						Globals::g_NetID = *((UINT*)pack.pData);
	//					}
	//					else if (pack.nType == 1) //avisa q um cliente se conecto
	//					{
	//						MessageBoxA(nullptr, "new client has connected", "SEI LA", MB_OK);
	//						//sprintf(temp, "new client has connected <%d>", *((UINT*)msgPkg.pData));
	//						//SendMessage(hLbx,LB_ADDSTRING,0,(LPARAM)temp);
	//					}
	//					else if (pack.nType == 2) // avisa que um cliente se desconecto
	//					{
	//						MessageBoxA(nullptr, "new client has disconnected", "SEI LA", MB_OK);
	//	/*				   sprintf(temp, "client <%d> has disconnected", *((UINT*)msgPkg.pData));
	//						SendMessage(hLbx,LB_ADDSTRING,0,(LPARAM)temp);*/
	//					}
	//					else if (pack.nType == 3) //recebeu uma mensagem pelo chat
	//					{
	//						//Vector3 v = *((Vector3*)pack.pData);
	//						g_Messages.clear();
	//						g_Messages.append((char*)pack.pData);
	//						g_Messages.append("\n");
	//					
	//						/*SendMessage(hLbx,LB_ADDSTRING,0,(LPARAM)msgPkg.pData);
	//						if (g_nmd==NMD_SERVER) {
	//							g_pDevice->SendToClients(&msgPkg);
	//							}*/
	//					}
	//					/*int n=SendMessage(hLbx,LB_GETCOUNT,0,0);
	//					SendMessage(hLbx,LB_SETCURSEL,(WPARAM)n-1,0);*/
	//				} // while msg incoming
	//			}
	//		}
	//	}
	}
}

void RenderHelperTexts()
{
	//g_pSpriteBatch->BeginRender(ACBLENDSTATE::ACBS_NonPremultiplied);
	//
	////renderiza os fps
	//std::string fpsText("FPS: ");
	//fpsText.append(GenericTools::IntToString(ACTimeControl::GetFPS()));
	//fpsText.append("\n\r");
	//fpsText.append("Particles emitter: ");

	//g_pSpriteFont->SetText(fpsText, fpsText.size());
	//g_pSpriteBatch->Render(g_pSpriteFont, &Vector2(-Globals::g_MainWndWidth /2 , +Globals::g_MainWndHeight / 2 - 20), &Vector2(1.0f), 0.0f, &Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//g_pSpriteBatch->EndRender();
}

void RenderChatTexts()
{
	//g_pSpriteBatch->BeginRender(ACBLENDSTATE::ACBS_NonPremultiplied);
	//g_pChatSpriteFont->SetText(g_Messages, g_Messages.size());
	//g_pSpriteBatch->Render(g_pChatSpriteFont, &Vector2(-Globals::g_MainWndWidth /2 , 0), &Vector2(1.0f), 0.0f, &Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//g_pSpriteBatch->EndRender();
}


//registra a classe e cria a janela
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_DEMO );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"DemoWindow";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_DEMO );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    HInst = hInstance;
    RECT rc = { 0, 0, Globals::MainWndWidth, Globals::MainWndHeight };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    HWnd = CreateWindow( L"DemoWindow", L"DemoProject", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                           nullptr );
    if( !HWnd )
        return E_FAIL;

	Globals::MainWnd = HWnd;

    ShowWindow( HWnd, nCmdShow );

    return S_OK;
}


//é chamado toda vez q a aplicacao recebe uma mensagem
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
	
    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_MOUSEMOVE:
			Globals::pMouse->SetPosition(LOWORD (lParam), HIWORD (lParam));
			break;
		case WM_LBUTTONDOWN:
			Globals::pMouse->SetButtonDown(ACMOUSEBUTTON::ACMOUSE_LEFT);
			break;
		case WM_RBUTTONDOWN:
			Globals::pMouse->SetButtonDown(ACMOUSEBUTTON::ACMOUSE_RIGHT);
			break;
		case WM_MBUTTONDOWN:
			Globals::pMouse->SetButtonDown(ACMOUSEBUTTON::ACMOUSE_MIDDLE);
			break;

		case WM_LBUTTONUP:
			Globals::pMouse->SetButtonUp(ACMOUSEBUTTON::ACMOUSE_LEFT);
			break;
		case WM_RBUTTONUP:
			Globals::pMouse->SetButtonUp(ACMOUSEBUTTON::ACMOUSE_RIGHT);
			break;
		case WM_MBUTTONUP:
			Globals::pMouse->SetButtonUp(ACMOUSEBUTTON::ACMOUSE_MIDDLE);
			break;

		case WM_CHAR:
			//if (Globals::ShowConsole)
			//{
			//	if (wParam != 39) //ASC Apostrofo
			//		Globals::pConsole->SetCharacter(wParam);
			//}
			break;
		case WM_KEYDOWN:
			//if (wParam == 192) //VK Apostrofo
			//{
			//	Globals::pConsole->Clear();
			//	Globals::ShowConsole = !Globals::ShowConsole;
			//}

			//if (!Globals::ShowConsole)
				Globals::pKeyboard->SetKeyDown((ACKEYS)wParam);
			break;
		case WM_KEYUP:
			//if (!Globals::ShowConsole)
				Globals::pKeyboard->SetKeyUp((ACKEYS)wParam);
			break;

		case WM_MENUCHAR:
			// Nao solta um beep quando da alt-enter
			return MAKELRESULT(0, MNC_CLOSE);

		// Pega essa mensagem para nao deixar a janela com menos de 200x200
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

		case WM_ACTIVATE:
			if( LOWORD(wParam) == WA_INACTIVE )
				pGame->Active(FALSE);
			else
				pGame->Active(TRUE);
			return 0;

		// WM_ENTERSIZEMOVE is sent when the user grabs the resize bars.
		case WM_ENTERSIZEMOVE:
			pGame->Active(FALSE);
			return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			RECT rcWnd;
			GetClientRect(HWnd, &rcWnd);
			Globals::MainWndWidth = rcWnd.right - rcWnd.left;
			Globals::MainWndHeight = rcWnd.bottom - rcWnd.top;

			pGame->ReziseWindow(Globals::MainWndWidth, Globals::MainWndHeight);
			pGame->Active(TRUE);
			return 0;

		// network msg to be handled
		case WM_ACSERVER: case WM_ACCLIENT: 
			{
				HRESULT hr = Globals::pNetworkDevice->MsgProc(wParam, lParam);
				if (FAILED(hr)) 
				{
					//FUDEU DESCONECTAR ESSA PORRA
					//g_MyGame.Done();
					PostQuitMessage(0);
				}
			}
			break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


HRESULT EngineStartup()
{
	RECT rcWnd;

	int x = 0, y = 0;

	#pragma region CRIA GRAPHICSDEVICE
	//*** CRIA O PONTEIRO PARA O GRAPHICSDEVICE ***
	//cria o objeto renderizador
	pRenderer = new ACRenderer(HInst);
	if (FAILED(pRenderer->CreateDevice("ACD3DEngine.dll")))
	{
		MessageBoxA(nullptr, "Loading ACD3DEngine.dll from lib failed.", "ACEngine - error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	Globals::pRenderDevice = pRenderer->GetDevice();
	if (Globals::pRenderDevice == nullptr) 
		return E_FAIL;
	if (FAILED(Globals::pRenderDevice->Init(HWnd, false, true)))
		return AC_FAIL;
	//ativa a janela principal
	Globals::pRenderDevice->SetActiveViewport(HWnd);
	#pragma endregion

	#pragma region CRIA INPUTDEVICE
	pInput = new ACInput(HInst);
	if (FAILED(pInput->CreateDevice("ACDIEngine.dll")))
	{
		MessageBoxA(nullptr, "Loading ACDIEngine.dll from lib failed.", "ACEngine - error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	Globals::pInputDevice = pInput->GetDevice();
	if (Globals::pInputDevice == nullptr) 
		return E_FAIL;
	pInput->CreateMouse();
	Globals::pMouse = pInput->GetMouse();
	pInput->CreateKeyboard();
	Globals::pKeyboard = pInput->GetKeyboard();
	GameInput::Init(Globals::pKeyboard, Globals::pMouse);
	#pragma endregion

	#pragma region CRIA NETWORKDEVICE
	//pNetwork = new ACNetwork(HInst);
	//if (FAILED(pNetwork->CreateDevice("ACWSEngine.dll")))
	//{
	// 	MessageBoxA(nullptr, "Loading ACWSEngine.dll from lib failed.", "ACEngine - error", MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}
	//Globals::pNetworkDevice = pNetwork->GetDevice();
	//if (Globals::pNetworkDevice == nullptr) 
	//	return E_FAIL;
	#pragma endregion

	#pragma region CRIA AUDIO DEVICE
	pAudio = new ACAudio(HInst);
	if (FAILED(pAudio->CreateDevice("ACOpenALEngine.dll")))
	{
		MessageBoxA(nullptr, "Loading ACOpenALEngine.dll from lib failed.", "ACEngine - error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	Globals::pAudioDevice = pAudio->GetDevice();
	if (Globals::pAudioDevice == nullptr) 
		return E_FAIL;
	if (FAILED(Globals::pAudioDevice->Init(HWnd, true)))
		return AC_FAIL;

	//configura e carrega alguns sons
	Globals::pAudioDevice->SetListener(Vector3(0,0,0),Vector3(0,0,0),Vector3(0,0,0),Vector3(0,0,0));
	#pragma endregion

	#pragma region CRIA O CONTENT MANAGER
	pContentManager = new ACContentManager(Globals::pRenderDevice, Globals::pAudioDevice);
	#pragma endregion

	#pragma region CRIA O GAME
	pGame = new Game(Globals::pRenderDevice, pContentManager);
	pGame->Initialize();
	pGame->Active(TRUE);
	pGame->LoadContents();
	#pragma endregion

	return AC_OK;	
}

//mata todos os bojetos da memoria
void CleanupDevice()
{
	//remover nesta ordem pq senao ele tenta remover os conteudos depois de remover o ponteiro do contentmanager ai fudeu
	SAFE_DELETE(pGame);
	SAFE_DELETE(pContentManager);
	SAFE_DELETE(pRenderer);
	SAFE_DELETE(pInput);
	SAFE_DELETE(pNetwork);
	SAFE_DELETE(pAudio);
}