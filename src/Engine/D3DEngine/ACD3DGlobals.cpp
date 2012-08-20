#include "ACD3DGlobals.h"

ID3D11Device*			ACD3DGlobals::G_pD3dDevice;
ID3D11DeviceContext*	ACD3DGlobals::G_pContext;
ACD3DVertexManager*		ACD3DGlobals::G_pVertexManager;

UINT					ACD3DGlobals::G_Numerator; //udandos quando o vsync esta ativado
UINT					ACD3DGlobals::G_Denomerator;
