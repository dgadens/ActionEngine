#include "SD_GameInput.h"

ACKeyboard* SD_GameInput::mpKeyboard;
ACMouse*    SD_GameInput::mpMouse;

ACKeyboardState SD_GameInput::mCurrentKbState;
ACMouseState    SD_GameInput::mCurrentMouseState;

ACKeyboardState SD_GameInput::mPreviousKbState;
ACMouseState    SD_GameInput::mPreviousMouseState;


void SD_GameInput::Init(ACKeyboard* keyboard, ACMouse* mouse)
{
	mpKeyboard = keyboard;
    mpMouse = mouse;
};

void SD_GameInput::Process()
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
	//Ray ray;
	//Ray::GetPickRay(&Vector2(mCurrentMouseState.PosX, mCurrentMouseState.PosY), &Globals::pCamera->Projection, &Globals::pCamera->View, Globals::MainWndWidth, Globals::MainWndHeight,  Globals::pCamera->NearPlane, Globals::pCamera->FarPlane, &ray);
	//VerifySelection::Process(cube->GetModel(), &ray); 
#pragma endregion

	//Distância em Z da camera até o plano da nave
	//float distance = ray.Position.Z - 0;
	//Posiçao do clique no plano da nave
	//Vector3 position = Vector3(ray.Position.X + ray.Direction.X * distance, 
	//							ray.Position.Y + ray.Direction.Y * distance,
	//							ray.Position.Z + ray.Direction.Z * distance);

	//if(mCurrentMouseState.IsReleased(ACMOUSE_LEFT) && mPreviousMouseState.IsPressed(ACMOUSE_LEFT))
	//	cube->Explode();
	

	//seta os valores anteriores
	mCurrentKbState.Copy(&mPreviousKbState);
	mCurrentMouseState.Copy(&mPreviousMouseState);
};