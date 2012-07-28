//************************************************
// classe generica para dispositivo grafico (ela é implementada na dll conforme o tipo de dispositivo)
// creator: Daniel Gadens
// date: 07/05/2011
//************************************************

#ifndef __ACRENDERDEVICE_H
#define __ACRENDERDEVICE_H

#define MAX_3DHWND 9

#include <stdio.h>
#include <vector>
#include <string>

#include "EngineCommon.h"
#include "ACCamera.h"
#include "ACModel.h"
#include "ACParticles.h"
#include "ACLine.h"
#include "ACTexture.h"
#include "EngineMath.h"
#include "ACContentManager.h"
#include "ACRenderDeviceEnums.h"
#include "ACVertexBuffer.h"
#include "ACVertexFormat.h"

//classe abstrata q controla os buffers de vertices
class ACVertexManager
{
protected:
	FILE*                   mpLOG;
	ACVertexBuffer*         mpCurrentVertexBuffer;
	ACSkin*					mpCurrentSkin;
	BOOL					mIsWire;
	Vector3					mWireColor;
	//usado para verificar se modificou algum cache, se nao modificou ja cai fora no flush all
	BOOL                    mChangeAnyCache;

public:
	ACVertexManager(){};
	virtual ~ACVertexManager(){};

	virtual HRESULT CreateStaticBuffer(VertexFormat vertexFormat, 
									   UINT numVertices, 
									   UINT numIndices, 
									   const void* pVertices, 
									   const UINT* pIndices,
									   ACSkin* pSkin,
									   ACVertexBuffer** ppOutVertexBufferPtr) = 0;
	virtual void ReleaseBuffer(ACVertexBuffer* vertexBuffer) = 0;

	//renderiza o estatico
	virtual HRESULT Render(ACVertexBuffer* vertexBuffer) = 0;

	//renderiza linha com vb dinamico
	virtual HRESULT RenderLines(UINT numVertices, 
								UINT numIndices, 
								ACVertexPositionColored* vertices, 
								UINT* indices,
								BOOL strip) = 0; 

	//renderiza pontos usando o vb dinamico
	virtual HRESULT RenderPoints(UINT numVertices, 
								 ACVertexPositionColored* vertices) = 0; 

	//render dinamico (faz o controle de buffers)
	virtual HRESULT Render(VertexFormat vertexFormat, 
						   UINT numVertices, 
						   UINT numIndices, 
						   void* vertices, 
						   UINT* indices, 
						   ACSkin* skin) = 0;

	//invalido o vb pq se ele renderizar usando o vbdinamico ele tem q setar denovo o estatico quando utilizar
	//usando no teste pra ver se o vb atual é o mesmo q esta sendo renderizado
	virtual void InvalidateStaticBuffer() = 0;

	//faz o flush em tudo no caso ele zera todos os buffers pq todos vao parar no rendertarget, tem q ser chamado no endrendering
	virtual HRESULT ForcedFlushAll() = 0;
	//seleciona o tipo de vertice e renderiza todos os buffers
	virtual HRESULT ForcedFlush(VertexFormat vertexFormat) = 0;

	//verifica se esta em modo wireframe
	virtual BOOL IsWire() = 0;
	virtual void SetIsWire(BOOL value) = 0;
	//pega a cor do wireframe
	virtual Vector3 GetWireColor() = 0;
	virtual void SetWireColor(const Vector3& color) = 0;

	virtual void Release() = 0;

	virtual void Log(char* message, ...) = 0;

};

//classe abstrata principal
class ACRenderDevice
{
protected:
	HWND mActiveWnd;		   //janela ativa para manipulacao
	HWND mActiveWndRendering;  //janela ativa para renderizacao
	HINSTANCE mhDLL;           //modulo dll
	BOOL mWindowed;			   //modo janela?
	CHAR mAdapterName[256];    //nome do adaptador
	FILE* mpLOG;			   //arquivo de log
	BOOL mIsFullscreen;

	ACSHADEMODE			mCurrentShadeMode;				//modo de preenchimento atual
	ACBLENDSTATE		mCurrentBlendState;				//blend atual
	ACDEPTHBUFFERSTATE	mCurrentDepthBufferState;		//depthbuffer atual
	ACRASTERIZESTATE	mCurrentRasterizationState;		//rasterization atual
	ACSAMPLERSTATE		mCurrentSamplerState[16];		//sampler atual

	ACContentManager*	mpCManager;	//contentmanager
	ACVertexManager*	mpVManager;	//manage vertices/indices

public:
	INT  VideoCardMemory;
	char VideoCardDescription[128];
	BOOL VSyncEnable;
	BOOL GeometryShaderSupport;

	BOOL IsRunning;

	ACRenderDevice(){ };
	virtual ~ACRenderDevice() {	};

	//seta o contentmanager 
	void SetContentManager(ACContentManager* cManager)
	{
		mpCManager = cManager;
	};

	//init
	virtual HRESULT Init(HWND, BOOL enableVSync, BOOL log) = 0;
	virtual HRESULT AddViewport(HWND, BOOL enableVSync) = 0;
	virtual HRESULT DropViewport(HWND) =0 ;
	virtual void SetActiveViewport(HWND) = 0;			//seta a janela ativa (onde as transformacoes serao feitas)
	virtual void SetActiveRenderingViewport(HWND)=0;	//seta a janela de renderizacao ativa 
	virtual void GoFullscreen(BOOL)=0;
	virtual HRESULT Resize(UINT width, UINT height)=0;
	virtual void Release() = 0;
	virtual void Log(char* message, ...) =0; //os 3 pontos sao para passar parametros, no caso eu uso os esquemas do printf %d %s etc

	//atributos VP
	virtual UINT GetVPWidth() = 0;							//retorna a largura da janela de renderizacao ativa
	virtual UINT GetVPHeight() = 0;						//retorna a altura da janela de renderizacao ativa

	//states
	virtual void SetBlendState(ACBLENDSTATE state) = 0;
	virtual void SetDepthBufferState(ACDEPTHBUFFERSTATE state) = 0;
	virtual void SetRasterizeState(ACRASTERIZESTATE state) = 0;
	virtual void SetSamplerState(ACSAMPLERSTATE state, UINT slot) = 0;

	//clear
	virtual HRESULT Clear(BOOL clearPixels, BOOL clearDepth, BOOL clearStencil) = 0;
	virtual void SetClearColor(const Vector4& color) = 0;

	//modo de preenchimento
	virtual void SetShadeMode(ACSHADEMODE value) = 0;
	virtual ACSHADEMODE GetShadeMode() = 0;
	virtual void SetWireColor(const Vector3& color) = 0;

	//tools
	virtual void SaveScreenShot(const std::string& path) = 0;

	//render
	virtual HRESULT BeginRendering() = 0;
	//renderiza as linhas usando o vbdinamico
	virtual void RenderLines(UINT numVertices, 
							 UINT numIndices, 
							 ACVertexPositionColored* vertices, 
							 UINT* indices,
							 BOOL strip) = 0; 

	//renderiza pontos usando o vb dinamico
	virtual void RenderPoints(UINT numVertices, 
						      ACVertexPositionColored* vertices) = 0; 

	//renderiza pontos usando o vb dinamico, pe protegido pq o usuario usa o spritebacth pra renderizar
	virtual void RenderSprite(UINT numVertices, 
							  ACVertexSprite* vertices,
						      ACSkin* skin) = 0; 

	//render dinamico de triangulos
	virtual void Render(VertexFormat vertexFormat, 
						UINT numVertices, 
						UINT numIndices, 
						void* vertices, 
						UINT* indices,
						ACSkin* skin) = 0; 

	//renderiza oq tem no vb estatico
	virtual void Render(ACVertexBuffer* vertexBuffer) = 0;  
	virtual void EndRendering() = 0;

	//buffers
	virtual HRESULT CreateStaticBuffer(VertexFormat vertexFormat, 
									   UINT numVertices, 
									   UINT numIndices, 
									   const void* pVertices, 
									   const UINT* pIndices, 
									   ACSkin* pSkin,
									   ACVertexBuffer** ppOutVertexBufferPtr) = 0;

	virtual void ReleaseBuffer(ACVertexBuffer* vertexBuffer) = 0;

	//textures
	virtual HRESULT LoadTexture(std::string fileName, ACTexture** ppOutTexturePtr) = 0;
	virtual HRESULT ReleaseTexture(ACTexture* pTexture) = 0;
	virtual void SetTexture(ACTexture* pTexture, UINT slot) = 0;

	//shaders
	virtual HRESULT CompileVS(std::string fileName, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr) = 0;
	virtual HRESULT ReleaseVS(ACVertexShader* pVertexShader) = 0;
	virtual void ActiveVS(ACVertexShader* vertexShader) = 0;

	virtual HRESULT CompilePS(std::string fileName, ACPixelShader** ppOutPixelShaderPtr) = 0;
	virtual HRESULT ReleasePS(ACPixelShader* pPixelShader) = 0;
	virtual void ActivePS(ACPixelShader* pixelShader) = 0;

	virtual HRESULT CompileGS(std::string fileName, ACGeometryShader** ppOutGeometryShaderPtr) = 0;
	virtual HRESULT ReleaseGS(ACGeometryShader* pGeometryShader) = 0;
	virtual void ActiveGS(ACGeometryShader* geometryShader) = 0;

	//shaders constants
	virtual void SetWorldMatrix(const Matrix& value) = 0;
	virtual void SetViewProjectionMatrix(const Matrix& value) = 0;
	virtual void SetCameraPosition(const Vector3& value) = 0;
	virtual void SetTime(float time) = 0;
	
	//light
	virtual void SetLightPosition(const Vector3& value) = 0;
	virtual void SetLightDirection(const Vector3& value) = 0;
	virtual void SetLightColor(const Vector3& value) = 0;
	virtual void SetLightRange(float value) = 0;
	virtual void SetLightFalloff(float value) = 0;
	
	//material
	virtual void SetMaterialAmbient(const Vector3& value) = 0;
	virtual void SetMaterialEmissive(const Vector3& value) = 0;
	virtual void SetMaterialSpecular(const Vector3& value) = 0;
	virtual void SetMaterialSpecularPower(float value) = 0;
	virtual void SetMaterialDiffuse(const Vector4& value) = 0;

	virtual void ApplyConstants() = 0;
};

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACD3D10.h dentro da dll
typedef HRESULT (*CREATERENDERDEVICE) (HINSTANCE hDLL, ACRenderDevice **pInterface);
typedef HRESULT (*RELEASERENDERDEVICE) (ACRenderDevice **pInterface);

#endif

