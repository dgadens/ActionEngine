//*****************************
// controla os inputs
// creator: Daniel Gadens
// date: 26/08/2012
//******************************

#ifndef __SD_INPUT_H
#define __SD_INPUT_H

#include "SD_Globals.h"
#include "ACCamera.h"
#include "EngineMath.h"
#include "ACKeyboard.h"
#include "ACKeyboardState.h"
#include "ACMouse.h"
#include "ACMouseState.h"
#include "ACTimeControl.h"

class SD_GameInput
{
private:
	static ACKeyboard* mpKeyboard;
	static ACMouse* mpMouse;

	static ACKeyboardState mCurrentKbState;
	static ACMouseState mCurrentMouseState;

	static ACKeyboardState mPreviousKbState;
	static ACMouseState mPreviousMouseState;

public:
	static void Init(ACKeyboard* keyboard, ACMouse* mouse);
	static void Process();
};

#endif