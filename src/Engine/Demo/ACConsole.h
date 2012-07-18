
#ifndef __ACCONSOLE_H
#define __ACCONSOLE_H

#include "ACSpriteBatch.h"
#include "ACSpriteFont.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"
#include "EngineCommon.h"
#include "Globals.h"
#include <string>

class ACConsole
{
private:
	ACRenderDevice* mpGDevice;
	ACContentManager* mpCManager;

	int mIndex;
	ACSpriteBatch*  mpSpriteBatch;
	ACSpriteFont*   mpSpriteFont;

	static std::string COMMAND0;
	static std::string COMMAND1;
	static std::string COMMAND2;
	static std::string COMMAND3;
	static std::string SYMBOLS;

public:

	ACConsole(ACRenderDevice* gDevice, ACContentManager* cManager, std::string fontName)
	{ 
		mpGDevice = gDevice;
		mpCManager = cManager;

		mpSpriteBatch = new ACSpriteBatch(gDevice);
		mpSpriteFont = new ACSpriteFont(gDevice, cManager, fontName);

		Clear();
	};

	~ACConsole()
	{
		SAFE_DELETE(mpSpriteBatch);
		SAFE_DELETE(mpSpriteFont);
	};

	char Text[256];
	void Clear();

	void ProcessCommand();
	void SetCharacter(char c);
	void Render();

};

#endif