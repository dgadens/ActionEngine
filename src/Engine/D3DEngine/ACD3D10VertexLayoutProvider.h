//***************************
// Classe que constroi todos os formatos de vertices
// Creator: Daniel Gadens
// Date: 05/05/2012
//***************************

#ifndef __ACD3D10VERTEXLAYOUTPROVIDER_H
#define __ACD3D10VERTEXLAYOUTPROVIDER_H

#include <d3d10.h>
#include "EngineCommon.h"

class ACD3D10VertexLayoutProvider
{

public:
	static ID3D10InputLayout* pVertexPositionLayout;
	static ID3D10InputLayout* pVertexPositionColoredLayout;
	static ID3D10InputLayout* pVertexPositionTexturedLayout;
	static ID3D10InputLayout* pVertexPositionNormalTexturedLayout;
	static ID3D10InputLayout* pVertexPositionTexturedExtraInfoLayout;
	static ID3D10InputLayout* pVertexSpriteLayout;

	static HRESULT CreateInputLayout(ID3D10Device* gDevice, ID3D10Blob* pVSBuf, VertexFormat vertexFormat);
};

#endif