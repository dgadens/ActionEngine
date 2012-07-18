
#ifndef __BACKGROUND_H
#define __BACKGROUND_H

#include "ACSpriteBatch.h"
#include "ACTimeControl.h"
#include "EngineMath.h"
#include "ACContentManager.h"

class Background
{
private:
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	//ACSpriteBatch* mSpriteBatch;
	////ACSprite* mSprite1;
	//Vector2 mSprite1Position;
	////ACSprite* mSprite2;
	//Vector2 mSprite2Position;

public:
	Background(ACRenderDevice* gDevice, ACContentManager* cManager);
	~Background();
	void SetTexture(std::string textureName);
	void Update(float elapsedTime);
	void Render();

};
#endif