#pragma once

#include "MathConverter.h"
#include "ACRenderDevice.h"
#include "ACContentManagerW.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EngineWrapper
{
public ref class ACRenderDeviceW
{
	public:
		ACRenderDevice* Value;

		ACRenderDeviceW(ACRenderDevice* device);

		void SetContentManager(ACContentManagerW^ cManager);

		//init
		void Init(IntPtr windowPtr, bool enableVSync, bool log);
		void AddViewport(IntPtr windowPtr, BOOL enableVSync);
		void DropViewport(IntPtr windowPtr);
		void SetActiveViewport(IntPtr windowPtr);			//seta a janela ativa (onde as transformacoes serao feitas)
		void SetActiveRenderingViewport(IntPtr windowPtr);	//seta a janela de renderizacao ativa 
		void GoFullscreen(BOOL value);
		void Resize(UINT width, UINT height);
		void Release();

		//atributos VP
		UINT GetVPWidth();							//retorna a largura da janela de renderizacao ativa
		UINT GetVPHeight();						//retorna a altura da janela de renderizacao ativa

		//states
		void SetBlendState(ACBLENDSTATE state);
		ACBLENDSTATE GetBlendState();
		void SetDepthBufferState(ACDEPTHBUFFERSTATE state);
		ACDEPTHBUFFERSTATE GetDepthBufferState();
		void SetRasterizeState(ACRASTERIZESTATE state);
		ACRASTERIZESTATE GetRasterizeState();
		void SetSamplerState(ACSAMPLERSTATE state, UINT slot);
		ACSAMPLERSTATE GetSamplerState(UINT slot);

		//clear
		void Clear(BOOL clearPixels, BOOL clearDepth, BOOL clearStencil);
		void SetClearColor(EngineMathCLI::Vector4^ color);

		//modo de preenchimento
		void SetShadeMode(ACSHADEMODE value);
		ACSHADEMODE GetShadeMode();
		void SetWireColor(EngineMathCLI::Vector3^ color);

		//RenderTargetManager
		UINT CreateRenderTarget(UINT width, UINT height);
		void RenderTargetClear(UINT id, EngineMathCLI::Vector4^ color);
		void RenderTargetClearAll(EngineMathCLI::Vector4^ color);
		void RenderTargetActivate(UINT id); //0 é o defaut ou seja a tela
		ACTexture* RenderTargetGetTexture(UINT id);
		void ResizeRenderTarget(UINT id, UINT width, UINT height);

		//tools
		void SaveScreenShot(const std::string& path);

		//render
		void BeginRendering();
		//renderiza as linhas usando o vbdinamico
		void RenderLines(UINT numVertices, 
									UINT numIndices, 
									ACVertexPositionColored* vertices, 
									UINT* indices,
									BOOL strip); 

		//renderiza pontos usando o vb dinamico
		void RenderPoints(UINT numVertices, 
									ACVertexPositionColored* vertices); 

		//renderiza pontos usando o vb dinamico, pe protegido pq o usuario usa o spritebacth pra renderizar
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

		//buffers
		void CreateStaticBuffer(VertexFormat vertexFormat, 
											UINT numVertices, 
											UINT numIndices, 
											const void* pVertices, 
											const UINT* pIndices, 
											ACSkin* pSkin,
											ACVertexBuffer** ppOutVertexBufferPtr);

		void ReleaseBuffer(ACVertexBuffer* vertexBuffer);

		//textures
		void LoadTexture(std::string path, ACTexture** ppOutTexturePtr);
		void ReleaseTexture(ACTexture* pTexture);
		void SetTexture(ACTexture* pTexture, UINT slot);

		//shaders
		void CompileVS(std::string path, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr);
		void ReleaseVS(ACVertexShader* pVertexShader);
		void ActiveVS(ACVertexShader* vertexShader);

		void CompilePS(std::string path, ACPixelShader** ppOutPixelShaderPtr);
		void ReleasePS(ACPixelShader* pPixelShader);
		void ActivePS(ACPixelShader* pixelShader);

		void CompileGS(std::string path, ACGeometryShader** ppOutGeometryShaderPtr);
		void ReleaseGS(ACGeometryShader* pGeometryShader);
		void ActiveGS(ACGeometryShader* geometryShader);

		//shaders constants
		void SetWorldMatrix(EngineMathCLI::Matrix^ value);
		void SetSkinMatrizes(UINT numOfMatrizes, array<EngineMathCLI::Matrix>^ value);
		void SetViewProjectionMatrix(EngineMathCLI::Matrix^ value);
		void SetCameraPosition(EngineMathCLI::Vector3^ value);
		void SetTime(float time);
	
		//light
		void SetLightPosition(EngineMathCLI::Vector3^ value);
		void SetLightDirection(EngineMathCLI::Vector3^ value);
		void SetLightColor(EngineMathCLI::Vector3^ value);
		void SetLightRange(float value);
		void SetLightFalloff(float value);
	
		//material
		void SetMaterialAmbient(EngineMathCLI::Vector3^ value);
		void SetMaterialEmissive(EngineMathCLI::Vector3^ value);
		void SetMaterialSpecular(EngineMathCLI::Vector3^ value);
		void SetMaterialSpecularPower(float value);
		void SetMaterialDiffuse(EngineMathCLI::Vector4^ value);

		void ApplyConstants();

	};
}
