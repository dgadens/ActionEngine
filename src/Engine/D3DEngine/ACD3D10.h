//************************************
//implementacao da acrenderdevice, cria um dispositivo do tipo directx 10
// creator: Daniel Gadens
// date: 20/04/2011
//***********************************

#ifndef __ACD3D10_H
#define __ACD3D10_H

#include <d3d10.h>
#include <d3dx10.h>
#include <DXGI.h>
#include <WTypes.h>
#include <map>

#include "EngineMain.h"
#include "EngineMath.h"
#include "EngineCommon.h"
#include "ACRenderDevice.h"

#include "ACCamera.h"
#include "ACD3D10Globals.h"
#include "ACD3D10Tools.h"
#include "ACD3D10Configurations.h"
#include "ACD3D10VpComponents.h"
#include "ACD3D10VertexLayoutProvider.h"
#include "ACD3D10ConstantBuffersStructs.h"
#include "ACD3D10VertexManager.h"

//herda da interface padrao
class ACD3D10 : public ACRenderDevice
{
private:

	D3D10_DRIVER_TYPE							mDriverType;
	
	std::map<HWND, ACD3D10VpComponents*>        mpVpComponents; //mantem os buffers
	ACD3D10VpComponents*					    mpCurrentVpComponents;
	BOOL										mStencil;
	BOOL										mIsSceneRunning;
	FLOAT										mClearColor[4];

	//constant buffer usando para todos os shaders
	ID3D10Buffer*								mpVSCBPerFrame;
	ID3D10Buffer*								mpVSCBPerModel;
	ID3D10Buffer*								mpVSCBPerPass;
	ID3D10Buffer*								mpPSCBMaterial;

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

	ACD3D10(HINSTANCE hDLL);
	~ACD3D10();

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
	void SetDepthBufferState(ACDEPTHBUFFERSTATE state);
	void SetRasterizeState(ACRASTERIZESTATE state);
	void SetSamplerState(ACSAMPLERSTATE state, UINT slot);

	HRESULT Clear(BOOL,BOOL,BOOL);
	void SetClearColor(const Vector4& color);

	//seta o modo de preenchimento
	void SetShadeMode(ACSHADEMODE value);
	ACSHADEMODE GetShadeMode();
	void SetWireColor(const Vector3& color);

	//tools
	//void SaveScreenShot(const std::string& path);

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
	HRESULT LoadTexture(std::string fileName, ACTexture** ppOutTexturePtr);
	HRESULT ReleaseTexture(ACTexture* pTexture);
	void SetTexture(ACTexture* pTexture, UINT slot);

	//shaders
	HRESULT CompileVS(std::string fileName, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr);
	HRESULT ReleaseVS(ACVertexShader* pVertexShader);
	void ActiveVS(ACVertexShader* vertexShader);

	HRESULT CompilePS(std::string fileName, ACPixelShader** ppOutPixelShaderPtr);
	HRESULT ReleasePS(ACPixelShader* pPixelShader);
	void ActivePS(ACPixelShader* pixelShader);

	HRESULT CompileGS(std::string fileName, ACGeometryShader** ppOutGeometryShaderPtr);
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
//Na real é um delegate e a assinatura esta dentro do ACD3D10.h dentro da dll
DLLEXPORT HRESULT CreateRenderDevice(HINSTANCE hDLL, ACRenderDevice **pInterface);
DLLEXPORT HRESULT ReleaseRenderDevice(ACRenderDevice **pInterface);

#endif