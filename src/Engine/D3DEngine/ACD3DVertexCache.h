//******************************
// Classe que controla os buffers
// Creator: Daniel Gadens
// Date: 01/06/2012
//*****************************

#ifndef __ACD3DVERTEXCACHE_H
#define __ACD3DVERTEXCACHE_H

#include <d3d11.h>

#include "EngineMath.h"
#include "ACSkin.h"
#include "ACVertexFormat.h"

class ACD3DVertexManager;

//classe q representa o vb dinamico
class ACD3DVertexCache
{
	ID3D11Device*			mpGDevice;
	ID3D11DeviceContext*	mpContext;
	ACD3DVertexManager*		mpVManager;
	FILE*					mpLOG;
	ACSkin*					mpSkin;         //skin atual quando troca ele faz o flush
	ID3D11Buffer*			mpVB;
	ID3D11Buffer*			mpIB;

	UINT  mNumVerticesMax;  // numero maximo de vertices no buffer
	UINT  mNumIndicesMax;   // numero maximo de indices no buffer
	UINT  mStride;			// tamanho de 1 vertices

public:
	BOOL  IsEmpty;
	UINT  NumVertices;		// numero atual de vertices no buffer
	UINT  NumIndices;		// numero atual de indices no buffer

	ACD3DVertexCache(ACD3DVertexManager* vManager,
					 ID3D11Device* gDevice,
					 UINT verticesMax, 
					 UINT indicesMax, 
                     UINT nStride,  
                     VertexFormat vFormat, 
					 FILE* log);

	~ACD3DVertexCache();

	//retorna se usa o skin passado como parametro
	BOOL UseSkin(ACSkin* skin);
	//adiciona uma lista de vertices e indices ao buffer e ja vincula a um skin
	HRESULT Add(UINT numVertices, UINT numIndices, void* vertices, UINT* indices, ACSkin* skin);
	//manda renderizar ou pq esta lotado o buffer ou pq trocou de skin
	HRESULT Flush();

	void Log(char* message, ...);
};

#endif