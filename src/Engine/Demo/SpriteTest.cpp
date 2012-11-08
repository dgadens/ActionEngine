#include "SpriteTest.h"

SpriteTest::SpriteTest(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	mpSpriteBatch = new ACSpriteBatch(mpGDevice, cManager);
	mpTexture = mpCManager->LoadTexture("TesteSprite.png");
	mpTexture2 = mpCManager->LoadTexture("BoxTNTDiffuse.png");
	mpTexture3 = mpCManager->LoadTexture("TexturaTeste.png");

	mPosition = Vector2(0);
	mColor = Vector4(1);
	mRotation = 0;
	ZeroMemory(&mRectangle, sizeof ( ACRectangle ));
};

SpriteTest::~SpriteTest()
{
	Release();
};

void SpriteTest::Release()
{
	if (mpTexture != nullptr)
		mpCManager->RemoveTexture(mpTexture);
	if (mpTexture2 != nullptr)
		mpCManager->RemoveTexture(mpTexture2);
	if (mpTexture3 != nullptr)
		mpCManager->RemoveTexture(mpTexture3);

	SAFE_RELEASE(mpSpriteBatch);
};

void SpriteTest::Update(float elapsedTime)
{
	mRotation += elapsedTime;
};

void SpriteTest::Draw()
{
	mpSpriteBatch->BeginRender(ACBLENDSTATE::ACBS_NonPremultiplied);

	mPosition.X = 0;
	mPosition.Y = 0;

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		mRectangle.X = 0;
		mRectangle.Y = 0;
		mRectangle.Width = 256;
		mRectangle.Height = 256;
		mPosition.X = i*2;
		mPosition.Y = 300;
		mpSpriteBatch->Render(mpTexture2, mPosition, mRectangle, mColor, Vector2(128,128), 0, 1.0f, FlipType::FT_None);
	}

	for (i = 0; i < 2; i++)
	{
		mPosition.X = i * 300;
		mPosition.Y = 512;
		mRectangle.X = 0;
		mRectangle.Y = 0;
		mRectangle.Width = 256;
		mRectangle.Height = 256;

		mpSpriteBatch->Render(mpTexture3, mPosition, mRectangle, mColor, 0, 1.0f, FlipType::FT_None);
	}

	mpSpriteBatch->EndRender();
};