#include "ACKeyboardState.h"

bool ACKeyboardState::IsPressed(ACKEYS key)
{
	return Keys[key];
};

bool ACKeyboardState::IsReleased(ACKEYS key)
{
	return !Keys[key];
};

void ACKeyboardState::Copy(ACKeyboardState* dest)
{
	memcpy(dest->Keys, this->Keys, sizeof (bool) * 256);
};