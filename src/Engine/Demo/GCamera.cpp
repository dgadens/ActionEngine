#include "GCamera.h"

GCamera::GCamera()
{
	GCamera(Free);
};

GCamera::GCamera(GCameraType type) : ACCamera()
{
	mCameraType = type;
};

GCamera::~GCamera()
{
};

void GCamera::Update(FLOAT elapsedTime)
{
	mElapsedTime = elapsedTime;
	ProcessInput();

	switch (mCameraType)
	{
		case Free:
		{
			Vector3 result;

			//rotacao em x
            Matrix rotation; 
			Matrix::CreateRotationX(mAngle.X, &rotation);

			//rotacao em y
            Matrix rotation2; 
			Matrix::CreateRotationY(mAngle.Y, &rotation2);

			//multiplica
			Matrix::Multiply(&rotation, &rotation2, &rotation);

			//transforma a posicao
			Vector3::Transform(&mTranslation, &rotation, &mTranslation);
			mPosition = mPosition + mTranslation;

			//transforma o target
            Vector3::Transform(&Vector3(0, 0, 1), &rotation, &result);
			Vector3 target = mPosition - result;
			SetTarget(target);

			//transforma o up
            Vector3::Transform(&Vector3(0, 1, 0), &rotation, &mUp);

		}
		break;
		case SideScroller:

		break;
	};

	ACCamera::Update();
};

void GCamera::ProcessInput()
{
	ACKeyboardState kbS = GGameInput::GetKBState();
	ACMouseState mS = GGameInput::GetMouseState();
	ACMouseState pmS = GGameInput::GetPreviousMouseState();

	memset( &mTranslation, 0, sizeof ( Vector3 ));

	if (kbS.IsPressed(ACKEYS::ACKEY_A))
		mTranslation.X -= 100 * mElapsedTime;

	if (kbS.IsPressed(ACKEYS::ACKEY_D))
		mTranslation.X += 100 * mElapsedTime;

	if (kbS.IsPressed(ACKEYS::ACKEY_S))
		mTranslation.Z += 100 * mElapsedTime;

	if (kbS.IsPressed(ACKEYS::ACKEY_W))
		mTranslation.Z -= 100 * mElapsedTime;

	if (kbS.IsPressed(ACKEYS::ACKEY_E))
		mTranslation.Y += 100 * mElapsedTime;

	if (kbS.IsPressed(ACKEYS::ACKEY_Q))
		mTranslation.Y -= 100 * mElapsedTime;

	if (mS.IsPressed(ACMOUSEBUTTON::ACMOUSE_RIGHT) &&
		pmS.IsPressed(ACMOUSEBUTTON::ACMOUSE_RIGHT))
	{
		mAngle.X -= (mS.PosY - pmS.PosY) * mElapsedTime;
		mAngle.Y -= (mS.PosX - pmS.PosX) * mElapsedTime;
	}
};