//******************************
// Classe que controla os buffers
// Creator: Daniel Gadens
// Date: 01/06/2012
//*****************************

#ifndef __ACD3DVertexManager_H
#define __ACD3DVertexManager_H

#include <d3d11.h>

#include "EngineMath.h"
#include "ACVertexFormat.h"
#include "ACRenderDevice.h"
#include "ACD3DVertexCache.h"

const UINT NUM_CACHES = 10;

class ACSkin;
class ACD3D;

class ACD3DVertexManager : public ACVertexManager
{
	ID3D11Device*           mpGDevice;
	ID3D11DeviceContext*    mpContext;
	
	ACD3DVertexCache*		mpCacheLines;             //criei um vb dinamico exclusivo para armazenar so linhas
	ACD3DVertexCache*		mpCachePoints;            //criei um vb dinamico exclusivo para armazenar so pontos

	ACD3DVertexCache*		mpCacheP[NUM_CACHES];     // position
	ACD3DVertexCache*		mpCachePC[NUM_CACHES];    // position colored
	ACD3DVertexCache*		mpCachePT[NUM_CACHES];    // position textured
	ACD3DVertexCache*		mpCachePNT[NUM_CACHES];   // position normal textured
	ACD3DVertexCache*		mpCachePTE[NUM_CACHES];   // position textured extra info
	ACD3DVertexCache*		mpCacheSM[NUM_CACHES];    // skinnedmesh
	ACD3DVertexCache*		mpCacheSP[NUM_CACHES];    // sprite

	UINT					mMaxVertices; //numero maximo de vertices dentro do buffer
	UINT					mMaxIndices;  //numero maximo de indices no buffer

	ACD3D*					mpACD3D;   //referencia local para a classe principal, usando para poder setar os materiais

public:
	ACD3DVertexManager(ACD3D* pACD3D, 
  				       ID3D11Device* gDevice, 
					   UINT maxVertices, 
					   UINT maxIndices, 
					   FILE* pLog);

	~ACD3DVertexManager();

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

	ACD3D* GetACD3D(void) { return mpACD3D; };
};

#endif