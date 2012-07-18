#include "ACSpriteBatch.h"
#include "ACFont.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"

class SpriteTest
{
private:
	ACSpriteBatch* mpSpriteBatch;

	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	ACTexture* mpTexture;
	ACTexture* mpTexture2;
	ACTexture* mpTexture3;

	Vector2    mPosition;
	Vector4    mColor;
	ACRectangle mRectangle;
	float	   mRotation;

public:
	SpriteTest(ACRenderDevice* gDevice, ACContentManager* cManager);
	~SpriteTest();

	void Update(float elapsedTime);
	void Draw();
};