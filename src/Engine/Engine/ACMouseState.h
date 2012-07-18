//********************
// classe controladora do mouse
// Creator: Daniel Gadens
// Date: 25/07/2011
//**********************

#ifndef __ACMOUSESTATE_H
#define __ACMOUSESTATE_H

#include "EngineCommon.h"
#include "ACInputEnums.h"

class ACMouseState
{
public:
	bool Buttons[3];
	int  PosX;
	int  PosY;

	bool IsPressed(ACMOUSEBUTTON key);
	bool IsReleased(ACMOUSEBUTTON key);

	void Copy(ACMouseState* dest);

};

#endif