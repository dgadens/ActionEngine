#include "ACD3D.h"

//methodo externo a classe q cria o gdevice
HRESULT CreateRenderDevice(HINSTANCE hDLL, ACRenderDevice **pDevice)
{
	if (!*pDevice)
	{
		*pDevice = new ACD3D(hDLL);
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

ACD3D::ACD3D(HINSTANCE hDLL)
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

ACD3D::~ACD3D()
{
	Release();
};

#pragma region Initialization

HRESULT ACD3D::Init(HWND hWnd, BOOL enableVSync, BOOL log)
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
	ACD3DConfigurations::DefineBlendStateDefault();

	//inicializa escrevendo no zbuffer
	mCurrentDepthBufferState	= ACDEPTHBUFFERSTATE::ACDBS_WriteEnable;	
	ACD3DConfigurations::DefineDepthStencilStateEnableDepthBuffer();

	//inicializa como solido anti-horario
	mCurrentRasterizationState	= ACRASTERIZESTATE::ACRS_SolidCullCCW;
	ACD3DConfigurations::DefineRasterizeStateSolidCullCCW();

	//inicializa todos os samplers states 16 o caso do d3d10
	for (int i = 0; i < 16; i++)
	{
		mCurrentSamplerState[i] = ACSAMPLERSTATE::ACSS_Bilinear_Wrap;
		ACD3DConfigurations::DefineSamplerStateBilinearWrap(i);
	}

	//cria o vertexmanager
	mpVManager = new ACD3DVertexManager(this, ACD3DGlobals::G_pD3dDevice, ACConfigurations::GetMaxVerticesInBuffer(), ACConfigurations::GetMaxIndicesInBuffer(), mpLOG);
	ACD3DGlobals::G_pVertexManager = static_cast<ACD3DVertexManager*>(mpVManager);
	
	Log("Init Success");

    return hr;
};

HRESULT ACD3D::CreateGraphicsDevice(int width, int height)
{
	HRESULT hr;

	//inicializa membros
	mDriverType = D3D_DRIVER_TYPE_HARDWARE;
	mFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	ACD3DGlobals::G_pD3dDevice = nullptr;
	ACD3DGlobals::G_pContext = nullptr;

	//tipo de dispositivo debug ou release
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	//********************************
	//  cria do dispositivo grafico
	//********************************

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		mDriverType = driverTypes[driverTypeIndex];
		//passa null no adapter ai ele cria automatico, depois eu recupero para pegar algumas configuracoes
		hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, 
							   numFeatureLevels, D3D11_SDK_VERSION, &ACD3DGlobals::G_pD3dDevice, 
							   &mFeatureLevel, &ACD3DGlobals::G_pContext );
		if( SUCCEEDED( hr ) )
		{
			Log("Create device success.");
			break;
		}
	}
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create device error. D3DCreateDevice()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	//*********************************
	// fim da criacao do dispositivo grafico
	//*********************************
	IDXGIFactory* pDXGIFactory = ACD3DTools::GetDXGIFactory();
	IDXGIAdapter* pDXGIAdapter = ACD3DTools::GetDXGIAdapter();

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
		MessageBoxA(nullptr, "[ERROR] EnumOutputs. D3DCreateDevice()", "Error", MB_OK | MB_ICONERROR);
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
				ACD3DGlobals::G_Numerator = pDisplayModeList[i].RefreshRate.Numerator;
				ACD3DGlobals::G_Denomerator = pDisplayModeList[i].RefreshRate.Denominator;
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

HRESULT ACD3D::CreateConstantBuffers()
{
	HRESULT hr = AC_OK;

    // Cria o constant buffer para o VS_CB_PERFRAME
    D3D11_BUFFER_DESC cbDescVS;
	ZeroMemory( &cbDescVS, sizeof (D3D11_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( VS_CB_PERFRAME );
    cbDescVS.Usage = D3D11_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3DGlobals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpVSCBPerFrame );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create VS_CB_PERFRAME. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Cria o constant buffer para o VS_CB_PERMODEL
	ZeroMemory( &cbDescVS, sizeof (D3D11_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( VS_CB_PERMODEL );
    cbDescVS.Usage = D3D11_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3DGlobals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpVSCBPerModel );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create VS_CB_PERMODEL. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Cria o constant buffer para o VS_CB_PERPASS
	ZeroMemory( &cbDescVS, sizeof (D3D11_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( VS_CB_PERPASS );
    cbDescVS.Usage = D3D11_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3DGlobals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpVSCBPerPass );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create VS_CB_PERPASS. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Cria o constant buffer para o PS_CB_MATERIAL
	ZeroMemory( &cbDescVS, sizeof (D3D11_BUFFER_DESC));
    cbDescVS.ByteWidth = sizeof( PS_CB_MATERIAL );
    cbDescVS.Usage = D3D11_USAGE_DYNAMIC;
    cbDescVS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDescVS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDescVS.MiscFlags = 0;
	hr = ACD3DGlobals::G_pD3dDevice->CreateBuffer(&cbDescVS, NULL, &mpPSCBMaterial );
	if(FAILED(hr))
	{
		MessageBoxA(nullptr, "[ERROR] Create PS_CB_LIGHT. CreateConstantBuffers()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	return hr;
};

HRESULT ACD3D::AddViewport(HWND hWnd, BOOL enableVSync)
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
	ACD3DVpComponents* vpComponent = new ACD3DVpComponents();

	//pega o factory
	IDXGIFactory* pDXGIFactory = ACD3DTools::GetDXGIFactory();

	#pragma region CREATE SWAP CHAIN
	IDXGISwapChain* pSwapChain;

	//define o rendertargetview, define o buffer e coloca como backbuffer em cada swapchain
	//cria os swpachains para cada janela
	DXGI_SWAP_CHAIN_DESC scd;
	SecureZeroMemory(&scd, sizeof(scd));
	if (VSyncEnable)
		scd = ACD3DConfigurations::DefineSwapShain(hWnd, width, height, ACD3DGlobals::G_Numerator, ACD3DGlobals::G_Denomerator); //ser form o vsync enable ele sincroniza a renderizacao com o refreshrate da tela
	else
		scd = ACD3DConfigurations::DefineSwapShain(hWnd, width, height, 60, 1); //senao ele usa direto os valores padroes isso pode gerar alguns artefatos MF
	hr = pDXGIFactory->CreateSwapChain(ACD3DGlobals::G_pD3dDevice, &scd, &pSwapChain);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Creating SwapChain. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// Release the factory.
	pDXGIFactory->Release();
	pDXGIFactory = nullptr;

	vpComponent->pSwapChain = pSwapChain;
	#pragma endregion

	#pragma region CREATE RENDERTARGETVIEW
	//array de backbuffers, um pra cada janela
	ID3D11Texture2D* pBackBuffer;
	ID3D11RenderTargetView* pRenderTargetView;

	hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create buffer. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = ACD3DGlobals::G_pD3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create render target. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	vpComponent->pRenderTargetView = pRenderTargetView;

	pBackBuffer->Release();
	#pragma endregion

	#pragma region CREATE DEPTHSTENCILVIEW
	ID3D11Texture2D* pDepthStencil;
    ID3D11DepthStencilView* pDepthStencilView;

	//define o stencil view
	hr = ACD3DConfigurations::DefineDepthStencilView(ACD3DGlobals::G_pD3dDevice,
													&pDepthStencil, &pDepthStencilView, 
													width, height);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create depth stencil view. AddViewport()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	vpComponent->pDepthStencilView = pDepthStencilView;

	pDepthStencil->Release();
	#pragma endregion

	#pragma region CREATE VIEWPORT
	D3D11_VIEWPORT vp;
	ACD3DConfigurations::DefineViewPort(width, height, 0, 1, rc.left, rc.top, &vp);

	vpComponent->Viewport = vp;
	#pragma endregion

	//inser o objeto no map
	mpVpComponents.insert(std::pair<HWND, ACD3DVpComponents*>(hWnd, vpComponent));


	//seta a viewport
	ACD3DGlobals::G_pContext->RSSetViewports( 1, &vp );

	//seta o render target e o depthstencil
	ACD3DGlobals::G_pContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

	SetActiveViewport(hWnd);
	SetActiveRenderingViewport(hWnd);

	return AC_OK;
};

HRESULT ACD3D::DropViewport(HWND hWnd)
{
	SAFE_RELEASE(mpVpComponents[hWnd]->pRenderTargetView);
	SAFE_RELEASE(mpVpComponents[hWnd]->pDepthStencilView);
	SAFE_RELEASE(mpVpComponents[hWnd]->pSwapChain);

	mpVpComponents.erase(hWnd);
	return AC_OK;
};

void ACD3D::SetActiveViewport(HWND hWnd)
{
	mActiveWnd = hWnd;
};

void ACD3D::SetActiveRenderingViewport(HWND hWnd)
{
	mActiveWndRendering = hWnd;
	mpCurrentVpComponents = mpVpComponents[mActiveWndRendering];

	ACD3DGlobals::G_pContext->RSSetViewports( 1, &mpCurrentVpComponents->Viewport);
	ACD3DGlobals::G_pContext->OMSetRenderTargets( 1, &mpCurrentVpComponents->pRenderTargetView, mpCurrentVpComponents->pDepthStencilView );
};

HRESULT ACD3D::Resize(UINT width, UINT height)
{
	HRESULT hr;

	//mata o anterior
	SAFE_RELEASE(mpVpComponents[mActiveWnd]->pRenderTargetView);
	SAFE_RELEASE(mpVpComponents[mActiveWnd]->pDepthStencilView);

	#pragma region RESIZE BACKBUFFER
	IDXGISwapChain* pSwapChain = mpVpComponents[mActiveWnd]->pSwapChain;
	hr = pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] ResizeBuffer. Resize()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	#pragma endregion

	#pragma region CREATE RENDERTARGETVIEW
	//array de backbuffers, um pra cada janela
	ID3D11Texture2D* pBackBuffer;
	ID3D11RenderTargetView* pRenderTargetView;
	hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create Buffer. Resize()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = ACD3DGlobals::G_pD3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create render target view. Resize()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	mpVpComponents[mActiveWnd]->pRenderTargetView = pRenderTargetView;
	pBackBuffer->Release();
	#pragma endregion

	#pragma region CREATE DEPTHSTENCILVIEW
	//cria outro com as novas configuracoes
	ID3D11Texture2D* pDepthStencil;
	ID3D11DepthStencilView* pDepthStencilView;
	//define o stencil view
	hr = ACD3DConfigurations::DefineDepthStencilView(ACD3DGlobals::G_pD3dDevice,
													&pDepthStencil, &pDepthStencilView, 
													width, height);
	if( FAILED( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] Create depth stencil view. Resize()", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	mpVpComponents[mActiveWnd]->pDepthStencilView = pDepthStencilView;

	pDepthStencil->Release();
	#pragma endregion

	#pragma region RESIZE VIEWPORT
	mpVpComponents[mActiveWnd]->Viewport.Width = width;
	mpVpComponents[mActiveWnd]->Viewport.Height = height;
	#pragma endregion

	//seta a viewport
	ACD3DGlobals::G_pContext->RSSetViewports( 1, &mpVpComponents[mActiveWnd]->Viewport );

	//seta o render target e o depthstencil
	ACD3DGlobals::G_pContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

	return AC_OK;
};

void ACD3D::GoFullscreen(BOOL value)
{
	mIsFullscreen = value;
	IDXGISwapChain* pSwapChain = mpVpComponents[mActiveWnd]->pSwapChain;
	if (SUCCEEDED(pSwapChain->SetFullscreenState(value, nullptr)))
		Log("Goto Fullscreen success. GoFullscreen()");
	else
	{
		MessageBoxA(nullptr, "[ERROR] Goto Fullscreen. GoFullscreen()", "Error", MB_OK | MB_ICONERROR);
	}
};

UINT ACD3D::GetVPWidth()
{
	return mpVpComponents[mActiveWndRendering]->Viewport.Width;
};

UINT ACD3D::GetVPHeight()
{
	return mpVpComponents[mActiveWndRendering]->Viewport.Height;
};

//remove a galera da memoria
void ACD3D::Release()
{
	ACD3DConfigurations::Release();
	ACD3DVertexLayoutProvider::ReleaseAll();

	if( ACD3DGlobals::G_pContext ) 
		ACD3DGlobals::G_pContext->ClearState();

	SAFE_MAP_RELEASE_CLEAR(mpVpComponents);
	SAFE_VECTOR_RELEASE_CLEAR(mpRenderTargets);

	SAFE_RELEASE(mpVSCBPerFrame);
	SAFE_RELEASE(mpVSCBPerModel);
	SAFE_RELEASE(mpVSCBPerPass);
	SAFE_RELEASE(mpPSCBMaterial);
	SAFE_RELEASE(mpVManager);
	SAFE_RELEASE(ACD3DGlobals::G_pContext);
	SAFE_RELEASE(ACD3DGlobals::G_pD3dDevice);

	if (mpLOG!=nullptr)
	{
		fclose(mpLOG);
		mpLOG = nullptr;
	}
};

//limpa os buffers
HRESULT ACD3D::Clear(BOOL clearPixels, BOOL clearDepth, BOOL clearStencil)
{
	ACD3DVpComponents* pVp = mpVpComponents[mActiveWndRendering];
	
	UINT flags = 0;
	if (clearPixels)
		ACD3DGlobals::G_pContext->ClearRenderTargetView( pVp->pRenderTargetView, mClearColor );

	if (clearDepth)
	{
		flags = D3D11_CLEAR_DEPTH;
		if (clearStencil)
			flags |= D3D11_CLEAR_STENCIL;
	}
	else
	{
		if (clearStencil)
			flags = D3D11_CLEAR_STENCIL;
	}

	if (flags != 0)
		ACD3DGlobals::G_pContext->ClearDepthStencilView( pVp->pDepthStencilView, flags, 1.0f, 0 );

	return AC_OK;
};

//seta a cor de fundo
void ACD3D::SetClearColor(const Vector4& color)
{
	mClearColor[0] = color.X;
	mClearColor[1] = color.Y;
	mClearColor[2] = color.Z;
	mClearColor[3] = color.W;
};

#pragma endregion

#pragma region Log

void ACD3D::Log(char* message, ...)
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

void ACD3D::SetBlendState(ACBLENDSTATE state)
{
	if (mCurrentBlendState != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentBlendState = state;

		switch (state)
		{
			case ACBLENDSTATE::ACBS_Default:				 ACD3DConfigurations::DefineBlendStateDefault(); break;
			case ACBLENDSTATE::ACBS_Additive:				 ACD3DConfigurations::DefineBlendStateAdditive(); break;
			case ACBLENDSTATE::ACBS_AlphaBlend:				 ACD3DConfigurations::DefineBlendStateAlphaBlend(); break;
			case ACBLENDSTATE::ACBS_NonPremultiplied:		 ACD3DConfigurations::DefineBlendStateNonPremultiplied(); break;
			case ACBLENDSTATE::ACBS_Opaque:					 ACD3DConfigurations::DefineBlendStateOpaque(); break;
		}
	}
};

ACBLENDSTATE ACD3D::GetBlendState()
{
	return mCurrentBlendState;
};

void ACD3D::SetDepthBufferState(ACDEPTHBUFFERSTATE state)
{
	if (mCurrentDepthBufferState != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentDepthBufferState = state;

		switch (state)
		{
			case ACDEPTHBUFFERSTATE::ACDBS_WriteEnable:	    ACD3DConfigurations::DefineDepthStencilStateEnableDepthBuffer(); break;
			case ACDEPTHBUFFERSTATE::ACDBS_WriteDisable:	ACD3DConfigurations::DefineDepthStencilStateDisableDepthBuffer(); break;
		}
	}
};

ACDEPTHBUFFERSTATE ACD3D::GetDepthBufferState()
{
	return mCurrentDepthBufferState;
};

void ACD3D::SetRasterizeState(ACRASTERIZESTATE state)
{
	if (mCurrentRasterizationState != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentRasterizationState = state;

		switch (state)
		{
			case ACRASTERIZESTATE::ACRS_SolidCullCCW:	    
				ACD3DConfigurations::DefineRasterizeStateSolidCullCCW(); 
				mpVManager->SetIsWire(false);
				break;
			case ACRASTERIZESTATE::ACRS_WireframeCullCCW:	
				ACD3DConfigurations::DefineRasterizeStateWireframeCullCCW(); 
				mpVManager->SetIsWire(true);
			break;
			case ACRASTERIZESTATE::ACRS_SolidCullCW:	    
				ACD3DConfigurations::DefineRasterizeStateSolidCullCW(); 
				mpVManager->SetIsWire(false);
				break;
			case ACRASTERIZESTATE::ACRS_WireframeCullCW:	
				ACD3DConfigurations::DefineRasterizeStateWireframeCullCW(); 
				mpVManager->SetIsWire(true);
				break;
			case ACRASTERIZESTATE::ACRS_SolidCullNone:	    
				ACD3DConfigurations::DefineRasterizeStateSolidCullNone(); 
				mpVManager->SetIsWire(false);
				break;
			case ACRASTERIZESTATE::ACRS_WireframeCullNone:	
				ACD3DConfigurations::DefineRasterizeStateWireframeCullNone(); 
				mpVManager->SetIsWire(true);
				break;
		}
	}
};

ACRASTERIZESTATE ACD3D::GetRasterizeState()
{
	return mCurrentRasterizationState;
};

void ACD3D::SetSamplerState(ACSAMPLERSTATE state, UINT slot)
{
	if (mCurrentSamplerState[slot] != state)
	{
		mpVManager->ForcedFlushAll();
		mCurrentSamplerState[slot] = state;

		switch (state)
		{
			case ACSAMPLERSTATE::ACSS_Point_Wrap:			ACD3DConfigurations::DefineSamplerStatePointWrap(slot); break;
			case ACSAMPLERSTATE::ACSS_Bilinear_Wrap:		ACD3DConfigurations::DefineSamplerStateBilinearWrap(slot); break;
			case ACSAMPLERSTATE::ACSS_Trilinear_Wrap:		ACD3DConfigurations::DefineSamplerStateTrilinearWrap(slot); break;
			case ACSAMPLERSTATE::ACSS_Anisotropic_Wrap:		ACD3DConfigurations::DefineSamplerStateAnisotropicWrap(slot); break;

			case ACSAMPLERSTATE::ACSS_Point_Clamp:			ACD3DConfigurations::DefineSamplerStatePointClamp(slot); break;
			case ACSAMPLERSTATE::ACSS_Bilinear_Clamp:		ACD3DConfigurations::DefineSamplerStateBilinearClamp(slot); break;
			case ACSAMPLERSTATE::ACSS_Trilinear_Clamp:		ACD3DConfigurations::DefineSamplerStateTrilinearClamp(slot); break;
			case ACSAMPLERSTATE::ACSS_Anisotropic_Clamp:	ACD3DConfigurations::DefineSamplerStateAnisotropicClamp(slot); break;

			case ACSAMPLERSTATE::ACSS_Point_Border:			ACD3DConfigurations::DefineSamplerStatePointBorder(slot); break;
			case ACSAMPLERSTATE::ACSS_Bilinear_Border:		ACD3DConfigurations::DefineSamplerStateBilinearBorder(slot); break;
			case ACSAMPLERSTATE::ACSS_Trilinear_Border:		ACD3DConfigurations::DefineSamplerStateTrilinearBorder(slot); break;
			case ACSAMPLERSTATE::ACSS_Anisotropic_Border:	ACD3DConfigurations::DefineSamplerStateAnisotropicBorder(slot); break;
		}
	}
};

ACSAMPLERSTATE ACD3D::GetSamplerState(UINT slot)
{
	return mCurrentSamplerState[slot];
};

void ACD3D::SetShadeMode(ACSHADEMODE value)
{
	//se o modo de preenchimento nao for ao corrente entao ele manda renderizar tudo q ta no buffer
	if (value != mCurrentShadeMode)
	{
		mpVManager->ForcedFlushAll();
		mCurrentShadeMode = value;
	}
};

ACSHADEMODE ACD3D::GetShadeMode()
{
	return mCurrentShadeMode;
};

void ACD3D::SetWireColor(const Vector3& color)
{
	mpVManager->SetWireColor(color);
};

#pragma endregion

#pragma region Render

//comeca a renderizacao
HRESULT ACD3D::BeginRendering()
{
	return ACD3D::Clear(true, true, true);
};

void ACD3D::RenderLines(UINT numVertices, 
					UINT numIndices, 
					ACVertexPositionColored* vertices, 
					UINT* indices,
					BOOL strip)
{
	mpVManager->RenderLines(numVertices, numIndices, vertices, indices, strip);
};

//renderiza pontos usando o vb dinamico
void ACD3D::RenderPoints(UINT numVertices, 
					       ACVertexPositionColored* vertices)
{
	mpVManager->RenderPoints(numVertices, vertices);
};

//renderiza sprites, é um metodo especial usando dentro do spritebatch apenas
void ACD3D::RenderSprite(UINT numVertices, 
						   ACVertexSprite* vertices,
						   ACSkin* skin)
{
	//no caso do directx10+ eu uso point pq o sprite vai ser criado no GS
	//quando nao tem suporte a GS ele tem q criar aqui mesmo os vertices pra mandar
	SetShadeMode(ACSHADEMODE::ACSM_PointSprite);
	mpVManager->Render(VertexFormat::VF_VertexSprite, numVertices, 0, vertices, nullptr, skin);
}; 

//render dinamico de triangulos
void ACD3D::Render(VertexFormat vertexFormat, 
					 UINT numVertices, 
					 UINT numIndices, 
					 void* vertices, 
					 UINT* indices,
					 ACSkin* skin)
{
	mpVManager->Render(vertexFormat, numVertices, numIndices, vertices, indices, skin);
};

//renderiza oq tem no vb estatico
void ACD3D::Render(ACVertexBuffer* vertexBuffer)
{
	mpVManager->Render(vertexBuffer);
};

//termina a renderizacao
void ACD3D::EndRendering()
{
	//renderiza tudo q esta no vertexbuffer dinamico
	mpVManager->ForcedFlushAll();
	ACD3DVpComponents* pVp = mpVpComponents[mActiveWndRendering];
	pVp->pSwapChain->Present( 0, 0 );
};

#pragma endregion

#pragma region VB/IB

HRESULT ACD3D::CreateStaticBuffer(VertexFormat vertexFormat, 
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

void ACD3D::ReleaseBuffer(ACVertexBuffer* vertexBuffer)
{
	mpVManager->ReleaseBuffer(vertexBuffer);
};

#pragma endregion

#pragma region Textures

//carrega uma textura 2d do disco
HRESULT ACD3D::LoadTexture(std::string path, ACTexture** ppOutTexturePtr)
{
	*ppOutTexturePtr = new ACTexture();
	ZeroMemory(*ppOutTexturePtr, sizeof ( ACTexture ));

	ID3D11Resource* pTexture = nullptr;

	//carrega a textura do arquivo
	HRESULT hr = D3DX11CreateTextureFromFileA(ACD3DGlobals::G_pD3dDevice,
									path.c_str(),
									nullptr,
									nullptr,
									&pTexture,
									nullptr);

	if(hr != AC_OK)
    {
		std::string message;
        switch(hr)
        {
			case D3D11_ERROR_FILE_NOT_FOUND:  message = "[ERROR] Erro ao carregar textura. File not found: " + path;  break;
			case D3DERR_INVALIDCALL:          message = "[ERROR] Erro ao carregar textura. Invalid call: "   + path; break;
			case E_INVALIDARG:                message = "[ERROR] Erro ao carregar textura. Invalid argument: " + path; break;
			case E_OUTOFMEMORY:               message = "[ERROR] Erro ao carregar textura. Out of memory: " + path; break;
			default:                          message = "[ERROR] Erro ao carregar textura. Unknown error: " + path;
		}

		MessageBoxA(nullptr, message.c_str(), "Error", MB_OK | MB_ICONERROR);
		Log(const_cast<char*>(message.c_str()));

		return hr;
    }

	//dimension type
	D3D11_RESOURCE_DIMENSION type;
	pTexture->GetType( &type );
	switch( type )
	{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc;
			ID3D11Texture1D *pTexture1D = (ID3D11Texture1D*)pTexture;
			pTexture1D->GetDesc( &desc );

			//popula o actexture
			(*ppOutTexturePtr)->Width = desc.Width;
			(*ppOutTexturePtr)->Height = 0;
			(*ppOutTexturePtr)->MipLevels = desc.MipLevels;
			(*ppOutTexturePtr)->TextureType = ACTT_Texture1D;
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			D3D11_TEXTURE2D_DESC desc;
			ID3D11Texture2D *pTexture2D = (ID3D11Texture2D*)pTexture;
			pTexture2D->GetDesc( &desc );

			//popula o actexture
			(*ppOutTexturePtr)->Width = desc.Width;
			(*ppOutTexturePtr)->Height = desc.Height;
			(*ppOutTexturePtr)->MipLevels = desc.MipLevels;
			(*ppOutTexturePtr)->TextureType = ACTT_Texture2D;
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc;
			ID3D11Texture3D *pTexture3D = (ID3D11Texture3D*)pTexture;
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

	ID3D11ShaderResourceView* pSRV;
	hr = ACD3DGlobals::G_pD3dDevice->CreateShaderResourceView( pTexture, nullptr, &pSRV );

	if (FAILED ( hr ) )
	{
		MessageBoxA(nullptr, "[ERROR] CreateShaderResourceView. LoadTexture()", "Error", MB_OK | MB_ICONERROR);
	}

	//popula o actexture
	(*ppOutTexturePtr)->pData = pSRV;

	return AC_OK;
};

//remove a textura da memoria do buffer
HRESULT ACD3D::ReleaseTexture(ACTexture* pTexture)
{
	((ID3D11ShaderResourceView*)pTexture->pData)->Release();
	pTexture->pData = nullptr;
	return AC_OK;
};

//setar a textura para o shader, o numero do slot é a posicao da textura dentro do shader
void ACD3D::SetTexture(ACTexture* pTexture, UINT slot)
{
	if (pTexture != mpCurrentTexture[slot])
	{
		ID3D11ShaderResourceView* tex = nullptr;
		if (pTexture!=nullptr)
			tex = (ID3D11ShaderResourceView*)pTexture->pData;

		mpCurrentTexture[slot] = pTexture;
		ACD3DGlobals::G_pContext->PSSetShaderResources(slot, 1, &tex);
	}
};

#pragma endregion

#pragma region Shaders

//VERTEX SHADER
HRESULT ACD3D::CompileVS(std::string path, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr)
{
	*ppOutVertexShaderPtr = new ACVertexShader();

	//compila o VS 
	ID3DBlob* pVSBuf = nullptr;
	ID3DBlob* pError = nullptr;

	//se estiver em debug ele compila o shader para ser debugado, em release ele é otimizado
	DWORD dwShaderFlags = 0;
	#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    #endif

	HRESULT hr = 0;
	hr = D3DX11CompileFromFileA(path.c_str(), 
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
		ID3D11VertexShader* pVS;
		ACD3DGlobals::G_pD3dDevice->CreateVertexShader( ( DWORD* )pVSBuf->GetBufferPointer(),
														  pVSBuf->GetBufferSize(),
														  nullptr,
														  &pVS);

		(*ppOutVertexShaderPtr)->pVS = pVS;
		(*ppOutVertexShaderPtr)->Format = vertexFormat;

		//cria o layout de vertice
		hr = ACD3DVertexLayoutProvider::CreateInputLayout(ACD3DGlobals::G_pD3dDevice, pVSBuf, vertexFormat);
	}

	SAFE_RELEASE(pVSBuf);
	SAFE_RELEASE(pError);

	return hr;
};

HRESULT ACD3D::ReleaseVS(ACVertexShader* pVertexShader)
{
	((ID3D11VertexShader*)pVertexShader->pVS)->Release();
	pVertexShader->pVS = nullptr;
	return AC_OK;
};

void ACD3D::ActiveVS(ACVertexShader* vertexShader)
{ 
	//muda estado 
	if (vertexShader != mpCurrentVertexShader)
	{
		//se mudar o shader ele limpa o buffer
		mpVManager->ForcedFlushAll();

		mpCurrentVertexShader = vertexShader;

		if (vertexShader != nullptr)
		{
			ACD3DGlobals::G_pContext->VSSetShader( (ID3D11VertexShader*)vertexShader->pVS, nullptr, 0);

			//muda estado
			switch( vertexShader->Format)
			{
				case VF_VertexPosition: 	
					 ACD3DGlobals::G_pContext->IASetInputLayout(ACD3DVertexLayoutProvider::pVertexPositionLayout); 
				break;
				case VF_VertexPositionColored: 	
					 ACD3DGlobals::G_pContext->IASetInputLayout(ACD3DVertexLayoutProvider::pVertexPositionColoredLayout); 
				break;
				case VF_VertexPositionTextured: 	
					 ACD3DGlobals::G_pContext->IASetInputLayout(ACD3DVertexLayoutProvider::pVertexPositionTexturedLayout); 
				break;
				case VF_VertexPositionNormalTextured: 	
					 ACD3DGlobals::G_pContext->IASetInputLayout(ACD3DVertexLayoutProvider::pVertexPositionNormalTexturedLayout); 
				break;
				case VF_VertexPositionTexturedExtraInfo: 	
					 ACD3DGlobals::G_pContext->IASetInputLayout(ACD3DVertexLayoutProvider::pVertexPositionTexturedExtraInfoLayout); 
				break;
				case VF_VertexSprite: 	
					 ACD3DGlobals::G_pContext->IASetInputLayout(ACD3DVertexLayoutProvider::pVertexSpriteLayout); 
				break;
			};
		}
		else
		{
			ACD3DGlobals::G_pContext->VSSetShader( nullptr, nullptr, 0);
			ACD3DGlobals::G_pContext->IASetInputLayout( nullptr );
		}
	}
};

//PIXEL SHADER
HRESULT ACD3D::CompilePS(std::string path, ACPixelShader** ppOutPixelShaderPtr)
{
	*ppOutPixelShaderPtr = new ACPixelShader();

	//compila o PS 
	ID3DBlob* pPSBuf = nullptr;
	ID3DBlob* pError = nullptr;

	//se estiver em debug ele compila o shader para ser debugado, em release ele é otimizado
	DWORD dwShaderFlags = 0;
	#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    #endif

	HRESULT hr = 0;
	hr = D3DX11CompileFromFileA(path.c_str(), 
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
		ID3D11PixelShader* pPS;
		ACD3DGlobals::G_pD3dDevice->CreatePixelShader( ( DWORD* )pPSBuf->GetBufferPointer(),
														  pPSBuf->GetBufferSize(), 
														  nullptr,
														  &pPS);

		(*ppOutPixelShaderPtr)->pPS = pPS;

		hr = AC_OK;
	}

	SAFE_RELEASE(pPSBuf);
	SAFE_RELEASE(pError);

	return hr;
};

HRESULT ACD3D::ReleasePS(ACPixelShader* pPixelShader)
{
	((ID3D11PixelShader*)pPixelShader->pPS)->Release();
	pPixelShader->pPS = nullptr;
	return AC_OK;
};

void ACD3D::ActivePS(ACPixelShader* pixelShader)
{
	//muda estado
	if (pixelShader != mpCurrentPixelShader)
	{
		//se mudar o shader ele limpa o buffer
		mpVManager->ForcedFlushAll();

		mpCurrentPixelShader = pixelShader;

		if (pixelShader != nullptr)
			ACD3DGlobals::G_pContext->PSSetShader( (ID3D11PixelShader*)pixelShader->pPS, nullptr, 0 );
		else
			ACD3DGlobals::G_pContext->PSSetShader( nullptr, nullptr, 0 );
	}
};

//GEOMETRY SHADER
HRESULT ACD3D::CompileGS(std::string path, ACGeometryShader** ppOutGeometryShaderPtr)
{
	*ppOutGeometryShaderPtr = new ACGeometryShader();

	//compila o GS 
	ID3DBlob* pGSBuf = nullptr;
	ID3DBlob* pError = nullptr;

	//se estiver em debug ele compila o shader para ser debugado, em release ele é otimizado
	DWORD dwShaderFlags = 0;
	#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    #endif

	HRESULT hr = 0;
	hr = D3DX11CompileFromFileA(path.c_str(), 
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
		ID3D11GeometryShader* pGS;
		ACD3DGlobals::G_pD3dDevice->CreateGeometryShader( ( DWORD* )pGSBuf->GetBufferPointer(),
														  pGSBuf->GetBufferSize(),
														  nullptr,
														  &pGS);

		(*ppOutGeometryShaderPtr)->pGS = pGS;

		hr = AC_OK;
	}

	SAFE_RELEASE(pGSBuf);
	SAFE_RELEASE(pError);

	return hr;
};

HRESULT ACD3D::ReleaseGS(ACGeometryShader* pGeometryShader)
{
	((ID3D11GeometryShader*)pGeometryShader->pGS)->Release();
	pGeometryShader->pGS = nullptr;
	return AC_OK;
};

void ACD3D::ActiveGS(ACGeometryShader* geometryShader)
{
	//muda estado
	if (geometryShader != mpCurrentGeometryShader)
	{
		//se mudar o shader ele limpa o buffer
		mpVManager->ForcedFlushAll();

		mpCurrentGeometryShader = geometryShader;
		if (geometryShader!=nullptr)
			ACD3DGlobals::G_pContext->GSSetShader( (ID3D11GeometryShader*)geometryShader->pGS, nullptr, 0 );
		else
			ACD3DGlobals::G_pContext->GSSetShader( nullptr, nullptr, 0 );
	}
};

#pragma endregion

#pragma region Shaders Constants
void ACD3D::TransposeMatrixToColumnMajor(const Matrix& source, Matrix& dest)
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

void ACD3D::SetWorldMatrix(const Matrix& value)
{
	Matrix currentWorld = mVSCBPerModelData.xWorld;
	TransposeMatrixToColumnMajor(value, mVSCBPerModelData.xWorld);

	//se nao for a mesma matrix world entao ele da o flush nos buffer e ja seta
	if (currentWorld != mVSCBPerModelData.xWorld)
	{
		mpVManager->ForcedFlushAll();

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(ACD3DGlobals::G_pContext->Map(mpVSCBPerModel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		{
			MessageBoxA(nullptr, "Erro ao atualizar cb world.", "Erro", MB_OK | MB_ICONERROR);
			return;
		}

		VS_CB_PERMODEL* pCBPerModelData = (VS_CB_PERMODEL*)mappedResource.pData;
		pCBPerModelData->xWorld = mVSCBPerModelData.xWorld;
		ACD3DGlobals::G_pContext->Unmap(mpVSCBPerModel, 0);
		ACD3DGlobals::G_pContext->VSSetConstantBuffers(1, 1, &mpVSCBPerModel);
	}
};

void ACD3D::SetViewProjectionMatrix(const Matrix& value)
{
	mChangeCBPerPass = true;
	TransposeMatrixToColumnMajor(value, mVSCBPerPassData.xViewProjection);
};

void ACD3D::SetLightPosition(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightPosition = value;
};

void ACD3D::SetLightDirection(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightDirection = value;
};

void ACD3D::SetLightColor(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightColor = value;
};

void ACD3D::SetLightRange(float value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightRange = value;
};

void ACD3D::SetLightFalloff(float value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xLightFalloff = value;
};

void ACD3D::SetCameraPosition(const Vector3& value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xCameraPosition = value;
};

void ACD3D::SetTime(float value)
{
	mChangeCBPerFrame = true;
	mVSCBPerFrameData.xTime = value;
};

void ACD3D::SetMaterialAmbient(const Vector3& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialAmbient = value;
};

void ACD3D::SetMaterialEmissive(const Vector3& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialEmissive = value;
};

void ACD3D::SetMaterialSpecular(const Vector3& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialSpecular = value;
};

void ACD3D::SetMaterialSpecularPower(float value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialSpecularPower = value;
};

void ACD3D::SetMaterialDiffuse(const Vector4& value)
{
	mChangeCBMaterial = true;
	mPSCBMaterialData.xMaterialDiffuse = value;
};

void ACD3D::ApplyConstants()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//seta o constantbuffer por frame
	if (mChangeCBPerFrame)
	{
		ZeroMemory( &mappedResource, sizeof ( D3D11_MAPPED_SUBRESOURCE ) );

		if (FAILED(ACD3DGlobals::G_pContext->Map(mpVSCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		{
			MessageBoxA(nullptr, "Erro ao atualizar cb perframe.", "Erro", MB_OK | MB_ICONERROR);
			return;
		}

		VS_CB_PERFRAME* pCBPerFrameData = (VS_CB_PERFRAME*)mappedResource.pData;
		pCBPerFrameData->xCameraPosition = mVSCBPerFrameData.xCameraPosition;
		pCBPerFrameData->xLightPosition = mVSCBPerFrameData.xLightPosition;
		pCBPerFrameData->xLightDirection = mVSCBPerFrameData.xLightDirection;
		pCBPerFrameData->xLightColor = mVSCBPerFrameData.xLightColor;
		pCBPerFrameData->xLightRange = mVSCBPerFrameData.xLightRange;
		pCBPerFrameData->xLightFalloff = mVSCBPerFrameData.xLightFalloff;
		pCBPerFrameData->xTime = mVSCBPerFrameData.xTime;

		ACD3DGlobals::G_pContext->Unmap(mpVSCBPerFrame, 0);

		ACD3DGlobals::G_pContext->VSSetConstantBuffers(0, 1, &mpVSCBPerFrame);
		ACD3DGlobals::G_pContext->PSSetConstantBuffers(0, 1, &mpVSCBPerFrame);

		mChangeCBPerFrame= false;
	}

	//seta o constantbuffer por passo
	if (mChangeCBPerPass)
	{
		ZeroMemory( &mappedResource, sizeof ( D3D11_MAPPED_SUBRESOURCE ) );

		if (FAILED(ACD3DGlobals::G_pContext->Map(mpVSCBPerPass, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		{
			MessageBoxA(nullptr, "Erro ao atualizar cb perpass.", "Erro", MB_OK | MB_ICONERROR);
			return;
		}

		VS_CB_PERPASS* pCBPerPassData = (VS_CB_PERPASS*)mappedResource.pData;
		pCBPerPassData->xViewProjection = mVSCBPerPassData.xViewProjection;

		ACD3DGlobals::G_pContext->Unmap(mpVSCBPerPass, 0);

		ACD3DGlobals::G_pContext->VSSetConstantBuffers(2, 1, &mpVSCBPerPass);

		mChangeCBPerPass= false;
	}

	//seta o constantbuffer do material
	if (mChangeCBMaterial)
	{
		ZeroMemory( &mappedResource, sizeof ( D3D11_MAPPED_SUBRESOURCE ) );

		if (FAILED(ACD3DGlobals::G_pContext->Map(mpPSCBMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		{
			MessageBoxA(nullptr, "Erro ao atualizar cb material.", "Erro", MB_OK | MB_ICONERROR);
			return;
		}

		PS_CB_MATERIAL* pCBMaterialData = (PS_CB_MATERIAL*)mappedResource.pData;
		pCBMaterialData->xMaterialAmbient =  mPSCBMaterialData.xMaterialAmbient;
		pCBMaterialData->xMaterialEmissive = mPSCBMaterialData.xMaterialEmissive;
		pCBMaterialData->xMaterialDiffuse = mPSCBMaterialData.xMaterialDiffuse;
		pCBMaterialData->xMaterialSpecular = mPSCBMaterialData.xMaterialSpecular;
		pCBMaterialData->xMaterialSpecularPower = mPSCBMaterialData.xMaterialSpecularPower;

		ACD3DGlobals::G_pContext->Unmap(mpPSCBMaterial, 0);

		ACD3DGlobals::G_pContext->PSSetConstantBuffers(1, 1, &mpPSCBMaterial);

		mChangeCBMaterial = false;
	}
};

#pragma endregion

#pragma region Tools

void ACD3D::SaveScreenShot(const std::string& path)
{
	ACD3DVpComponents* vpComponent = mpCurrentVpComponents;

    ID3D11Resource* backbufferRes;
    vpComponent->pRenderTargetView->GetResource(&backbufferRes);

	if (FAILED(D3DX11SaveTextureToFileA( ACD3DGlobals::G_pContext, backbufferRes, D3DX11_IFF_JPG, path.c_str())))
		MessageBoxA(nullptr, "Error saving image. Verify folder permission.", "Error", MB_OK);

    SAFE_RELEASE( backbufferRes );
};

#pragma endregion

#pragma region RENDER TARGET MANAGER

//Cria um render target
//return: ID do rt no vetor
UINT ACD3D::CreateRenderTarget(UINT width, UINT height)
{
	ACD3DRenderToTexture* rtt = new ACD3DRenderToTexture(ACD3DGlobals::G_pD3dDevice);
	rtt->Initialize(width, height);
	mpRenderTargets.push_back(rtt);
	return mpRenderTargets.size();
};

void ACD3D::ResizeRenderTarget(UINT id, UINT width, UINT height)
{
	//remove o atual e coloca o redimensionado
	SAFE_RELEASE(mpRenderTargets[id-1]);
	ACD3DRenderToTexture* rtt = new ACD3DRenderToTexture(ACD3DGlobals::G_pD3dDevice);
	rtt->Initialize(width, height);
	mpRenderTargets[id-1] = rtt;
};

void ACD3D::RenderTargetClear(UINT id, const Vector4& color)
{
	mpRenderTargets[id-1]->ClearRenderTarget(mpCurrentVpComponents->pDepthStencilView, color);
};

void ACD3D::RenderTargetClearAll(const Vector4& color)
{
	for (auto it = mpRenderTargets.begin(); it != mpRenderTargets.end(); ++it)
		(*it)->ClearRenderTarget(mpCurrentVpComponents->pDepthStencilView, color);
};

//Zero é o default (screen)
void ACD3D::RenderTargetActivate(UINT id)
{
	//se for zero entao ele seta a tela como rendertarget
	if (id == 0)
		ACD3DGlobals::G_pContext->OMSetRenderTargets(1, &mpCurrentVpComponents->pRenderTargetView, mpCurrentVpComponents->pDepthStencilView);
	else
		mpRenderTargets[id-1]->SetRenderTarget(mpCurrentVpComponents->pDepthStencilView);
};

ACTexture* ACD3D::RenderTargetGetTexture(UINT id)
{
	return mpRenderTargets[id-1]->GetTexture();
};

#pragma endregion