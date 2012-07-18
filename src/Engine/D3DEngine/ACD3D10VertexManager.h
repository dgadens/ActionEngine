//******************************
// Classe que controla os buffers
// Creator: Daniel Gadens
// Date: 01/06/2012
//*****************************

#ifndef __ACD3D10VERTEXMANAGER_H
#define __ACD3D10VERTEXMANAGER_H

#include <d3d10.h>

#include "EngineMath.h"
#include "ACVertexFormat.h"
#include "ACRenderDevice.h"
#include "ACD3D10VertexCache.h"

class ACSkin;
class ACD3D10;

class ACD3D10VertexManager : public ACVertexManager
{
	ID3D10Device*           mpGDevice;
	
	ACD3D10VertexCache*		mpCacheLines;             //criei um vb dinamico exclusivo para armazenar so linhas
	ACD3D10VertexCache*		mpCachePoints;            //criei um vb dinamico exclusivo para armazenar so pontos

	ACD3D10VertexCache*		mpCacheP[NUM_CACHES];     // position
	ACD3D10VertexCache*		mpCachePC[NUM_CACHES];    // position colored
	ACD3D10VertexCache*		mpCachePT[NUM_CACHES];    // position textured
	ACD3D10VertexCache*		mpCachePNT[NUM_CACHES];   // position normal textured
	ACD3D10VertexCache*		mpCachePTE[NUM_CACHES];   // position textured extra info
	ACD3D10VertexCache*		mpCacheSM[NUM_CACHES];    // skinnedmesh
	ACD3D10VertexCache*		mpCacheSP[NUM_CACHES];    // sprite

	UINT					mMaxVertices; //numero maximo de vertices dentro do buffer
	UINT					mMaxIndices;  //numero maximo de indices no buffer

	ACD3D10*				mpACD3D10;   //referencia local para a classe principal, usando para poder setar os materiais

public:
	ACD3D10VertexManager(ACD3D10* pACD3D10, 
					     ID3D10Device* gDevice, 
						 UINT maxVertices, 
						 UINT maxIndices, 
						 FILE* pLog);

	~ACD3D10VertexManager();

	HRESULT CreateStaticBuffer(VertexFormat vertexFormat, 
							   UINT numVertices, 
							   UINT numIndices, 
							   const void* pVertices, 
							   const UINT* pIndices,
							   ACSkin* pSkin,
							   ACVertexBuffer** ppOutVertexBufferPtr);

	void ReleaseBuffer(ACVertexBuffer* vertexBuffer);

	HRESULT Render(ACVertexBuffer* vertexBuffer);

	//renderiza linhas
	HRESULT RenderLines(UINT numVertices, 
						UINT numIndices, 
						ACVertexPositionColored* vertices, 
						UINT* indices,
						BOOL strip); 

	//renderiza pontos usando o vb dinamico
	HRESULT RenderPoints(UINT numVertices, 
						 ACVertexPositionColored* vertices); 

	//render dinamico (faz o controle de buffers)
	HRESULT Render(VertexFormat vertexFormat, 
				   UINT numVertices, 
				   UINT numIndices, 
				   void* vertices, 
				   UINT* indices, 
				   ACSkin* skin);

	void InvalidateStaticBuffer();

	//faz o flush em tudo no caso ele zera todos os buffers pq todos vao parar no rendertarget, tem q ser chamado no endrendering
	HRESULT ForcedFlushAll();
	//seleciona o tipo de vertice e renderiza todos os buffers
	HRESULT ForcedFlush(VertexFormat vertexFormat);

	//seta o wire
	BOOL IsWire();
	void SetIsWire(BOOL value);
	//pega a cor do wireframe
	Vector3 GetWireColor();
	void SetWireColor(const Vector3& color);

	void Release();

	void Log(char* message, ...);

	ACD3D10* GetACD3D10(void) { return mpACD3D10; };
};

#endif