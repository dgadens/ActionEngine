#include "GameInput.h"

ACKeyboard* GameInput::mpKeyboard;
ACMouse*    GameInput::mpMouse;

ACKeyboardState GameInput::mCurrentKbState;
ACMouseState    GameInput::mCurrentMouseState;

ACKeyboardState GameInput::mPreviousKbState;
ACMouseState    GameInput::mPreviousMouseState;


void GameInput::Init(ACKeyboard* keyboard, ACMouse* mouse)
{
	mpKeyboard = keyboard;
    mpMouse = mouse;
};

void GameInput::Process(GPowerCube* cube)
{
	mpKeyboard->GetState(&mCurrentKbState);
	mpMouse->GetState(&mCurrentMouseState);

	if (mCurrentKbState.IsPressed(ACKEYS::ACKEY_A))
	{
		//cube->AddAcceleration(-1, 0);
	}

	if (mCurrentKbState.IsPressed(ACKEYS::ACKEY_D))
	{
		//cube->AddAcceleration(1, 0);
	}

	if (mCurrentKbState.IsPressed(ACKEYS::ACKEY_W))
	{
		//cube->AddAcceleration(0, 1);
	}

	if (mCurrentKbState.IsPressed(ACKEYS::ACKEY_S))
	{
		//cube->AddAcceleration(0, -1);
	}
	

#pragma region Piking
	//verifica selecao
	Ray ray;
	Ray::GetPickRay(&Vector2(mCurrentMouseState.PosX, mCurrentMouseState.PosY), &Globals::pCamera->GetProjection(), &Globals::pCamera->GetView(), Globals::MainWndWidth, Globals::MainWndHeight,  Globals::pCamera->GetNear(), Globals::pCamera->GetFar(), &ray);
	VerifySelection::Process(cube->GetModel(), &ray); 
#pragma endregion

	//Distância em Z da camera até o plano da nave
	float distance = ray.Position.Z - 0;
	//Posiçao do clique no plano da nave
	Vector3 position = Vector3(ray.Position.X + ray.Direction.X * distance, 
								ray.Position.Y + ray.Direction.Y * distance,
								ray.Position.Z + ray.Direction.Z * distance);

	if(mCurrentMouseState.IsReleased(ACMOUSE_LEFT) && mPreviousMouseState.IsPressed(ACMOUSE_LEFT))
		cube->Explode();
	

	//seta os valores anteriores
	mCurrentKbState.Copy(&mPreviousKbState);
	mCurrentMouseState.Copy(&mPreviousMouseState);
};