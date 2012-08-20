//***************************
// Classe que constroi todos os formatos de vertices
// Creator: Daniel Gadens
// Date: 05/05/2012
//***************************

#ifndef __ACD3DVERTEXLAYOUTPROVIDER_H
#define __ACD3DVERTEXLAYOUTPROVIDER_H

#include <d3d11.h>
#include "EngineCommon.h"

class ACD3DVertexLayoutProvider
{

public:
	static ID3D11InputLayout* pVertexPositionLayout;
	static ID3D11InputLayout* pVertexPositionColoredLayout;
	static ID3D11InputLayout* pVertexPositionTexturedLayout;
	static ID3D11InputLayout* pVertexPositionNormalTexturedLayout;
	static ID3D11InputLayout* pVertexPositionTexturedExtraInfoLayout;
	static ID3D11InputLayout* pVertexSpriteLayout;

	static HRESULT CreateInputLayout(ID3D11Device* gDevice, ID3DBlob* pVSBuf, VertexFormat vertexFormat);
	static void ReleaseAll();
};

#endif