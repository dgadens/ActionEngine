//********************************
// Renderiza em uma textura, utilizado para salvar o SS e tb para efeitos de postprocessing
// Creator: Daniel Gadens
// Date: 26/07/2012
//********************************

#ifndef __ACD3D10RENDERTOTEXTURE_H
#define __ACD3D10RENDERTOTEXTURE_H

#include <d3d10.h>
#include "GenericTools.h"

class RenderToTexture
{
private:
	ID3D10Device*				mpGDevice;
	ID3D10Texture2D*			mpRenderTargetTexture;
	ID3D10RenderTargetView*		mpRenderTargetView;
	ID3D10ShaderResourceView*	mpShaderResourceView;

public:
	RenderToTexture(ID3D10Device* gDevice);
	~RenderToTexture();

	bool Initialize(int width, int height);
	void Release();

	void SetRenderTarget(ID3D10DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D10DepthStencilView* depthStencilView, float red, float green, float blue, float alpha);
	ID3D10ShaderResourceView* GetShaderResourceView();
};

#endif