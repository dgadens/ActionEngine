#include "ACD3DRenderToTexture.h"

ACD3DRenderToTexture::ACD3DRenderToTexture(ID3D11Device* gDevice)
{
	mpGDevice = gDevice;
	mpGDevice->GetImmediateContext(&mpContext);

	mpRenderTargetTexture = nullptr;
	mpRenderTargetView = nullptr;
	mpShaderResourceView = nullptr;
	mpTexture = nullptr;
};

ACD3DRenderToTexture::~ACD3DRenderToTexture()
{
	Release();
};

void ACD3DRenderToTexture::Release()
{
	SAFE_RELEASE(mpShaderResourceView);
	SAFE_RELEASE(mpRenderTargetView);
	SAFE_RELEASE(mpRenderTargetTexture);
};

bool ACD3DRenderToTexture::Initialize(int width, int height)
{
	//cria a textura dx11
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = mpGDevice->CreateTexture2D(&textureDesc, NULL, &mpRenderTargetTexture);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = mpGDevice->CreateRenderTargetView(mpRenderTargetTexture, &renderTargetViewDesc, &mpRenderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = mpGDevice->CreateShaderResourceView(mpRenderTargetTexture, &shaderResourceViewDesc, &mpShaderResourceView);
	if(FAILED(result))
	{
		return false;
	}

	//cria a textura AC
	mpTexture = new ACTexture();
	ZeroMemory(mpTexture, sizeof ( ACTexture ));
	mpTexture->Alpha = 1.0f;
	mpTexture->Width = width;
	mpTexture->Height = height;
	mpTexture->TextureType = ACTEXTURETYPE::ACTT_Texture2D;
	mpTexture->pData = mpShaderResourceView;
	mpTexture->IsRenderTarget = true;

	return true;
};

void ACD3DRenderToTexture::SetRenderTarget(ID3D11DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mpContext->OMSetRenderTargets(1, &mpRenderTargetView, depthStencilView);
};

void ACD3DRenderToTexture::ClearRenderTarget(ID3D11DepthStencilView* depthStencilView, const Vector4& color)
{
	float c[4];
	c[0] = color.X;
	c[1] = color.Y;
	c[2] = color.Z;
	c[3] = color.W;

	// Clear the back buffer.
	mpContext->ClearRenderTargetView(mpRenderTargetView, c);
    
	// Clear the depth buffer.
	mpContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
};

ID3D11ShaderResourceView* ACD3DRenderToTexture::GetShaderResourceView()
{
	return mpShaderResourceView;
};

ACTexture* ACD3DRenderToTexture::GetTexture()
{
	return mpTexture;
};
