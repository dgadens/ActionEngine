#include "StdAfx.h"
#include "ACRenderDeviceW.h"

using namespace EngineWrapper;

ACRenderDeviceW::ACRenderDeviceW(ACRenderDevice* device)
{
	Value = device;
}

ACRenderDeviceW::~ACRenderDeviceW()
{
	Release();
}

void ACRenderDeviceW::Release()
{
	Value->Release();
}

void ACRenderDeviceW::SetContentManager(ACContentManagerW^ cManager)
{
	Value->SetContentManager(cManager->Value);
}

void ACRenderDeviceW::Init(IntPtr windowPtr, bool enableVSync, bool log)
{
	Value->Init((HWND)windowPtr.ToPointer(), enableVSync ? 1 : 0, log ? 1 : 0);
}

void ACRenderDeviceW::AddViewport(IntPtr windowPtr, BOOL enableVSync)
{
	Value->AddViewport((HWND)windowPtr.ToPointer(), enableVSync);
}

void ACRenderDeviceW::DropViewport(IntPtr windowPtr)
{
	Value->DropViewport((HWND)windowPtr.ToPointer());
}

void ACRenderDeviceW::SetActiveViewport(IntPtr windowPtr)
{
	Value->SetActiveViewport((HWND)windowPtr.ToPointer());
}

void ACRenderDeviceW::SetActiveRenderingViewport(IntPtr windowPtr)
{
	Value->SetActiveRenderingViewport((HWND)windowPtr.ToPointer());
}

void ACRenderDeviceW::Resize(UINT width, UINT height)
{
	Value->Resize(width, height);
}

UINT ACRenderDeviceW::GetVPWidth()
{
	return Value->GetVPWidth();
}

UINT ACRenderDeviceW::GetVPHeight()
{
	return Value->GetVPHeight();
}

//states
void ACRenderDeviceW::SetBlendState(ACBLENDSTATE state)
{
	Value->SetBlendState(state);
}

ACBLENDSTATE ACRenderDeviceW::GetBlendState()
{
	return Value->GetBlendState();
}

void ACRenderDeviceW::SetDepthBufferState(ACDEPTHBUFFERSTATE state)
{
	Value->SetDepthBufferState(state);
}

ACDEPTHBUFFERSTATE ACRenderDeviceW::GetDepthBufferState()
{
	return Value->GetDepthBufferState();
}

void ACRenderDeviceW::SetRasterizeState(ACRASTERIZESTATE state)
{
	Value->SetRasterizeState(state);
}

ACRASTERIZESTATE ACRenderDeviceW::GetRasterizeState()
{
	return Value->GetRasterizeState();
}

void ACRenderDeviceW::SetSamplerState(ACSAMPLERSTATE state, UINT slot)
{
	Value->SetSamplerState(state, slot);
}

ACSAMPLERSTATE ACRenderDeviceW::GetSamplerState(UINT slot)
{
	return Value->GetSamplerState(slot);
}

//clear
void ACRenderDeviceW::Clear(BOOL clearPixels, BOOL clearDepth, BOOL clearStencil)
{
	Value->Clear(clearPixels, clearDepth, clearStencil);
}

void ACRenderDeviceW::SetClearColor(EngineMathCLI::Vector4^ color)
{
	Value->SetClearColor(MathConverter::ToUnmanaged(color));
}

//modo de preenchimento
void ACRenderDeviceW::SetShadeMode(ACSHADEMODE value)
{
	Value->SetShadeMode(value);
}

ACSHADEMODE ACRenderDeviceW::GetShadeMode()
{
	return Value->GetShadeMode();
}

void ACRenderDeviceW::SetWireColor(EngineMathCLI::Vector3^ color)
{
	Value->SetWireColor(MathConverter::ToUnmanaged(color));
}

//RenderTargetManager
UINT ACRenderDeviceW::CreateRenderTarget(UINT width, UINT height)
{
	return Value->CreateRenderTarget(width, height);
}

void ACRenderDeviceW::RenderTargetClear(UINT id, EngineMathCLI::Vector4^ color)
{
	Value->RenderTargetClear(id, MathConverter::ToUnmanaged(color));
}

void ACRenderDeviceW::RenderTargetClearAll(EngineMathCLI::Vector4^ color)
{
	Value->RenderTargetClearAll(MathConverter::ToUnmanaged(color));
}

void ACRenderDeviceW::RenderTargetActivate(UINT id)
{
	Value->RenderTargetActivate(id);
}

ACTexture* ACRenderDeviceW::RenderTargetGetTexture(UINT id)
{
	return Value->RenderTargetGetTexture(id);
}

void ACRenderDeviceW::ResizeRenderTarget(UINT id, UINT width, UINT height)
{
	Value->ResizeRenderTarget(id, width, height);
}

//tools
void ACRenderDeviceW::SaveScreenShot(const std::string& path)
{
	Value->SaveScreenShot(path);
}

//render
void ACRenderDeviceW::BeginRendering()
{
	Value->BeginRendering();
}

//renderiza as linhas usando o vbdinamico
void ACRenderDeviceW::RenderLines(UINT numVertices, 
								  UINT numIndices, 
								  ACVertexPositionColored* vertices, 
								  UINT* indices,
								  BOOL strip)
{
	Value->RenderLines(numVertices, 
					   numIndices, 
					   vertices,
					   indices,
					   strip);
}

//renderiza pontos usando o vb dinamico
void ACRenderDeviceW::RenderPoints(UINT numVertices, ACVertexPositionColored* vertices)
{
	Value->RenderPoints(numVertices, vertices);
}

//renderiza pontos usando o vb dinamico, pe protegido pq o usuario usa o spritebacth pra renderizar
void ACRenderDeviceW::RenderSprite(UINT numVertices, 
								   ACVertexSprite* vertices,
								   ACSkin* skin)
{
	Value->RenderSprite(numVertices, 
						vertices,
						skin);
}

//render dinamico de triangulos
void ACRenderDeviceW::Render(VertexFormat vertexFormat, 
							 UINT numVertices, 
							 UINT numIndices, 
							 void* vertices, 
							 UINT* indices,
							 ACSkin* skin)
{
	Value->Render(vertexFormat,
				  numVertices, 
				  numIndices, 
				  vertices, 
				  indices,
				  skin);
}

//renderiza oq tem no vb estatico
void ACRenderDeviceW::Render(ACVertexBuffer* vertexBuffer)
{
	Value->Render(vertexBuffer);
}

void ACRenderDeviceW::EndRendering()
{
	Value->EndRendering();
}

//buffers
void ACRenderDeviceW::CreateStaticBuffer(VertexFormat vertexFormat, 
									UINT numVertices, 
									UINT numIndices, 
									const void* pVertices, 
									const UINT* pIndices, 
									ACSkin* pSkin,
									ACVertexBuffer** ppOutVertexBufferPtr)
{
	Value->CreateStaticBuffer(vertexFormat, 
							  numVertices, 
							  numIndices, 
							  pVertices, 
							  pIndices, 
							  pSkin,
							  ppOutVertexBufferPtr);
}

void ACRenderDeviceW::ReleaseBuffer(ACVertexBuffer* vertexBuffer)
{
	Value->ReleaseBuffer(vertexBuffer);
}

//textures
void ACRenderDeviceW::LoadTexture(std::string path, ACTexture** ppOutTexturePtr)
{
	Value->LoadTexture(path, ppOutTexturePtr);
}

void ACRenderDeviceW::ReleaseTexture(ACTexture* pTexture)
{
	Value->ReleaseTexture(pTexture);
}

void ACRenderDeviceW::SetTexture(ACTexture* pTexture, UINT slot)
{
	Value->SetTexture(pTexture, slot);
}

//shaders
void ACRenderDeviceW::CompileVS(std::string path, VertexFormat vertexFormat, ACVertexShader** ppOutVertexShaderPtr)
{
	Value->CompileVS(path, vertexFormat, ppOutVertexShaderPtr);
}

void ACRenderDeviceW::ReleaseVS(ACVertexShader* ReleaseVS)
{
	Value->ReleaseVS(ReleaseVS);
}

void ACRenderDeviceW::ActiveVS(ACVertexShader* vertexShader)
{
	Value->ActiveVS(vertexShader);
}

void ACRenderDeviceW::CompilePS(std::string path, ACPixelShader** ppOutPixelShaderPtr)
{
	Value->CompilePS(path, ppOutPixelShaderPtr);
}

void ACRenderDeviceW::ReleasePS(ACPixelShader* pPixelShader)
{
	Value->ReleasePS(pPixelShader);
}

void ACRenderDeviceW::ActivePS(ACPixelShader* pixelShader)
{
	Value->ActivePS(pixelShader);
}

void ACRenderDeviceW::CompileGS(std::string path, ACGeometryShader** ppOutGeometryShaderPtr)
{
	Value->CompileGS(path, ppOutGeometryShaderPtr);
}

void ACRenderDeviceW::ReleaseGS(ACGeometryShader* pGeometryShader)
{
	Value->ReleaseGS(pGeometryShader);
}

void ACRenderDeviceW::ActiveGS(ACGeometryShader* geometryShader)
{
	Value->ActiveGS(geometryShader);
}

//shaders constants
void ACRenderDeviceW::SetWorldMatrix(EngineMathCLI::Matrix^ value)
{
	Value->SetWorldMatrix(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetSkinMatrizes(UINT numOfMatrizes, array<EngineMathCLI::Matrix>^ value)
{
	Value->SetSkinMatrizes(numOfMatrizes, MathConverter::ToUnmanaged(numOfMatrizes, value));
}

void ACRenderDeviceW::SetViewProjectionMatrix(EngineMathCLI::Matrix^ value)
{
	Value->SetViewProjectionMatrix(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetCameraPosition(EngineMathCLI::Vector3^ value)
{
	Value->SetCameraPosition(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetTime(float time)
{
	Value->SetTime(time);
}
	
//light
void ACRenderDeviceW::SetLightPosition(EngineMathCLI::Vector3^ value)
{
	Value->SetLightPosition(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetLightDirection(EngineMathCLI::Vector3^ value)
{
	Value->SetLightDirection(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetLightColor(EngineMathCLI::Vector3^ value)
{
	Value->SetLightColor(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetLightRange(float value)
{
	Value->SetLightRange(value);
}

void ACRenderDeviceW::SetLightFalloff(float value)
{
	Value->SetLightFalloff(value);
}

//material
void ACRenderDeviceW::SetMaterialAmbient(EngineMathCLI::Vector3^ value)
{
	Value->SetMaterialAmbient(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetMaterialEmissive(EngineMathCLI::Vector3^ value)
{
	Value->SetMaterialEmissive(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetMaterialSpecular(EngineMathCLI::Vector3^ value)
{
	Value->SetMaterialSpecular(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::SetMaterialSpecularPower(float value)
{
	Value->SetMaterialSpecularPower(value);
}

void ACRenderDeviceW::SetMaterialDiffuse(EngineMathCLI::Vector4^ value)
{
	Value->SetMaterialDiffuse(MathConverter::ToUnmanaged(value));
}

void ACRenderDeviceW::ApplyConstants()
{
	Value->ApplyConstants();
}
