//********************
// classe controladora do teclado
// Creator: Daniel Gadens
// Date: 25/07/2011
//**********************

#ifndef __ACKEYBOARDSTATE_H
#define __ACKEYBOARDSTATE_H

#include "EngineCommon.h"
#include "ACInputEnums.h"

class ACKeyboardState
{
public:
	BOOL Keys[256];

	BOOL IsPressed(ACKEYS key);
	BOOL IsReleased(ACKEYS key);

	void Copy(ACKeyboardState* dest);

};

#endif