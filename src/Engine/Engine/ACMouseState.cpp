#include "ACMouseState.h"

BOOL ACMouseState::IsPressed(ACMOUSEBUTTON key)
{
	return Buttons[key];
};

BOOL ACMouseState::IsReleased(ACMOUSEBUTTON key)
{
	return !Buttons[key];
};

void ACMouseState::Copy(ACMouseState* dest)
{
	memcpy(dest->Buttons, this->Buttons, sizeof (BOOL) * 3);
	dest->PosX = this->PosX;
	dest->PosY = this->PosY;
};