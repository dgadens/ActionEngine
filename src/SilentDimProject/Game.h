//********************************
// Classe base para gerenciamento do game
// Creator: Daniel Gadens
// Date: 16/06/2012
//*******************************

#ifndef __GAME_H
#define __GAME_H

#include "EngineMain.h"
#include "EngineCommon.h"
#include "EngineMath.h"

#include "SD_Globals.h"

class Game
{
	//atributos obrigatorios
	ACRenderDevice*		mpGDevice;
	ACContentManager*	mpCManager;
	ACCamera*			mpCamera;
	ACFont*				mpFont;
	ACSpriteBatch*		mpSpriteBatch;

	//RenderTarget
	UINT				mRenderTargetID;

	void DrawTexts();
	void SetPerframeData();

public:
	Game(ACRenderDevice* gDevice, ACContentManager* cManager);
	~Game();

	void Active(BOOL value);
	void ReziseWindow(INT width, INT height);
	void Initialize();
	void LoadContents();
	void Update();
	void Draw();
};

#endif