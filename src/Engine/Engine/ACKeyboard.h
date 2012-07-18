//********************
// classe controladora do teclado
// Creator: Daniel Gadens
// Date: 25/07/2011
//**********************

#ifndef __ACKEYBOARD_H
#define __ACKEYBOARD_H

#include "EngineCommon.h"
#include "ACKeyboardState.h"
#include "ACInputEnums.h"

class ACKeyboard
{
private:
	bool mKeys[256];

public:
	ACKeyboard() 
	{
		//seta tudo pra falser
		memset(mKeys, 0, sizeof(bool) * 256);
	};
	~ACKeyboard() {};

	void SetKeyDown(ACKEYS key);
	void SetKeyUp(ACKEYS key);

	void GetState(ACKeyboardState* keysState);

};

#endif