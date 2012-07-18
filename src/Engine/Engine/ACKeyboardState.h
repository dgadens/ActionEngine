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
	bool Keys[256];

	bool IsPressed(ACKEYS key);
	bool IsReleased(ACKEYS key);

	void Copy(ACKeyboardState* dest);

};

#endif