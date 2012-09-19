//************************************
//implementacao da acrenderdevice, cria um dispositivo do tipo directx 10
// creator: Daniel Gadens
// date: 20/04/2011
//***********************************

#ifndef __ACD3D_H
#define __ACD3D_H

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DXGI.h>
#include <WTypes.h>
#include <map>

#include "EngineMain.h"
#include "EngineMath.h"
#include "EngineCommon.h"
#include "ACRenderDevice.h"

#include "ACCamera.h"
#include "ACD3DGlobals.h"
#include "ACD3DTools.h"
#include "ACD3DConfigurations.h"
#include "ACD3DVpComponents.h"
#include "ACD3DVertexLayoutProvider.h"
#include "ACD3DConstantBuffersStructs.h"
#include "ACD3DVertexManager.h"
#include "ACD3DRenderToTexture.h"

//herda da interface padrao
class ACD3D : public ACRenderDevice
{
private:

	D3D_DRIVER_TYPE								mDriverType;
	D3D_FEATURE_LEVEL							mFeatureLevel;
	
	std::map<HWND, ACD3DVpComponents*>			mpVpComponents; //mantem os buffers
	ACD3DVpComponents*							mpCurrentVpComponents;
	std::vector<ACD3DRenderToTexture*>			mpRenderTargets; //lista de rendertargets criados pelo cliente, retorna o id
	BOOL										mStencil;
	BOOL										mIsSceneRunning;
	FLOAT										mClearColor[4];

	//constant buffer usando para todos os shaders
	ID3D11Buffer*								mpVSCBPerFrame;
	ID3D11Buffer*								mpVSCBPerModel;
	ID3D11Buffer*								mpVSCBPerPass;
	ID3D11Buffer*								mpPSCBMaterial;

	VS_CB_PERFRAME                              mVSCBPerFrameData;
	VS_CB_PERMODEL                              mVSCBPerModelData;
    VS_CB_PERPASS                               mVSCBPerPassData;   
	PS_CB_MATERIAL                              mPSCBMaterialData;

	BOOL                                        mChangeCBPerFrame;
	BOOL                                        mChangeCBPerModel;
	BOOL                                        mChangeCBPerPass;
	BOOL                                        mChangeCBMaterial;

	//armazena o vbatual para testar se o proximo é igual se for entao ele nao precisa setar para o dispositivo grafico
	ACVertexBuffer*                             mpCurrentVertexBuffer;
	ACVertexShader*								mpCurrentVertexShader;
	ACPixelShader*								mpCurrentPixelShader;
	ACGeometryShader*							mpCurrentGeometryShader;
	ACTexture*									mpCurrentTexture[128]; //128 slots

	//starta a api
	HRESULT CreateGraphicsDevice(int width, int height);
	HRESULT CreateConstantBuffers();

	//usado para fazer a transposta direto quando passa para os shaders q sao ColumnMajor
	void TransposeMatrixToColumnMajor(const Matrix& source, Matrix& dest);

public:

	ACD3D(HINSTANCE hDLL);
	~ACD3D();

	//inicializacao
	HRESULT Init(HWND, BOOL enableVSync, BOOL log);
	HRESULT AddViewport(HWND, BOOL enableVSync);
	HRESULT DropViewport(HWND hWndId);
	void SetActiveViewport(HWND viewPortId);
	void SetActiveRenderingViewport(HWND viewPortId);
	void GoFullscreen(BOOL value);
	HRESULT Resize(UINT width, UINT height);    //resize da tela refaz todos os buffers e views para utiliza o tamanho passado
	UINT GetVPWidth();							//retorna a largura da janela de renderizacao ativa
	UINT GetVPHeight();  						//retorna a altura da janela de renderizacao ativa
	void Release();
	void Log(char* message, ...);

	//renderstates
	void SetBlendState(ACBLENDSTATE state);
	ACBLENDSTATE GetBlendState();
	void SetDepthBufferState(ACDEPTHBUFFERSTATE state);
	ACDEPTHBUFFERSTATE GetDepthBufferState();
	void SetRasterizeState(ACRASTERIZESTATE state);
	ACRASTERIZESTATE GetRasterizeState();
	void SetSamplerState(ACSAMPLERSTATE state, UINT slot);
	ACSAMPLERSTATE GetSamplerState(UINT slot);

	HRESULT Clear(BOOL,BOOL,BOOL);
	void SetClearColor(const Vector4& color);

	//seta o modo de preenchimento
	void SetShadeMode(ACSHADEMODE value);
	ACSHADEMODE GetShadeMode();
	void SetWireColor(const Vector3& color);

	//RenderTargetManager
	UINT CreateRenderTarget(UINT width, UINT height);
	void RenderTargetClear(UINT id, const Vector4& color);
	void RenderTargetClearAll(const Vector4& color);
	void RenderTargetActivate(UINT id); //0 é o defaut ou seja a tela
	ACTexture* RenderTargetGetTexture(UINT id);
	void ResizeRenderTarget(UINT id, UINT width, UINT height);

	//tools
	void SaveScreenShot(const std::string& path);

	//Render
	HRESULT BeginRendering();
	void RenderLines(UINT numVertices, 
					 UINT numIndices, 
					 ACVertexPositionColored* vertices, 
					 UINT* indices,
					 BOOL strip); 

	//renderiza pontos usando o vb dinamico
	void RenderPoints(UINT numVertices, 
					  ACVertexPositionColored* vertices); 

	void RenderSprite(UINT numVertices, 
					  ACVertexSprite* vertices,
				      ACSkin* skin); 

	//render dinamico de triangulos
	void Render(VertexFormat vertexFormat, 
				UINT numVertices, 
				UINT numIndices, 
				void* vertices, 
				UINT* indices,
				ACSkin* skin); 

	//renderiza oq tem no vb estatico
	void Render(ACVertexBuffer* vertexBuffer);  

	void EndRendering();

	//Vertex buffer
	HRESULT CreateStaticBuffer(VertexFormat vertexFormat, 
							   UINT numVertices, 
							   UINT numIndices, 
							   const void* pVertices, 
							   const UINT* pIndices, 
							   ACSkin* pSkin,
							   ACVertexBuffer** ppOutVertexBufferPtr);

	void ReleaseBuffer(ACVertexBuffer* vertexBuffer);
	
	//textures
	HRESULT LoadTexture(std::string path, ACTexture** ppOutTexturePtr);
	HRESULT ReleaseTexture(ACTexture* pTexture);
	void SetTexture(ACTexture* pTexture, UINT slot);

	//shaders
	HRESULT CompileVS(std::string path, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr);
	HRESULT ReleaseVS(ACVertexShader* pVertexShader);
	void ActiveVS(ACVertexShader* vertexShader);

	HRESULT CompilePS(std::string path, ACPixelShader** ppOutPixelShaderPtr);
	HRESULT ReleasePS(ACPixelShader* pPixelShader);
	void ActivePS(ACPixelShader* pixelShader);

	HRESULT CompileGS(std::string path, ACGeometryShader** ppOutGeometryShaderPtr);
	HRESULT ReleaseGS(ACGeometryShader* pGeometryShader);
	void ActiveGS(ACGeometryShader* pGeometryShader);

	//shaders constants
	void SetWorldMatrix(const Matrix& value);
	void SetViewProjectionMatrix(const Matrix& value);
	void SetCameraPosition(const Vector3& value);
	void SetTime(float time);

	//light
	void SetLightPosition(const Vector3& value);
	void SetLightDirection(const Vector3& value);
	void SetLightColor(const Vector3& value);
	void SetLightRange(float value);
	void SetLightFalloff(float value);

	//material
	void SetMaterialAmbient(const Vector3& value);
	void SetMaterialEmissive(const Vector3& value);
	void SetMaterialSpecular(const Vector3& value);
	void SetMaterialSpecularPower(float value);
	void SetMaterialDiffuse(const Vector4& value);

	void ApplyConstants();

};

//declaracao das funcoes que vao ser externadas pela dll. 
//Na real é um delegate e a assinatura esta dentro do ACD3D.h dentro da dll
DLLEXPORT HRESULT CreateRenderDevice(HINSTANCE hDLL, ACRenderDevice **pInterface);
DLLEXPORT HRESULT ReleaseRenderDevice(ACRenderDevice **pInterface);

#endif