//************************************
// Estrutura da textura como é indepentende da api ele tem o ponteiro void q vai ser usado conforme o loader da api
// creator: Daniel Gadens
// date: 19/05/2011
//**********************************

#ifndef __ACTEXTURE_H
#define __ACTEXTURE_H

#include <string>
#include "ActionTypeDef.h"
#include "ACRenderDeviceEnums.h"

struct ACTexture
{
	UINT			ID;             //id dentro do contentmanager
	UINT			Instance;       //numero de instancias q usam esse cara
	std::string		Name;      
	FLOAT			Alpha;
	int				Width;
	int				Height;
	int             Depth;
	int				MipLevels;
	ACTEXTURETYPE	TextureType;
	void			*pData;
};

#endif
