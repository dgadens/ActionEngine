#include "ACMouse.h"

void ACMouse::SetButtonDown(ACMOUSEBUTTON button)
{
	mButtons[button] = true;
};

void ACMouse::SetButtonUp(ACMOUSEBUTTON button)
{
	mButtons[button] = false;
};

void ACMouse::GetState(ACMouseState* mouseState)
{
	memcpy(mouseState->Buttons, mButtons, sizeof (BOOL) * 3);
	mouseState->PosX = mPosX;
	mouseState->PosY = mPosY;
};