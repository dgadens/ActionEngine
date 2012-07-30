#include "ACD3D10.h"

//methodo externo a classe q cria o gdevice
HRESULT CreateRenderDevice(HINSTANCE hDLL, ACRenderDevice **pDevice)
{
	if (!*pDevice)
	{
		*pDevice = new ACD3D10(hDLL);
		return AC_OK;
	}

	return AC_FAIL;
};

//metodo externo q libera o gdevice
HRESULT ReleaseRenderDevice(ACRenderDevice **pDevice)
{
	if (!*pDevice)
	{
		return AC_FAIL;
	}

	SAFE_DELETE(*pDevice);
	return AC_OK;
};

ACD3D10::ACD3D10(HINSTANCE hDLL)
{
	mpLOG = nullptr;

	mpCurrentVertexBuffer = nullptr;
	mpCurrentVertexShader = nullptr;
	mpCurrentPixelShader = nullptr;
	mpCurrentGeometryShader = nullptr;
	mpVManager = nullptr;

	//seta todos os slots de textura pra nulo
	for (int i = 0; i < 128; i++)
		mpCurrentTexture[i] = nullptr;

	ZeroMemory( &mVSCBPerFrameData, sizeof ( VS_CB_PERFRAME ));
	ZeroMemory( &mVSCBPerModelData, sizeof ( VS_CB_PERMODEL ));
    ZeroMemory( &mVSCBPerPassData , sizeof ( VS_CB_PERPASS ));
	ZeroMemory( &mPSCBMaterialData, sizeof ( PS_CB_MATERIAL ));
};

ACD3D10::~ACD3D10()
{
	Release();
};

#pragma region Initialization

HRESULT ACD3D10::Init(HWND hWnd, BOOL enableVSync, BOOL log)
{
	HRESULT hr = AC_OK;

	if (log)
		mpLOG = fopen("RenderLOG.txt", "w");

	Log("Begin Init");

	GeometryShaderSupport = true;

	//cria o retangulo para renderizacao
	RECT rc;
	GetClientRect( hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	if (FAILED(CreateGraphicsDevice(width, height)))
		return AC_FAIL;

	if (FAILED(CreateConstantBuffers()))
		return AC_FAIL;

	if (FAILED(AddViewport(hWnd, enableVSync)))
		return AC_FAIL;

	//inicializa os estados
	mCurrentShadeMode			= ACSHADEMODE::ACSM_TriangleList;
	
	//inicializa com o blend padrao
	mCurrentBlendState			= ACBLENDSTATE::ACBS_Default;	
	ACD3D10Configurations::DefineBlendStateDefault();

	//inicializa escrevendo no zbuffer
	mCurrentDepthBufferState	= ACDEPTHBUFFERSTATE::ACDBS_WriteEnable;	
	ACD3D10Configurations::DefineDepthStencilStateEnableDepthBuffer();

	//inicializa como solido anti-horario
	mCurrentRasterizationState	= ACRASTERIZESTATE::ACRS_SolidCullCCW;
	ACD3D10Configurations::DefineRasterizeStateSolidCullCCW();

	//inicializa todos os samplers states 16 o caso do d3d10
	for (int i = 0; i < 16; i++)
	{
		mCurrentSamplerState[i] = ACSAMPLERSTATE::ACSS_Bilinear_Wrap;
		ACD3D10Configurations::DefineSamplerStateBilinearWrap(i);
	}

	//cria o vertexmanager
	mpVManager = new ACD3D10VertexManager(this, ACD3D10Globals::G_pD3dDevice, MAX_VERTICES_IN_BUFFER, MAX_INDICES_IN_BUFFER, mpLOG);
	ACD3D10Globals::G_pVertexManager = static_cast<ACD3D10VertexManager*>(mpVManager);
	
	Log("Init Success");

    return hr;
};

HRESULT ACD3D10::CreateGraphicsDevice(int width, int height)
{
	HRESULT hr;

	//inicializa membros
	mDriverType = D3D10_DRIVER_TYPE_NULL;
	ACD3D10Globals::G_pD3dDevice = nullptr;

	//tipo de dispositivo debug ou release
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	D3D10_DRIVER_TYPE driverTypes[] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	//********************************
	//  cria do dispositivo grafico
	//********************************
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		mDriverType = driverTypes[driverTypeIndex];
		//passa null no adapter ai ele cria automatico, depois eu recupero para pegar algumas configuracoes
		hr = D3D10CreateDevice( nullptr, mDriverType, nullptr, createDeviceFlags,
								D3D10_SDK_VERSION, &ACD3D10Globals::G_pD3dDevice );
		if( SUCCEEDED( hr ) )
		{
			Log("Create device success.");
			break;
		}
	}
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create device error. D3D10CreateDevice()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create device error. D3D10CreateDevice()");
		return hr;
	}
	//*********************************
	// fim da criacao do dispositivo grafico
	//*********************************

	IDXGIAdapter* pDXGIAdapter = ACD3D10Tools::GetDXGIAdapter();
	IDXGIFactory* pDXGIFactory = ACD3D10Tools::GetDXGIFactory();

	//usando os objetos acima ele retorna algumas configuracoes
	IDXGIOutput*  pAdapterOutput;
	unsigned int numModes, i, stringLength;
	DXGI_MODE_DESC* pDisplayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;

	// Enumerate the primary adapter output (monitor).
	hr = pDXGIAdapter->EnumOutputs(0, &pAdapterOutput);
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] EnumOutputs. D3D10CreateDevice()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	hr = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] GetDisplayModeList", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	pDisplayModeList = new DXGI_MODE_DESC[numModes];
	if(!pDisplayModeList)
		return AC_FAIL;

	// Now fill the display mode list structures.
	hr = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList);
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] GetDisplayModeList", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(pDisplayModeList[i].Width == (unsigned int)width)
		{
			if(pDisplayModeList[i].Height == (unsigned int)height)
			{
				ACD3D10Globals::G_Numerator = pDisplayModeList[i].RefreshRate.Numerator;
				ACD3D10Globals::G_Denomerator = pDisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	hr = pDXGIAdapter->GetDesc(&adapterDesc);
	if(FAILED(hr))
		return hr;

	// Store the dedicated video card memory in megabytes.
	VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	Log("VideoCardMemory: %i (in megabytes).", VideoCardMemory);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, VideoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		Log("[ERROR] Get videocarddescription.");
		return AC_FAIL;
	}

	// Release the display mode list.
	SAFE_DELETE_A(pDisplayModeList);

	// Release the adapter output.
	pAdapterOutput->Release();
	pAdapterOutput = nullptr;

	// Release the adapter.
	pDXGIAdapter->Release();
	pDXGIAdapter = nullptr;

	// Release the factory.
	pDXGIFactory->Release();
	pDXGIFactory = nullptr;

	return AC_OK;
};

HRESULT ACD3D10::CreateConstantBuffers()
{
	HRESULT hr = AC_OK;

    // Cria o constant buffer para o VS_CB_PERFRAME
    D3D10_BUFFER_DESC cbDescVS;
	ZeroMemory( &cbDescVS, sizeof (D3D10_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( VS_CB_PERFRAME );
    cbDescVS.Usage = D3D10_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3D10Globals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpVSCBPerFrame );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create VS_CB_PERFRAME. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create VS_CB_PERFRAME");
		return hr;
	}

	// Cria o constant buffer para o VS_CB_PERMODEL
	ZeroMemory( &cbDescVS, sizeof (D3D10_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( VS_CB_PERMODEL );
    cbDescVS.Usage = D3D10_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3D10Globals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpVSCBPerModel );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create VS_CB_PERMODEL. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create VS_CB_PERMODEL");
		return hr;
	}

	// Cria o constant buffer para o VS_CB_PERPASS
	ZeroMemory( &cbDescVS, sizeof (D3D10_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( VS_CB_PERPASS );
    cbDescVS.Usage = D3D10_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3D10Globals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpVSCBPerPass );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create VS_CB_PERPASS. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create VS_CB_PERPASS");
		return hr;
	}

	// Cria o constant buffer para o PS_CB_MATERIAL
	ZeroMemory( &cbDescVS, sizeof (D3D10_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( PS_CB_MATERIAL );
    cbDescVS.Usage = D3D10_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3D10Globals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpPSCBMaterial );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create PS_CB_LIGHT. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create PS_CB_LIGHT");
		return hr;
	}

	return hr;
};

HRESULT ACD3D10::AddViewport(HWND hWnd, BOOL enableVSync)
{
	HRESULT hr;
	VSyncEnable = enableVSync;

	Log("Add viewport");

	//cria o retangulo para renderizacao
	RECT rc;
	GetClientRect( hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	//adiciona os objetos necessarios para cada vp
	ACD3D10VpComponents* vpComponent = new ACD3D10VpComponents();

	//pega o factory
	IDXGIFactory* pDXGIFactory = ACD3D10Tools::GetDXGIFactory();

#pragma region CREATE SWAP CHAIN
	IDXGISwapChain* pSwapChain;

	//define o rendertargetview, define o buffer e coloca como backbuffer em cada swapchain
	//cria os swpachains para cada janela
	DXGI_SWAP_CHAIN_DESC scd;
	SecureZeroMemory(&scd, sizeof(scd));
	if (VSyncEnable)
		scd = ACD3D10Configurations::DefineSwapShain(hWnd, width, height, ACD3D10Globals::G_Numerator, ACD3D10Globals::G_Denomerator); //ser form o vsync enable ele sincroniza a renderizacao com o refreshrate da tela
	else
		scd = ACD3D10Configurations::DefineSwapShain(hWnd, width, height, 60, 1); //senao ele usa direto os valores padroes isso pode gerar alguns artefatos MF
	hr = pDXGIFactory->CreateSwapChain(ACD3D10Globals::G_pD3dDevice, &scd, &pSwapChain);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Creating SwapChain. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Creating SwapChain. AddViewport()");
		return hr;
	}

	// Release the factory.
	pDXGIFactory->Release();
	pDXGIFactory = nullptr;

	vpComponent->pSwapChain = pSwapChain;
#pragma endregion

#pragma region CREATE RENDERTARGETVIEW
	//array de backbuffers, um pra cada janela
	ID3D10Texture2D* pBackBuffer;
	ID3D10RenderTargetView* pRenderTargetView;

	hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create buffer. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create buffer. AddViewport()");
		return hr;
	}
			
	hr = ACD3D10Globals::G_pD3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create render target. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create render target. AddViewport()");
		return hr;
	}

	vpComponent->pRenderTargetView = pRenderTargetView;

	pBackBuffer->Release(); //release na textura usando como backbuffer
#pragma endregion

#pragma region CREATE DEPTHSTENCILVIEW
	ID3D10Texture2D* pDepthStencil;
    ID3D10DepthStencilView* pDepthStencilView;

	//define o stencil view
	hr = ACD3D10Configurations::DefineDepthStencilView(ACD3D10Globals::G_pD3dDevice,
													&pDepthStencil, &pDepthStencilView, 
													width, height);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create depth stencil view. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create depth stencil view. AddViewport()");
		return hr;
	}

	vpComponent->pDepthStencilView = pDepthStencilView;

	pDepthStencil->Release();
#pragma endregion

#pragma region CREATE VIEWPORT
	D3D10_VIEWPORT vp;
	ACD3D10Configurations::DefineViewPort(width, height, 0, 1, rc.left, rc.top, &vp);

	vpComponent->Viewport = vp;
#pragma endregion

#pragma region CREATERENDERTOTEXTURE
	ACD3D10RenderToTexture* pRenderToTexture = new ACD3D10RenderToTexture(ACD3D10Globals::G_pD3dDevice);
	pRenderToTexture->Initialize(width, height);
	vpComponent->pRenderToTexture = pRenderToTexture;
#pragma endregion 


	//inser o objeto no map
	mpVpComponents.insert(std::pair<HWND, ACD3D10VpComponents*>(hWnd, vpComponent));


	//seta a viewport
	ACD3D10Globals::G_pD3dDevice->RSSetViewports( 1, &vp );

	//seta o render target e o depthstencil
	ACD3D10Globals::G_pD3dDevice->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

	SetActiveViewport(hWnd);
	SetActiveRenderingViewport(hWnd);

	return AC_OK;
};

HRESULT ACD3D10::DropViewport(HWND hWnd)
{
	SAFE_RELEASE(mpVpComponents[hWnd]->pRenderToTexture);
	SAFE_RELEASE(mpVpComponents[hWnd]->pDepthStencilView);
	SAFE_RELEASE(mpVpComponents[hWnd]->pRenderTargetView);
	SAFE_RELEASE(mpVpComponents[hWnd]->pSwapChain);

	mpVpComponents.erase(hWnd);
	return AC_OK;
};

void ACD3D10::SetActiveViewport(HWND hWnd)
{
	mActiveWnd = hWnd;
};

void ACD3D10::SetActiveRenderingViewport(HWND hWnd)
{
	mActiveWndRendering = hWnd;
	mpCurrentVpComponents = mpVpComponents[mActiveWndRendering];

	ACD3D10Globals::G_pD3dDevice->RSSetViewports( 1, &mpCurrentVpComponents->Viewport);
	ACD3D10Globals::G_pD3dDevice->OMSetRenderTargets( 1, &mpCurrentVpComponents->pRenderTargetView, mpCurrentVpComponents->pDepthStencilView );
};

HRESULT ACD3D10::Resize(UINT width, UINT height)
{
	HRESULT hr;

	//mata o anterior
	SAFE_RELEASE(mpVpComponents[mActiveWnd]->pRenderToTexture);
	SAFE_RELEASE(mpVpComponents[mActiveWnd]->pRenderTargetView);
	SAFE_RELEASE(mpVpComponents[mActiveWnd]->pDepthStencilView);

#pragma region RESIZE BACKBUFFER
	IDXGISwapChain* pSwapChain = mpVpComponents[mActiveWnd]->pSwapChain;
	hr = pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] ResizeBuffer. Resize()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] ResizeBuffer. Resize()");
		return hr;
	}
#pragma endregion

#pragma region CREATE RENDERTARGETVIEW
	//array de backbuffers, um pra cada janela
	ID3D10Texture2D* pBackBuffer;
	ID3D10RenderTargetView* pRenderTargetView;
	hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create Buffer. Resize()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create Buffer. Resize()");
		return hr;
	}

	hr = ACD3D10Globals::G_pD3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create render target view. Resize()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create render target view. Resize()");
		return hr;
	}

	mpVpComponents[mActiveWnd]->pRenderTargetView = pRenderTargetView;

	pBackBuffer->Release(); //release na textura usando como backbuffer
#pragma endregion

#pragma region CREATE DEPTHSTENCILVIEW
	//cria outro com as novas configuracoes
	ID3D10Texture2D* pDepthStencil;
	ID3D10DepthStencilView* pDepthStencilView;
	//define o stencil view
	hr = ACD3D10Configurations::DefineDepthStencilView(ACD3D10Globals::G_pD3dDevice,
													&pDepthStencil, &pDepthStencilView, 
													width, height);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create depth stencil view. Resize()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Create depth stencil view. Resize()");
		return hr;
	}

	mpVpComponents[mActiveWnd]->pDepthStencilView = pDepthStencilView;

	pDepthStencil->Release();
#pragma endregion

#pragma region RESIZE VIEWPORT
	mpVpComponents[mActiveWnd]->Viewport.Width = width;
	mpVpComponents[mActiveWnd]->Viewport.Height = height;
#pragma endregion

#pragma region CREATE RENDERTOTEXTURE
	ACD3D10RenderToTexture* pRenderToTexture = new ACD3D10RenderToTexture(ACD3D10Globals::G_pD3dDevice);
	pRenderToTexture->Initialize(width, height);
	mpVpComponents[mActiveWnd]->pRenderToTexture = pRenderToTexture;
#pragma endregion

	//seta a viewport
	ACD3D10Globals::G_pD3dDevice->RSSetViewports( 1, &mpVpComponents[mActiveWnd]->Viewport );

	//seta o render target e o depthstencil
	ACD3D10Globals::G_pD3dDevice->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

	return AC_OK;
};

void ACD3D10::GoFullscreen(BOOL value)
{
	mIsFullscreen = value;
	IDXGISwapChain* pSwapChain = mpVpComponents[mActiveWnd]->pSwapChain;
	if (SUCCEEDED(pSwapChain->SetFullscreenState(value, nullptr)))
		Log("Goto Fullscreen success. GoFullscreen()");
	else
	{
		MessageBoxA(nullptr, "[ERROR] Goto Fullscreen. GoFullscreen()", "Error", MB_OK | MB_ICONERROR);
		Log("[ERROR] Goto Fullscreen. GoFullscreen()");
	}
};

UINT ACD3D10::GetVPWidth()
{
	return mpVpComponents[mActiveWndRendering]->Viewport.Width;
};

UINT ACD3D10::GetVPHeight()
{
	return mpVpComponents[mActiveWndRendering]->Viewport.Height;
};

//remove a galera da memoria
void ACD3D10::Release()
{
	ACD3D10Configurations::Release();

	if( ACD3D10Globals::G_pD3dDevice ) 
		ACD3D10Globals::G_pD3dDevice->ClearState();

	SAFE_MAP_RELEASE_CLEAR(mpVpComponents);

	SAFE_RELEASE(mpVSCBPerFrame);
	SAFE_RELEASE(mpVSCBPerModel);
	SAFE_RELEASE(mpVSCBPerPass);
	SAFE_RELEASE(mpPSCBMaterial);
	SAFE_RELEASE(mpVManager);
	SAFE_RELEASE(ACD3D10Globals::G_pD3dDevice);

	if (mpLOG!=nullptr)
	{
		fclose(mpLOG);
		mpLOG = nullptr;
	}
};

//limpa os buffers
HRESULT ACD3D10::Clear(BOOL clearPixels, BOOL clearDepth, BOOL clearStencil)
{
	ACD3D10VpComponents* pVp = mpVpComponents[mActiveWndRendering];
	
	UINT flags = 0;
	if (clearPixels)
		ACD3D10Globals::G_pD3dDevice->ClearRenderTargetView( pVp->pRenderTargetView, mClearColor );

	if (clearDepth)
	{
		flags = D3D10_CLEAR_DEPTH;
		if (clearStencil)
			flags |= D3D10_CLEAR_STENCIL;
	}
	else
	{
		if (clearStencil)
			flags = D3D10_CLEAR_STENCIL;
	}

	if (flags != 0)
		ACD3D10Globals::G_pD3dDevice->ClearDepthStencilView( pVp->pDepthStencilView, flags, 1.0f, 0 );

	return AC_OK;
};

//seta a cor de fundo
void ACD3D10::SetClearColor(const Vector4& color)
{
	mClearColor[0] = color.X;
	mClearColor[1] = color.Y;
	mClearColor[2] = color.Z;
	mClearColor[3] = color.W;
};

#pragma endregion

#pragma region Log

void ACD3D10::Log(char* message, ...)
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

#pragma endregion

#pragma region Blend/Depth/Rasterization State

void ACD3D10::SetBlendState(ACBLENDSTATE state)
{
	if (mCurrentBlendState != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentBlendState = state;

		switch (state)
		{
			case ACBLENDSTATE::ACBS_Default:				 ACD3D10Configurations::DefineBlendStateDefault(); break;
			case ACBLENDSTATE::ACBS_Additive:				 ACD3D10Configurations::DefineBlendStateAdditive(); break;
			case ACBLENDSTATE::ACBS_AlphaBlend:				 ACD3D10Configurations::DefineBlendStateAlphaBlend(); break;
			case ACBLENDSTATE::ACBS_NonPremultiplied:		 ACD3D10Configurations::DefineBlendStateNonPremultiplied(); break;
			case ACBLENDSTATE::ACBS_Opaque:					 ACD3D10Configurations::DefineBlendStateOpaque(); break;
		}
	}
};

void ACD3D10::SetDepthBufferState(ACDEPTHBUFFERSTATE state)
{
	if (mCurrentDepthBufferState != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentDepthBufferState = state;

		switch (state)
		{
			case ACDEPTHBUFFERSTATE::ACDBS_WriteEnable:	    ACD3D10Configurations::DefineDepthStencilStateEnableDepthBuffer(); break;
			case ACDEPTHBUFFERSTATE::ACDBS_WriteDisable:	ACD3D10Configurations::DefineDepthStencilStateDisableDepthBuffer(); break;
		}
	}
};

void ACD3D10::SetRasterizeState(ACRASTERIZESTATE state)
{
	if (mCurrentRasterizationState != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentRasterizationState = state;

		switch (state)
		{
			case ACRASTERIZESTATE::ACRS_SolidCullCCW:	    
				ACD3D10Configurations::DefineRasterizeStateSolidCullCCW(); 
				mpVManager->SetIsWire(false);
				break;
			case ACRASTERIZESTATE::ACRS_WireframeCullCCW:	
				ACD3D10Configurations::DefineRasterizeStateWireframeCullCCW(); 
				mpVManager->SetIsWire(true);
			break;
			case ACRASTERIZESTATE::ACRS_SolidCullCW:	    
				ACD3D10Configurations::DefineRasterizeStateSolidCullCW(); 
				mpVManager->SetIsWire(false);
				break;
			case ACRASTERIZESTATE::ACRS_WireframeCullCW:	
				ACD3D10Configurations::DefineRasterizeStateWireframeCullCW(); 
				mpVManager->SetIsWire(true);
				break;
			case ACRASTERIZESTATE::ACRS_SolidCullNone:	    
				ACD3D10Configurations::DefineRasterizeStateSolidCullNone(); 
				mpVManager->SetIsWire(false);
				break;
			case ACRASTERIZESTATE::ACRS_WireframeCullNone:	
				ACD3D10Configurations::DefineRasterizeStateWireframeCullNone(); 
				mpVManager->SetIsWire(true);
				break;
		}
	}
};

void ACD3D10::SetSamplerState(ACSAMPLERSTATE state, UINT slot)
{
	if (mCurrentSamplerState[slot] != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentSamplerState[slot] = state;

		switch (state)
		{
			case ACSAMPLERSTATE::ACSS_Point_Wrap:			ACD3D10Configurations::DefineSamplerStatePointWrap(slot); break;
			case ACSAMPLERSTATE::ACSS_Bilinear_Wrap:		ACD3D10Configurations::DefineSamplerStateBilinearWrap(slot); break;
			case ACSAMPLERSTATE::ACSS_Trilinear_Wrap:		ACD3D10Configurations::DefineSamplerStateTrilinearWrap(slot); break;
			case ACSAMPLERSTATE::ACSS_Anisotropic_Wrap:		ACD3D10Configurations::DefineSamplerStateAnisotropicWrap(slot); break;

			case ACSAMPLERSTATE::ACSS_Point_Clamp:			ACD3D10Configurations::DefineSamplerStatePointClamp(slot); break;
			case ACSAMPLERSTATE::ACSS_Bilinear_Clamp:		ACD3D10Configurations::DefineSamplerStateBilinearClamp(slot); break;
			case ACSAMPLERSTATE::ACSS_Trilinear_Clamp:		ACD3D10Configurations::DefineSamplerStateTrilinearClamp(slot); break;
			case ACSAMPLERSTATE::ACSS_Anisotropic_Clamp:	ACD3D10Configurations::DefineSamplerStateAnisotropicClamp(slot); break;

			case ACSAMPLERSTATE::ACSS_Point_Border:			ACD3D10Configurations::DefineSamplerStatePointBorder(slot); break;
			case ACSAMPLERSTATE::ACSS_Bilinear_Border:		ACD3D10Configurations::DefineSamplerStateBilinearBorder(slot); break;
			case ACSAMPLERSTATE::ACSS_Trilinear_Border:		ACD3D10Configurations::DefineSamplerStateTrilinearBorder(slot); break;
			case ACSAMPLERSTATE::ACSS_Anisotropic_Border:	ACD3D10Configurations::DefineSamplerStateAnisotropicBorder(slot); break;
		}
	}
};

void ACD3D10::SetShadeMode(ACSHADEMODE value)
{
	//se o modo de preenchimento nao for ao corrente entao ele manda renderizar tudo q ta no buffer
	if (value != mCurrentShadeMode)
	{
		mpVManager->ForcedFlushAll();
		mCurrentShadeMode = value;
	}
};

ACSHADEMODE ACD3D10::GetShadeMode()
{
	return mCurrentShadeMode;
};

void ACD3D10::SetWireColor(const Vector3& color)
{
	mpVManager->SetWireColor(color);
};

#pragma endregion

#pragma region Render

//comeca a renderizacao
HRESULT ACD3D10::BeginRendering()
{
	return ACD3D10::Clear(true, true, true);
};

void ACD3D10::RenderLines(UINT numVertices, 
					UINT numIndices, 
					ACVertexPositionColored* vertices, 
					UINT* indices,
					BOOL strip)
{
	mpVManager->RenderLines(numVertices, numIndices, vertices, indices, strip);
};

//renderiza pontos usando o vb dinamico
void ACD3D10::RenderPoints(UINT numVertices, 
					       ACVertexPositionColored* vertices)
{
	mpVManager->RenderPoints(numVertices, vertices);
};

//renderiza sprites, é um metodo especial usando dentro do spritebatch apenas
void ACD3D10::RenderSprite(UINT numVertices, 
						   ACVertexSprite* vertices,
						   ACSkin* skin)
{
	//no caso do directx10+ eu uso point pq o sprite vai ser criado no GS
	//quando nao tem suporte a GS ele tem q criar aqui mesmo os vertices pra mandar
	SetShadeMode(ACSHADEMODE::ACSM_PointSprite);
	mpVManager->Render(VertexFormat::VF_VertexSprite, numVertices, 0, vertices, nullptr, skin);
}; 

//render dinamico de triangulos
void ACD3D10::Render(VertexFormat vertexFormat, 
					 UINT numVertices, 
					 UINT numIndices, 
					 void* vertices, 
					 UINT* indices,
					 ACSkin* skin)
{
	mpVManager->Render(vertexFormat, numVertices, numIndices, vertices, indices, skin);
};

//renderiza oq tem no vb estatico
void ACD3D10::Render(ACVertexBuffer* vertexBuffer)
{
	mpVManager->Render(vertexBuffer);
};

//termina a renderizacao
void ACD3D10::EndRendering()
{
	//renderiza tudo q esta no vertexbuffer dinamico
	mpVManager->ForcedFlushAll();
	ACD3D10VpComponents* pVp = mpVpComponents[mActiveWndRendering];
	pVp->pSwapChain->Present( 0, 0 );
};

#pragma endregion

#pragma region VB/IB

HRESULT ACD3D10::CreateStaticBuffer(VertexFormat vertexFormat, 
								   UINT numVertices, 
								   UINT numIndices, 
								   const void* pVertices, 
								   const UINT* pIndices, 
								   ACSkin* pSkin,
								   ACVertexBuffer** ppOutVertexBufferPtr)
{
	return mpVManager->CreateStaticBuffer(vertexFormat, 
										  numVertices, 
										  numIndices,
										  pVertices, 
										  pIndices, 
										  pSkin,
										  ppOutVertexBufferPtr);
};

void ACD3D10::ReleaseBuffer(ACVertexBuffer* vertexBuffer)
{
	mpVManager->ReleaseBuffer(vertexBuffer);
};

#pragma endregion

#pragma region Textures

//carrega uma textura 2d do disco
HRESULT ACD3D10::LoadTexture(std::string name, ACTexture** ppOutTexturePtr)
{
	std::string currentPath = ACGlobals::GetPathTextures();
	currentPath.append(name);

	*ppOutTexturePtr = new ACTexture();

	ID3D10Resource* pTexture = nullptr;

	//carrega a textura do arquivo
	HRESULT hr = D3DX10CreateTextureFromFileA(ACD3D10Globals::G_pD3dDevice,
									currentPath.c_str(),
									nullptr,
									nullptr,
									&pTexture,
									nullptr);

	if(hr != AC_OK)
    {
		std::string message;
        switch(hr)
        {
			case D3D10_ERROR_FILE_NOT_FOUND:  message = "[ERROR] Erro ao carregar textura. File not found: " + name;  break;
			case D3DERR_INVALIDCALL:          message = "[ERROR] Erro ao carregar textura. Invalid call: "   + name; break;
			case E_INVALIDARG:                message = "[ERROR] Erro ao carregar textura. Invalid argument: " + name; break;
			case E_OUTOFMEMORY:               message = "[ERROR] Erro ao carregar textura. Out of memory: " + name; break;
			default:                          message = "[ERROR] Erro ao carregar textura. Unknown error: " + name;
		}

		MessageBoxA(nullptr, message.c_str(), "Error", MB_OK | MB_ICONERROR);
		Log(const_cast<char*>(message.c_str()));

		return hr;
    }

	//dimension type
	D3D10_RESOURCE_DIMENSION type;
	pTexture->GetType( &type );
	switch( type )
	{
		case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D10_TEXTURE1D_DESC desc;
			ID3D10Texture1D *pTexture1D = (ID3D10Texture1D*)pTexture;
			pTexture1D->GetDesc( &desc );

			//popula o actexture
			(*ppOutTexturePtr)->Width = desc.Width;
			(*ppOutTexturePtr)->Height = 0;
			(*ppOutTexturePtr)->MipLevels = desc.MipLevels;
			(*ppOutTexturePtr)->TextureType = ACTT_Texture1D;
		}
		break;
		case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
		{
			D3D10_TEXTURE2D_DESC desc;
			ID3D10Texture2D *pTexture2D = (ID3D10Texture2D*)pTexture;
			pTexture2D->GetDesc( &desc );

			//popula o actexture
			(*ppOutTexturePtr)->Width = desc.Width;
			(*ppOutTexturePtr)->Height = desc.Height;
			(*ppOutTexturePtr)->MipLevels = desc.MipLevels;
			(*ppOutTexturePtr)->TextureType = ACTT_Texture2D;
		}
		break;
		case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D10_TEXTURE3D_DESC desc;
			ID3D10Texture3D *pTexture3D = (ID3D10Texture3D*)pTexture;
			pTexture3D->GetDesc( &desc );

			//popula o actexture
			(*ppOutTexturePtr)->Width = desc.Width;
			(*ppOutTexturePtr)->Height = desc.Height;
			(*ppOutTexturePtr)->Depth = desc.Depth;
			(*ppOutTexturePtr)->MipLevels = desc.MipLevels;
			(*ppOutTexturePtr)->TextureType = ACTT_Texture3D;
		}
		break;
	}

	ID3D10ShaderResourceView* pSRV;
	hr = ACD3D10Globals::G_pD3dDevice->CreateShaderResourceView( pTexture, nullptr, &pSRV );

	if (FAILED ( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] CreateShaderResourceView. LoadTexture()", "Error", MB_OK | MB_ICONERROR);
	}

	//popula o actexture
	(*ppOutTexturePtr)->pData = pSRV;
	(*ppOutTexturePtr)->Name = name;

	return AC_OK;
};

//remove a textura da memoria do buffer
HRESULT ACD3D10::ReleaseTexture(ACTexture* pTexture)
{
	((ID3D10ShaderResourceView*)pTexture->pData)->Release();
	pTexture->pData = nullptr;
	return AC_OK;
};

//setar a textura para o shader, o numero do slot é a posicao da textura dentro do shader
void ACD3D10::SetTexture(ACTexture* pTexture, UINT slot)
{
	if (pTexture != mpCurrentTexture[slot])
	{
		ID3D10ShaderResourceView* tex = nullptr;
		if (pTexture!=nullptr)
			tex = (ID3D10ShaderResourceView*)pTexture->pData;

		mpCurrentTexture[slot] = pTexture;
		ACD3D10Globals::G_pD3dDevice->PSSetShaderResources(slot, 1, &tex);
	}
};

#pragma endregion

#pragma region Shaders

//VERTEX SHADER
HRESULT ACD3D10::CompileVS(std::string fileName, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr)
{
	std::string currentPath = ACGlobals::GetPathShaders();
	currentPath.append(fileName);

	*ppOutVertexShaderPtr = new ACVertexShader();

	//compila o VS 
	ID3D10Blob* pVSBuf = nullptr;
	ID3D10Blob* pError = nullptr;

	//se estiver em debug ele compila o shader para ser debugado, em release ele é otimizado
	DWORD dwShaderFlags = 0;
	#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3D10_SHADER_DEBUG;
    #endif

	HRESULT hr = 0;
	hr = D3DX10CompileFromFileA(currentPath.c_str(), 
								nullptr, 
								nullptr, 
								"main", 
								"vs_4_0", 
								dwShaderFlags, 
								0, 
								nullptr, 
								&pVSBuf, 
								&pError, 
								nullptr );

    if( FAILED( hr ) )
    {
		char* pCompileErrors = static_cast<char*>(pError->GetBufferPointer());
		MessageBoxA(nullptr, pCompileErrors, "Error vS", MB_OK | MB_ICONERROR);
		Log("[ERROR] Erro ao carregar vertexshader. CompileVS(): %s", pCompileErrors); 
        hr = AC_FAIL;
    }
	else
	{
		//cria o vertexbuffer
		ID3D10VertexShader* pVS;
		ACD3D10Globals::G_pD3dDevice->CreateVertexShader( ( DWORD* )pVSBuf->GetBufferPointer(),
														  pVSBuf->GetBufferSize(),
														  &pVS);

		(*ppOutVertexShaderPtr)->pVS = pVS;
		(*ppOutVertexShaderPtr)->Name = fileName;
		(*ppOutVertexShaderPtr)->Format = vertexFormat;

		//cria o layout de vertice
		hr = ACD3D10VertexLayoutProvider::CreateInputLayout(ACD3D10Globals::G_pD3dDevice, pVSBuf, vertexFormat);
	}

	SAFE_RELEASE(pVSBuf);
	SAFE_RELEASE(pError);

	return hr;
};

HRESULT ACD3D10::ReleaseVS(ACVertexShader* pVertexShader)
{
	((ID3D10VertexShader*)pVertexShader->pVS)->Release();
	pVertexShader->pVS = nullptr;
	return AC_OK;
};

void ACD3D10::ActiveVS(ACVertexShader* vertexShader)
{ 
	//muda estado 
	if (vertexShader != mpCurrentVertexShader)
	{
		//se mudar o shader ele limpa o buffer
		mpVManager->ForcedFlushAll();

		mpCurrentVertexShader = vertexShader;

		if (vertexShader != nullptr)
		{
			ACD3D10Globals::G_pD3dDevice->VSSetShader( (ID3D10VertexShader*)vertexShader->pVS );

			//muda estado
			switch( vertexShader->Format)
			{
				case VF_VertexPosition: 	
					 ACD3D10Globals::G_pD3dDevice->IASetInputLayout(ACD3D10VertexLayoutProvider::pVertexPositionLayout); 
				break;
				case VF_VertexPositionColored: 	
					 ACD3D10Globals::G_pD3dDevice->IASetInputLayout(ACD3D10VertexLayoutProvider::pVertexPositionColoredLayout); 
				break;
				case VF_VertexPositionTextured: 	
					 ACD3D10Globals::G_pD3dDevice->IASetInputLayout(ACD3D10VertexLayoutProvider::pVertexPositionTexturedLayout); 
				break;
				case VF_VertexPositionNormalTextured: 	
					 ACD3D10Globals::G_pD3dDevice->IASetInputLayout(ACD3D10VertexLayoutProvider::pVertexPositionNormalTexturedLayout); 
				break;
				case VF_VertexPositionTexturedExtraInfo: 	
					 ACD3D10Globals::G_pD3dDevice->IASetInputLayout(ACD3D10VertexLayoutProvider::pVertexPositionTexturedExtraInfoLayout); 
				break;
				case VF_VertexSprite: 	
					 ACD3D10Globals::G_pD3dDevice->IASetInputLayout(ACD3D10VertexLayoutProvider::pVertexSpriteLayout); 
				break;
			};
		}
		else
		{
			ACD3D10Globals::G_pD3dDevice->VSSetShader( nullptr );
			ACD3D10Globals::G_pD3dDevice->IASetInputLayout( nullptr );
		}
	}
};

//PIXEL SHADER
HRESULT ACD3D10::CompilePS(std::string fileName, ACPixelShader** ppOutPixelShaderPtr)
{
	std::string currentPath = ACGlobals::GetPathShaders();
	currentPath.append(fileName);

	*ppOutPixelShaderPtr = new ACPixelShader();

	//compila o PS 
	ID3D10Blob* pPSBuf = nullptr;
	ID3D10Blob* pError = nullptr;

	//se estiver em debug ele compila o shader para ser debugado, em release ele é otimizado
	DWORD dwShaderFlags = 0;
	#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3D10_SHADER_DEBUG;
    #endif

	HRESULT hr = 0;
	hr = D3DX10CompileFromFileA(currentPath.c_str(), 
								nullptr, 
								nullptr, 
								"main", 
								"ps_4_0", 
								dwShaderFlags, 
								0, 
								nullptr, 
								&pPSBuf, 
								&pError, 
								nullptr );

    if( FAILED( hr ) )
    {
		char* pCompileErrors = static_cast<char*>(pError->GetBufferPointer());
		MessageBoxA(nullptr, pCompileErrors, "Error PS", MB_OK | MB_ICONERROR);
		Log("[ERROR] Erro ao carregar pixeshader. CompilePS(): %s", pCompileErrors); 
        hr = AC_FAIL;
    }
	else
	{
		//cria o pixelbuffer
		ID3D10PixelShader* pPS;
		ACD3D10Globals::G_pD3dDevice->CreatePixelShader( ( DWORD* )pPSBuf->GetBufferPointer(),
														  pPSBuf->GetBufferSize(), 
														  &pPS);

		(*ppOutPixelShaderPtr)->pPS = pPS;
		(*ppOutPixelShaderPtr)->Name = fileName;

		hr = AC_OK;
	}

	SAFE_RELEASE(pPSBuf);
	SAFE_RELEASE(pError);

	return hr;
};

HRESULT ACD3D10::ReleasePS(ACPixelShader* pPixelShader)
{
	((ID3D10PixelShader*)pPixelShader->pPS)->Release();
	pPixelShader->pPS = nullptr;
	return AC_OK;
};

void ACD3D10::ActivePS(ACPixelShader* pixelShader)
{
	//muda estado
	if (pixelShader != mpCurrentPixelShader)
	{
		//se mudar o shader ele limpa o buffer
		mpVManager->ForcedFlushAll();

		mpCurrentPixelShader = pixelShader;

		if (pixelShader != nullptr)
			ACD3D10Globals::G_pD3dDevice->PSSetShader( (ID3D10PixelShader*)pixelShader->pPS );
		else
			ACD3D10Globals::G_pD3dDevice->PSSetShader( nullptr );
	}
};

//GEOMETRY SHADER
HRESULT ACD3D10::CompileGS(std::string fileName, ACGeometryShader** ppOutGeometryShaderPtr)
{
	std::string currentPath = ACGlobals::GetPathShaders();
	currentPath.append(fileName);

	*ppOutGeometryShaderPtr = new ACGeometryShader();

	//compila o GS 
	ID3D10Blob* pGSBuf = nullptr;
	ID3D10Blob* pError = nullptr;

	//se estiver em debug ele compila o shader para ser debugado, em release ele é otimizado
	DWORD dwShaderFlags = 0;
	#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3D10_SHADER_DEBUG;
    #endif

	HRESULT hr = 0;
	hr = D3DX10CompileFromFileA(currentPath.c_str(), 
								nullptr, 
								nullptr, 
								"main", 
								"gs_4_0", 
								dwShaderFlags, 
								0, 
								nullptr, 
								&pGSBuf, 
								&pError, 
								nullptr );

    if( FAILED( hr ) )
    {
		char* pCompileErrors = static_cast<char*>(pError->GetBufferPointer());
		MessageBoxA(nullptr, pCompileErrors, "Error GS", MB_OK | MB_ICONERROR);
		Log("[ERROR] Erro ao carregar geometryshader. CompileGS(): %s", pCompileErrors); 
        hr = AC_FAIL;
    }
	else
	{
		//cria o geometrybuffer
		ID3D10GeometryShader* pGS;
		ACD3D10Globals::G_pD3dDevice->CreateGeometryShader( ( DWORD* )pGSBuf->GetBufferPointer(),
														  pGSBuf->GetBufferSize(), 
														  &pGS);

		(*ppOutGeometryShaderPtr)->pGS = pGS;
		(*ppOutGeometryShaderPtr)->Name = fileName;

		hr = AC_OK;
	}

	SAFE_RELEASE(pGSBuf);
	SAFE_RELEASE(pError);

	return hr;
};

HRESULT ACD3D10::ReleaseGS(ACGeometryShader* pGeometryShader)
{
	((ID3D10GeometryShader*)pGeometryShader->pGS)->Release();
	pGeometryShader->pGS = nullptr;
	return AC_OK;
};

void ACD3D10::ActiveGS(ACGeometryShader* geometryShader)
{
	//muda estado
	if (geometryShader != mpCurrentGeometryShader)
	{
		//se mudar o shader ele limpa o buffer
		mpVManager->ForcedFlushAll();

		mpCurrentGeometryShader = geometryShader;
		if (geometryShader!=nullptr)
			ACD3D10Globals::G_pD3dDevice->GSSetShader( (ID3D10GeometryShader*)geometryShader->pGS );
		else
			ACD3D10Globals::G_pD3dDevice->GSSetShader( nullptr );
	}
};

#pragma endregion

#pragma region Shaders Constants
void ACD3D10::TransposeMatrixToColumnMajor(const Matrix& source, Matrix& dest)
{
	dest.M11 = source.M11;
	dest.M21 = source.M12;
	dest.M31 = source.M13;
	dest.M41 = source.M14;
	dest.M12 = source.M21;
	dest.M22 = source.M22;
	dest.M32 = source.M23;
	dest.M42 = source.M24;
	dest.M13 = source.M31;
	dest.M23 = source.M32;
	dest.M33 = source.M33;
	dest.M43 = source.M34;
	dest.M14 = source.M41;
	dest.M24 = source.M42;
	dest.M34 = source.M43;
	dest.M44 = source.M44;
};

void ACD3D10::SetWorldMatrix(const Matrix& value)
{
	Matrix currentWorld = mVSCBPerModelData.xWorld;
	TransposeMatrixToColumnMajor(value, mVSCBPerModelData.xWorld);

	//se nao for a mesma matrix world entao ele da o flush nos buffer e ja seta
	if (currentWorld != mVSCBPerModelData.xWorld)
	{
		mpVManager->ForcedFlushAll();

		VS_CB_PERMODEL * pCBPerModelData;
		mpVSCBPerModel->Map( D3D10_MAP_WRITE_DISCARD, NULL, ( void** )&pCBPerModelData );
		pCBPerModelData->xWorld = mVSCBPerModelData.xWorld;
		mpVSCBPerModel->Unmap();
		ACD3D10Globals::G_pD3dDevice->VSSetConstantBuffers(1, 1, &mpVSCBPerModel);
	}
};

void ACD3D10::SetViewProjectionMatrix(const Matrix& value)
{
	mChangeCBPerPass = true;
	TransposeMatrixToColumnMajor(value, mVSCBPerPassData.xViewProjection);
};

void ACD3D10::SetLightPosition(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightPosition = value;
};

void ACD3D10::SetLightDirection(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightDirection = value;
};

void ACD3D10::SetLightColor(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightColor = value;
};

void ACD3D10::SetLightRange(float value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightRange = value;
};

void ACD3D10::SetLightFalloff(float value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightFalloff = value;
};

void ACD3D10::SetCameraPosition(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xCameraPosition = value;
};

void ACD3D10::SetTime(float value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xTime = value;
};

void ACD3D10::SetMaterialAmbient(const Vector3& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialAmbient = value;
};

void ACD3D10::SetMaterialEmissive(const Vector3& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialEmissive = value;
};

void ACD3D10::SetMaterialSpecular(const Vector3& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialSpecular = value;
};

void ACD3D10::SetMaterialSpecularPower(float value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialSpecularPower = value;
};

void ACD3D10::SetMaterialDiffuse(const Vector4& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialDiffuse = value;
};

void ACD3D10::ApplyConstants()
{
	//seta o constantbuffer por frame
	if (mChangeCBPerFrame)
	{
		VS_CB_PERFRAME * pCBPerFrameData;
		mpVSCBPerFrame->Map( D3D10_MAP_WRITE_DISCARD, NULL, ( void** )&pCBPerFrameData );
		pCBPerFrameData->xCameraPosition = mVSCBPerFrameData.xCameraPosition;
		pCBPerFrameData->xLightPosition = mVSCBPerFrameData.xLightPosition;
		pCBPerFrameData->xLightDirection = mVSCBPerFrameData.xLightDirection;
		pCBPerFrameData->xLightColor = mVSCBPerFrameData.xLightColor;
		pCBPerFrameData->xLightRange = mVSCBPerFrameData.xLightRange;
		pCBPerFrameData->xLightFalloff = mVSCBPerFrameData.xLightFalloff;
		pCBPerFrameData->xTime = mVSCBPerFrameData.xTime;
		mpVSCBPerFrame->Unmap();
		ACD3D10Globals::G_pD3dDevice->VSSetConstantBuffers(0, 1, &mpVSCBPerFrame);
		ACD3D10Globals::G_pD3dDevice->PSSetConstantBuffers(0, 1, &mpVSCBPerFrame);

		mChangeCBPerFrame= false;
	}

	//seta o constantbuffer por passo
	if (mChangeCBPerPass)
	{
		VS_CB_PERPASS * pCBPerPassData;
		mpVSCBPerPass->Map( D3D10_MAP_WRITE_DISCARD, NULL, ( void** )&pCBPerPassData );
		pCBPerPassData->xViewProjection = mVSCBPerPassData.xViewProjection;
		mpVSCBPerPass->Unmap();
		ACD3D10Globals::G_pD3dDevice->VSSetConstantBuffers(2, 1, &mpVSCBPerPass);

		mChangeCBPerPass= false;
	}

	//seta o constantbuffer do material
	if (mChangeCBMaterial)
	{
		PS_CB_MATERIAL* pCBMaterialData;
		mpPSCBMaterial->Map( D3D10_MAP_WRITE_DISCARD, NULL, ( void** )&pCBMaterialData );
		pCBMaterialData->xMaterialAmbient =  mPSCBMaterialData.xMaterialAmbient;
		pCBMaterialData->xMaterialEmissive = mPSCBMaterialData.xMaterialEmissive;
		pCBMaterialData->xMaterialDiffuse = mPSCBMaterialData.xMaterialDiffuse;
		pCBMaterialData->xMaterialSpecular = mPSCBMaterialData.xMaterialSpecular;
		pCBMaterialData->xMaterialSpecularPower = mPSCBMaterialData.xMaterialSpecularPower;
		mpPSCBMaterial->Unmap();
		ACD3D10Globals::G_pD3dDevice->PSSetConstantBuffers(1, 1, &mpPSCBMaterial);

		mChangeCBMaterial = false;
	}
};

#pragma endregion

#pragma region Tools

void ACD3D10::SaveScreenShot(const std::string& path)
{
	ACD3D10VpComponents* vpComponent = mpCurrentVpComponents = mpVpComponents[mActiveWnd];

    ID3D11Resource* backbufferRes;
    mpRenderTargetView->GetResource(&backbufferRes);
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.ArraySize = 1;
    texDesc.BindFlags = 0;
    texDesc.CPUAccessFlags = 0;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Width = vpComponent->Viewport.Width;
    texDesc.Height = vpComponent->Viewport.Height; 
    texDesc.MipLevels = 1;
    texDesc.MiscFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    ID3D11Texture2D* texture;
    if (FAILED(mpGDevice->CreateTexture2D(&texDesc, 0, &texture)))
		MessageBoxA(nullptr, "Error saving image.", "Error", MB_OK);

    mpContext->CopyResource(texture, backbufferRes);
    if (FAILED(D3DX10SaveTextureToFileA(texture, D3DX10_IFF_BMP, path.c_str())))
		MessageBoxA(nullptr, "Error saving image.", "Error", MB_OK);

    SAFE_RELEASE( texture );
    SAFE_RELEASE( backbufferRes );
};

#pragma endregion