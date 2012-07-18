//*******************************
// Estrutura com os dados do vb e tb o ponteiro para o VB e IB
// Creator: Daniel Gadens
// Date: 12/05/2012
//******************************

#ifndef __ACVERTEXBUFFER_H
#define __ACVERTEXBUFFER_H

#include "EngineCommon.h"
class ACSkin;

struct ACVertexBuffer
{
	UINT Stride;
	INT NumVertices;
	INT NumIndices;
	INT NumPrimitives;
	VertexFormat VFormat;
	ACSkin* Skin;
	void* pVB;
	void* pIB;

	ACVertexBuffer()
	{
		pVB = nullptr;
		pIB = nullptr;
	};
};

#endif