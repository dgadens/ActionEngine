#include "ACSpriteBatch.h"
#include "ACRenderDevice.h"

ACSpriteBatch::ACSpriteBatch(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	mpPS = nullptr;
	mpGS = nullptr;
	mpVS = nullptr;

	mpVS = mpCManager->LoadVertexShader("Sprite.VShlsl4", VertexFormat::VF_VertexSprite);
	mpPS = mpCManager->LoadPixelShader("Sprite.PShlsl4");

	//carrega os shaders dos sprites se tiver suporte a GS entao ele usa
	if (mpGDevice->GeometryShaderSupport)
		mpGS = mpCManager->LoadGeometryShader("Sprite.GShlsl4");

	ZeroMemory(&mVertex, sizeof ( ACVertexSprite ));
};

ACSpriteBatch::~ACSpriteBatch()
{
	if (mpPS != nullptr)
		mpCManager->RemovePixelShader(mpPS);

	if (mpGS != nullptr)
		mpCManager->RemoveGeometryShader(mpGS);

	if (mpVS != nullptr)
		mpCManager->RemoveVertexShader(mpVS);
};

ACSkin* ACSpriteBatch::GetSkin(ACTexture* texture)
{
	ACSkin* skin = nullptr;
	auto reg = mpSkins.find(texture->ID);

	//se ja existe no map entao ele resgata o skin, senao ele cria um novo, seta a textura e some no instance da textura
	if (reg == mpSkins.end())
	{
		skin = mpCManager->CreateSkin(false);
		skin->Textures[0] = texture;

		//se nao for rendertarget entao ele adiciona 1 na instancia pq no endrendering ele vai diminuir
		if (!texture->IsRenderTarget)
		{
			texture->Instance++;
			mpSkins.insert(std::pair<UINT, ACSkin*>(texture->ID, skin));
		}
	}
	else
		skin = reg->second;

	return skin;
};

void ACSpriteBatch::ToHomogenousClipSpace(const ACTexture* texture, const Vector2& position, ACVertexSprite& outVertex)
{
	outVertex.position.X = GenericTools::ConvertPixelsToClipSpace(mpGDevice->GetVPWidth(), position.X);
	outVertex.position.Y = -GenericTools::ConvertPixelsToClipSpace(mpGDevice->GetVPHeight(), position.Y);
	outVertex.dimension.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), texture->Width);
	outVertex.dimension.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), texture->Height);
};

void ACSpriteBatch::ToHomogenousClipSpace(const ACTexture* texture, const Vector2& position, float scale, ACVertexSprite& outVertex)
{
	outVertex.position.X = GenericTools::ConvertPixelsToClipSpace(mpGDevice->GetVPWidth(), position.X);
	outVertex.position.Y = -GenericTools::ConvertPixelsToClipSpace(mpGDevice->GetVPHeight(), position.Y);
	outVertex.dimension.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), (float)texture->Width * scale);
	outVertex.dimension.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), (float)texture->Height * scale);
};

void ACSpriteBatch::ToHomogenousClipSpace(const ACTexture* texture, const Vector2& position, Vector2& scale, ACVertexSprite& outVertex)
{
	outVertex.position.X = GenericTools::ConvertPixelsToClipSpace(mpGDevice->GetVPWidth(), position.X);
	outVertex.position.Y = -GenericTools::ConvertPixelsToClipSpace(mpGDevice->GetVPHeight(), position.Y);
	outVertex.dimension.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), (float)texture->Width * scale.X);
	outVertex.dimension.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), (float)texture->Height * scale.Y);
};

void ACSpriteBatch::BeginRender(ACBLENDSTATE blendState, ACPixelShader* customPixelShader)
{
	//desabilita a escrita do depthbuffer
	mpGDevice->SetBlendState(blendState);;
	BeginRender(customPixelShader);	
};

void ACSpriteBatch::BeginRender(ACPixelShader* customPixelShader)
{
	//pega o shademode atual depois restaura
	mCurrentShadeMode = mpGDevice->GetShadeMode();

	//desabilita a escrita do depthbuffer
	mpGDevice->SetDepthBufferState(ACDEPTHBUFFERSTATE::ACDBS_WriteDisable);

	//ativa os shaders necessarios
	mpGDevice->ActiveVS(mpVS);

	if (customPixelShader == nullptr)
		mpGDevice->ActivePS(mpPS);
	else
		mpGDevice->ActivePS(customPixelShader);

	if (mpGDevice->GeometryShaderSupport)
		mpGDevice->ActiveGS(mpGS);

	mCurrentAspect = mpGDevice->GetVPWidth() / (float)mpGDevice->GetVPHeight();
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const Vector4& color)
{
	//preciso criar um skin pq o gerenciador de vertices usa ele pra renderizar
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, mVertex);
	mVertex.rectangle.X = 0;
	mVertex.rectangle.Y = 0;
	mVertex.rectangle.Z = 1;
	mVertex.rectangle.W = 1;
	mVertex.rotation = 0;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const Vector4& color, float rotation)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, mVertex);
	mVertex.rectangle.X = 0;
	mVertex.rectangle.Y = 0;
	mVertex.rectangle.Z = 1;
	mVertex.rectangle.W = 1;
	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const Vector4& color, float rotation, float scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = 0;
	mVertex.rectangle.Y = 0;
	mVertex.rectangle.Z = 1;
	mVertex.rectangle.W = 1;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		mVertex.rectangle.X = 1;
		mVertex.rectangle.Z = 0;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		mVertex.rectangle.Y = 1;
		mVertex.rectangle.W = 0;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const Vector4& color, float rotation, Vector2& scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = 0;
	mVertex.rectangle.Y = 0;
	mVertex.rectangle.Z = 1;
	mVertex.rectangle.W = 1;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		mVertex.rectangle.X = 1;
		mVertex.rectangle.Z = 0;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		mVertex.rectangle.Y = 1;
		mVertex.rectangle.W = 0;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const Vector4& color, const Vector2& origin, float rotation, float scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = 0;
	mVertex.rectangle.Y = 0;
	mVertex.rectangle.Z = 1;
	mVertex.rectangle.W = 1;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		mVertex.rectangle.X = 1;
		mVertex.rectangle.Z = 0;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		mVertex.rectangle.Y = 1;
		mVertex.rectangle.W = 0;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), origin.X * scale);
	mVertex.origin.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), origin.Y * scale);
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const Vector4& color, const Vector2& origin, float rotation, Vector2& scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = 0;
	mVertex.rectangle.Y = 0;
	mVertex.rectangle.Z = 1;
	mVertex.rectangle.W = 1;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		mVertex.rectangle.X = 1;
		mVertex.rectangle.Z = 0;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		mVertex.rectangle.Y = 1;
		mVertex.rectangle.W = 0;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), origin.X * scale.X);
	mVertex.origin.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), origin.Y * scale.Y);
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};


void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, mVertex);
	mVertex.rectangle.X = (float)rectangle.X / texture->Width;
	mVertex.rectangle.Y = (float)rectangle.Y / texture->Height;
	mVertex.rectangle.Z = (float)(rectangle.X + rectangle.Width) / texture->Width;
	mVertex.rectangle.W = (float)(rectangle.Y + rectangle.Height) / texture->Height;
	mVertex.rotation = 0;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, float rotation)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, mVertex);
	mVertex.rectangle.X = (float)rectangle.X / texture->Width;
	mVertex.rectangle.Y = (float)rectangle.Y / texture->Height;
	mVertex.rectangle.Z = (float)(rectangle.X + rectangle.Width) / texture->Width;
	mVertex.rectangle.W = (float)(rectangle.Y + rectangle.Height) / texture->Height;
	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, float rotation, float scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = (float)rectangle.X / texture->Width;
	mVertex.rectangle.Y = (float)rectangle.Y / texture->Height;
	mVertex.rectangle.Z = (float)(rectangle.X + rectangle.Width) / texture->Width;
	mVertex.rectangle.W = (float)(rectangle.Y + rectangle.Height) / texture->Height;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		float aux = mVertex.rectangle.X;
		mVertex.rectangle.X = mVertex.rectangle.Z;
		mVertex.rectangle.Z = aux;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		float aux = mVertex.rectangle.Y;
		mVertex.rectangle.Y = mVertex.rectangle.W;
		mVertex.rectangle.W = aux;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, float rotation, Vector2& scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = (float)rectangle.X / texture->Width;
	mVertex.rectangle.Y = (float)rectangle.Y / texture->Height;
	mVertex.rectangle.Z = (float)(rectangle.X + rectangle.Width) / texture->Width;
	mVertex.rectangle.W = (float)(rectangle.Y + rectangle.Height) / texture->Height;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		float aux = mVertex.rectangle.X;
		mVertex.rectangle.X = mVertex.rectangle.Z;
		mVertex.rectangle.Z = aux;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		float aux = mVertex.rectangle.Y;
		mVertex.rectangle.Y = mVertex.rectangle.W;
		mVertex.rectangle.W = aux;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = 0;
	mVertex.origin.Y = 0;
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, const Vector2& origin, float rotation, float scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = (float)rectangle.X / texture->Width;
	mVertex.rectangle.Y = (float)rectangle.Y / texture->Height;
	mVertex.rectangle.Z = (float)(rectangle.X + rectangle.Width) / texture->Width;
	mVertex.rectangle.W = (float)(rectangle.Y + rectangle.Height) / texture->Height;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		float aux = mVertex.rectangle.X;
		mVertex.rectangle.X = mVertex.rectangle.Z;
		mVertex.rectangle.Z = aux;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		float aux = mVertex.rectangle.Y;
		mVertex.rectangle.Y = mVertex.rectangle.W;
		mVertex.rectangle.W = aux;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), origin.X * scale);
	mVertex.origin.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), origin.Y * scale);
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::Render(ACTexture* texture, Vector2& position, const ACRectangle& rectangle, const Vector4& color, const Vector2& origin, float rotation, Vector2& scale, UINT flip)
{
	ACSkin* skin = GetSkin(texture);

	//seta os dados conforme os parametros e ja converte para o espaço homogeneo
	ToHomogenousClipSpace(texture, position, scale, mVertex);
	mVertex.rectangle.X = (float)rectangle.X / texture->Width;
	mVertex.rectangle.Y = (float)rectangle.Y / texture->Height;
	mVertex.rectangle.Z = (float)(rectangle.X + rectangle.Width) / texture->Width;
	mVertex.rectangle.W = (float)(rectangle.Y + rectangle.Height) / texture->Height;

	if (flip & FlipType::FT_HorizontalFlip)
	{
		float aux = mVertex.rectangle.X;
		mVertex.rectangle.X = mVertex.rectangle.Z;
		mVertex.rectangle.Z = aux;
	}
	
	if (flip & FlipType::FT_VerticalFlip)
	{
		float aux = mVertex.rectangle.Y;
		mVertex.rectangle.Y = mVertex.rectangle.W;
		mVertex.rectangle.W = aux;
	}

	mVertex.rotation = rotation;
	mVertex.aspect = mCurrentAspect;
	mVertex.origin.X = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPWidth(), origin.X * scale.X);
	mVertex.origin.Y = GenericTools::ConvertPixelsToClipSpaceDistance(mpGDevice->GetVPHeight(), origin.Y * scale.Y);
	mVertex.color = color;

	mpGDevice->RenderSprite(1, &mVertex, skin); 
};

void ACSpriteBatch::RenderString(ACFont* font, const std::string& text, Vector2& position, const Vector4& color)
{
	char const *c = text.c_str();
	int texSize = text.size();
	Vector2 currentPosition = position;
	ACRectangle rectangle;
	Vector2 scale;
	int currentLine = 0;
	for (int i = 0; i < texSize; i++)
	{
		ACCharSettings ch = font->Characteres[(BYTE)c[i]];

		if (c[i]=='\n')
		{
			currentPosition.X = position.X;
			currentLine++;
		}

		//cria o rectangle das coordenadas
		rectangle.X = ch.X;
		rectangle.Y = ch.Y;
		rectangle.Width = ch.Width;
		rectangle.Height = ch.Height;

		//posicionamento
		currentPosition.X += ch.XOffset;
		currentPosition.Y = position.Y + ch.YOffset + currentLine * font->Size;

		//scale
		scale.X = ch.Width / (float)font->Width;
		scale.Y = ch.Height / (float)font->Height;

		Render(font->Texture, currentPosition, rectangle, color, 0, scale, FlipType::FT_None);

		currentPosition.X += ch.XAdvance;
	}
};

void ACSpriteBatch::EndRender()
{
	//restaura as propriedades
	mpGDevice->SetShadeMode(mCurrentShadeMode);
	mpGDevice->SetDepthBufferState(ACDEPTHBUFFERSTATE::ACDBS_WriteEnable);
	mpGDevice->SetBlendState(ACBLENDSTATE::ACBS_Default);

	mpGDevice->ActiveVS(nullptr);
	mpGDevice->ActivePS(nullptr);
	
	if (mpGDevice->GeometryShaderSupport)
		mpGDevice->ActiveGS(nullptr);

	//limpa os skins deste batch, uso o safe_clear pq eu quero q delete os skins
	SAFE_MAP_DELETE_CLEAR(mpSkins);
};
