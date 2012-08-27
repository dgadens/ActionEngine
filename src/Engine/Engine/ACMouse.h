//********************
// classe controladora do mouse
// Creator: Daniel Gadens
// Date: 25/07/2011
//**********************

#ifndef __ACMOUSE_H
#define __ACMOUSE_H

#include "EngineCommon.h"
#include "ACInputEnums.h"
#include "ACMouseState.h"

class ACMouse
{
private:
	BOOL mButtons[3];
	int  mPosX;
	int  mPosY;

public:
	ACMouse() 
	{
		memset(mButtons, 0, sizeof ( BOOL ) * 3);
	};

	~ACMouse() {};

	void SetButtonDown(ACMOUSEBUTTON button);
	void SetButtonUp(ACMOUSEBUTTON button);

	void SetPosition(int x, int y)
	{
		mPosX = x;
		mPosY = y;
	};

	void GetState(ACMouseState* mouseState);

};

#endif