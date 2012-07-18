//********************************
// Estruturas com o ponteiro para os shaders
// Creator: Daniel Gadens
// Date: 02/05/2012
//*******************************

#ifndef __ACSHADERS_H
#define __ACSHADERS_H

#include <string>

struct ACVertexShader
{
	UINT ID;				//id dentro do contentmanager 
	std::string Name;
	UINT Instance;
	VertexFormat Format;
	void* pVS;
};

struct ACPixelShader
{
	UINT ID;
	std::string Name;
	UINT Instance;
	void* pPS;
};

struct ACGeometryShader
{
	UINT ID;				
	std::string Name;
	UINT Instance;
	void* pGS;
};

#endif