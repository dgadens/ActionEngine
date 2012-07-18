//*****************************
// controla os inputs
// creator: Daniel Gadens
// date: 26/07/2011
//******************************

#ifndef __INPUT_H
#define __INPUT_H

#include "Globals.h"
#include "VerifySelection.h"
#include "ACCamera.h"
#include "EngineMath.h"
#include "ACKeyboard.h"
#include "ACKeyboardState.h"
#include "ACMouse.h"
#include "ACMouseState.h"
#include "ACTimeControl.h"
#include "GPowerCube.h"

class GameInput
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
	static void Process(GPowerCube* cube);
};

#endif