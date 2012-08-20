
#ifndef __ACD3DGLOBALS_H
#define __ACD3DGLOBALS_H

#include <D3D11.h>

class ACD3DVertexManager;

class ACD3DGlobals
{
public:
	static ID3D11Device*			G_pD3dDevice;
	static ID3D11DeviceContext*		G_pContext;
	static ACD3DVertexManager*		G_pVertexManager;
	static UINT						G_Numerator;
	static UINT						G_Denomerator;
};

#endif