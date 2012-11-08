//************************
// classe agrupadadora de sprites 
// creator: Daniel Gadens
// date: 07/07/2011
//**********************

#ifndef __ACSPRITEBATCH_H
#define __ACSPRITEBATCH_H

#include "EngineMath.h"
#include "ACShaders.h"
#include "ACContentManager.h"
#include "ACFont.h"

class ACRenderDevice;

class ACSpriteBatch
{
private:
	ACRenderDevice*		mpGDevice;
	ACContentManager*	mpCManager;

	ACPixelShader*		mpPS;
	ACGeometryShader*	mpGS;
	ACVertexShader*		mpVS;

	//lista de skins 1 por textura diferente, organizado por ID globals da textura
	std::map<UINT, ACSkin*> mpSkins;
	ACVertexSprite      mVertex;

	//usado para restaurar no enddraw
	ACSHADEMODE			mCurrentShadeMode;

	float				mCurrentAspect;
	ACSkin* GetSkin(ACTexture* texture);
	void ToHomogenousClipSpace(const ACTexture* texture, const Vector2& position, ACVertexSprite& outVertex);
	void ToHomogenousClipSpace(const ACTexture* texture, const Vector2& position, float scale, ACVertexSprite& outVertex);
	void ToHomogenousClipSpace(const ACTexture* texture, const Vector2& position, Vector2& scale, ACVertexSprite& outVertex);

public:

	ACSpriteBatch(ACRenderDevice* gDevice, ACContentManager* cManager);
	~ACSpriteBatch();

	void BeginRender(ACPixelShader* customPixelShader = nullptr);
	void BeginRender(ACBLENDSTATE blendState, ACPixelShader* customPixelShader = nullptr);

	void Render(ACTexture* texture, Vector2& position, const Vector4& color);
	void Render(ACTexture* texture, Vector2& position, const Vector4& color, float rotation);
	void Render(ACTexture* texture, Vector2& position, const Vector4& color, float rotation, float scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const Vector4& color, float rotation, Vector2& scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const Vector4& color, const Vector2& origin, float rotation, float scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const Vector4& color, const Vector2& origin, float rotation, Vector2& scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color);
	void Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, float rotation);
	void Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, float rotation, float scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, float rotation, Vector2& scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, const Vector2& origin, float rotation, float scale, UINT flip);
	void Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, const Vector2& origin, float rotation, Vector2& scale, UINT flip);

	void RenderString(ACFont* font, const std::string& text, Vector2& position, const Vector4& color);

	void EndRender();

	void Release();
};

#endif