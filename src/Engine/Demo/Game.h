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
#include "Globals.h"

#include "GPowerCube.h"
#include "DynamicTest.h"
#include "SpriteTest.h"
#include "LinePointTest.h"
#include "ScriptMachine.h"
#include "Floor.h"
#include "GCamera.h"
#include "GGameInput.h"

class Game
{
	//atributos obrigatorios
	ACRenderDevice*		mpGDevice;
	ACContentManager*	mpCManager;
	GCamera*			mpCamera;
	ACFont*				mpFont;
	ACSpriteBatch*		mpSpriteBatch;
	ScriptMachine*		mpScriptMachine;

	//Floor
	Floor*				mpFloor;

	//cubo
	GPowerCube*			mpPowerCube;
	//teste dinamico
	DynamicTest*        mpDynamicTest;

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