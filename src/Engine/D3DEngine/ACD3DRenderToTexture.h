//********************************
// Renderiza em uma textura, utilizado para salvar o SS e tb para efeitos de postprocessing
// Creator: Daniel Gadens
// Date: 26/07/2012
//********************************

#ifndef __ACD3DRENDERTOTEXTURE_H
#define __ACD3DRENDERTOTEXTURE_H

#include <d3d11.h>
#include "GenericTools.h"
#include "EngineMath.h"
#include "ACTexture.h"

class ACD3DRenderToTexture
{
private:
	ID3D11Device*				mpGDevice;
	ID3D11DeviceContext*		mpContext;
	ID3D11Texture2D*			mpRenderTargetTexture;
	ID3D11RenderTargetView*		mpRenderTargetView;
	ID3D11ShaderResourceView*	mpShaderResourceView;

	ACTexture*					mpTexture;

public:
	ACD3DRenderToTexture(ID3D11Device* gDevice);
	~ACD3DRenderToTexture();

	bool Initialize(int width, int height);
	void Release();

	void SetRenderTarget(ID3D11DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D11DepthStencilView* depthStencilView, const Vector4& color);
	ID3D11ShaderResourceView* GetShaderResourceView();
	ACTexture* GetTexture();
};

#endif