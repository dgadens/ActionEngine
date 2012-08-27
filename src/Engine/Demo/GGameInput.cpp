#include "GGameInput.h"

ACKeyboard* GGameInput::mpKeyboard;
ACMouse*    GGameInput::mpMouse;

ACKeyboardState GGameInput::mCurrentKbState;
ACMouseState    GGameInput::mCurrentMouseState;

ACKeyboardState GGameInput::mPreviousKbState;
ACMouseState    GGameInput::mPreviousMouseState;


void GGameInput::Init(ACKeyboard* keyboard, ACMouse* mouse)
{
	mpKeyboard = keyboard;
    mpMouse = mouse;
};

void GGameInput::Update()
{
	//seta os valores anteriores
	mCurrentKbState.Copy(&mPreviousKbState);
	mCurrentMouseState.Copy(&mPreviousMouseState);

	//resgata os valores atuais
	mpKeyboard->GetState(&mCurrentKbState);
	mpMouse->GetState(&mCurrentMouseState);
};

ACKeyboardState GGameInput::GetKBState()
{
	return mCurrentKbState;
};

ACKeyboardState GGameInput::GetPreviousKBState()
{
	return mPreviousKbState;
};

ACMouseState GGameInput::GetMouseState()
{
	return mCurrentMouseState;
};

ACMouseState GGameInput::GetPreviousMouseState()
{
	return mPreviousMouseState;
};

void GGameInput::Picking()
{
	Ray ray;
	Ray::GetPickRay(&Vector2(mCurrentMouseState.PosX, mCurrentMouseState.PosY), &Globals::pCamera->GetProjection(), &Globals::pCamera->GetView(), Globals::MainWndWidth, Globals::MainWndHeight,  Globals::pCamera->GetNear(), Globals::pCamera->GetFar(), &ray);
	//VerifySelection::Process(cube->GetModel(), &ray); 
};
