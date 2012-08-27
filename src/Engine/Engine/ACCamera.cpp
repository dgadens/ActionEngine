#include "ACCamera.h"

ACCamera::ACCamera()
{
	ResetCamera();
};

ACCamera::~ACCamera()
{
	Release();
};

void ACCamera::Release()
{
};

void ACCamera::ResetCamera()
{
    mNearPlane = 1.0f;
    mFarPlane = 1000.0f;
    mPosition.X  = 0.0f;
	mPosition.Y  = 0.0f;
	mPosition.Z  = 500.0f;

    PreviousPosition = mPosition;

    mTarget = Vector3(0, 0, -1);
    mUp = Vector3(0, 1, 0);

	Zoom = 800;

    mChangedPosition = FALSE;
};

void ACCamera::Update()
{
    mChangedZoom = FALSE;
    mChangedPosition = TRUE;

	Matrix::CreateLookAt(&mPosition, &mTarget, &mUp, &View);
	Matrix::CreatePerspectiveFieldOfView(PIOVER4, mWidth / mHeight, mNearPlane, mFarPlane, &Projection);

	Matrix::Multiply(&View, &Projection, &ViewProjection);
};

void ACCamera::SetWidth(FLOAT value)
{
	if (mWidth!=value)
	{
		mWidth = value;
		mChangedVPDimension = TRUE;
	}
};
		
void ACCamera::SetHeight(FLOAT value)
{
	if (mHeight!=value)
	{
		mHeight = value;
		mChangedVPDimension = TRUE;
	}
};

void ACCamera::SetNear(FLOAT value)
{
	if (mNearPlane != value)
	{
		mNearPlane = value;
		mChangedVPDimension = TRUE;
	}
};

void ACCamera::SetFar(FLOAT value)
{
	if (mFarPlane != value)
	{
		mFarPlane = value;
		mChangedVPDimension = TRUE;
	}
};

void ACCamera::SetPosition(const Vector3& value)
{
	if (mPosition != value)
	{
		mPosition = value;
		mChangedVPDimension = TRUE;
	}
};

void ACCamera::SetTarget(const Vector3& value)
{
	if (mTarget != value)
	{
		mTarget = value;
		mChangedVPDimension = TRUE;
	}
};

void ACCamera::SetUp(const Vector3& value)
{
	if (mUp != value)
	{
		mUp = value;
		mChangedVPDimension = TRUE;
	}
};

FLOAT ACCamera::GetWidth()
{
	return mWidth;
};

FLOAT ACCamera::GetHeight()
{
	return mHeight;
};

FLOAT ACCamera::GetNear()
{
	return mNearPlane;
};

FLOAT ACCamera::GetFar()
{
	return mFarPlane;
};

const Vector3& ACCamera::GetPosition()
{
	return mPosition;
};

const Vector3& ACCamera::GetTarget()
{
	return mTarget;
};

const Vector3& ACCamera::GetUp()
{
	return mUp;
};

