
#ifndef __ACD3D10GLOBALS_H
#define __ACD3D10GLOBALS_H

#include <D3D10.h>

class ACD3D10VertexManager;

class ACD3D10Globals
{
public:
	static ID3D10Device*			G_pD3dDevice;
	static ACD3D10VertexManager*	G_pVertexManager;
	static UINT						G_Numerator;
	static UINT						G_Denomerator;
};

#endif