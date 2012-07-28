#include "ACD3D10RenderToTexture.h"

ACD3D10RenderToTexture::ACD3D10RenderToTexture(ID3D10Device* gDevice)
{
	mpGDevice = gDevice;
	mpRenderTargetTexture = nullptr;
	mpRenderTargetView = nullptr;
	mpShaderResourceView = nullptr;
};

ACD3D10RenderToTexture::~ACD3D10RenderToTexture()
{
	Release();
};

void ACD3D10RenderToTexture::Release()
{
	SAFE_RELEASE(mpShaderResourceView);
	SAFE_RELEASE(mpRenderTargetView);
	SAFE_RELEASE(mpRenderTargetTexture);
};

bool ACD3D10RenderToTexture::Initialize(int width, int height)
{
	D3D10_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
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
	renderTargetViewDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = mpGDevice->CreateRenderTargetView(mpRenderTargetTexture, &renderTargetViewDesc, &mpRenderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = mpGDevice->CreateShaderResourceView(mpRenderTargetTexture, &shaderResourceViewDesc, &mpShaderResourceView);
	if(FAILED(result))
	{
		return false;
	}

	return true;
};

void ACD3D10RenderToTexture::SetRenderTarget(ID3D10DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mpGDevice->OMSetRenderTargets(1, &mpRenderTargetView, depthStencilView);
};

void ACD3D10RenderToTexture::ClearRenderTarget(ID3D10DepthStencilView* depthStencilView, 
					   float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	mpGDevice->ClearRenderTargetView(mpRenderTargetView, color);
    
	// Clear the depth buffer.
	mpGDevice->ClearDepthStencilView(depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
};

ID3D10ShaderResourceView* ACD3D10RenderToTexture::GetShaderResourceView()
{
	return mpShaderResourceView;
}

ID3D10Texture2D* ACD3D10RenderToTexture::GetTexture()
{
	return mpRenderTargetTexture;
};
