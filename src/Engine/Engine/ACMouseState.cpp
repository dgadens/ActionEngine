#include "ACMouseState.h"

bool ACMouseState::IsPressed(ACMOUSEBUTTON key)
{
	return Buttons[key];
};

bool ACMouseState::IsReleased(ACMOUSEBUTTON key)
{
	return !Buttons[key];
};

void ACMouseState::Copy(ACMouseState* dest)
{
	memcpy(dest->Buttons, this->Buttons, sizeof (bool) * 3);
	dest->PosX = this->PosX;
	dest->PosY = this->PosY;
};