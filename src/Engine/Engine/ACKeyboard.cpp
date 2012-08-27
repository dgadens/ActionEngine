#include "ACKeyboard.h"

void ACKeyboard::SetKeyDown(ACKEYS key)
{
	mKeys[key] = true;
};

void ACKeyboard::SetKeyUp(ACKEYS key)
{
	mKeys[key] = false;
};

void ACKeyboard::GetState(ACKeyboardState* keysState)
{
	memcpy(keysState->Keys, mKeys, sizeof (BOOL) * 256);
};
