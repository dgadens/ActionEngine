#include "Background.h"

Background::Background(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	//mSprite1=new ACSprite(mpGDevice, mpCManager);
	//mSprite2=new ACSprite(mpGDevice, mpCManager);
	//mSpriteBatch=new ACSpriteBatch(mpGDevice);

};

Background::~Background()
{
	//SAFE_DELETE(mSprite1);
	//SAFE_DELETE(mSprite2);
	//SAFE_DELETE(mSpriteBatch);
};

void Background::SetTexture(std::string textureName)
{
	//mSprite1->SetTexture(textureName);
	//mSprite2->SetTexture(textureName);

	//mSprite1Position.Y = 0;
	//mSprite2Position.Y = mSprite1->pTexture->Height;
};

void Background::Update(float elapsedTime)
{
	//float mov = elapsedTime * 100;
	//mSprite1Position.Y -= mov;
	//mSprite2Position.Y -= mov;

	//if (mSprite1Position.Y < -mSprite1->pTexture->Height)
	//	mSprite1Position.Y = mSprite2Position.Y + mSprite1->pTexture->Height;

	//if (mSprite2Position.Y < -mSprite2->pTexture->Height)
	//	mSprite2Position.Y = mSprite1Position.Y + mSprite2->pTexture->Height;
};

void Background::Render()
{
	//mSpriteBatch->BeginRender();
	//mSpriteBatch->Render(mSprite1, mSprite1Position, Vector2(1,1), 0);
	//mSpriteBatch->Render(mSprite2, mSprite2Position, Vector2(1,1), 0);
	//mSpriteBatch->EndRender();
};