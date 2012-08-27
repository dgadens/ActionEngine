#include "ACKeyboardState.h"

BOOL ACKeyboardState::IsPressed(ACKEYS key)
{
	return Keys[key];
};

BOOL ACKeyboardState::IsReleased(ACKEYS key)
{
	return !Keys[key];
};

void ACKeyboardState::Copy(ACKeyboardState* dest)
{
	memcpy(dest->Keys, this->Keys, sizeof (BOOL) * 256);
};