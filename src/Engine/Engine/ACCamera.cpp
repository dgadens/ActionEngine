#include "ACCamera.h"

ACCamera::ACCamera()
{
	mpDestination = nullptr;
	ResetCamera();
};

ACCamera::~ACCamera()
{
	Release();
};

void ACCamera::Release()
{
	SAFE_DELETE(mpDestination);
};

void ACCamera::ResetCamera()
{
    NearPlane = 1.0f;
    FarPlane = 1000.0f;
    Position.X  = 0.0f;
	Position.Y  = 0.0f;
	Position.Z  = 500.0f;

    PreviousPosition = Position;

    Target = Vector3(0, 0, -1);
    Up = Vector3(0, 1, 0);

	Zoom = 800;

    CurrentCameraMoviment = Vector3(0,0,0);

    mChangedPosition = FALSE;
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

void ACCamera::Update()
{
    mChangedZoom = FALSE;
    mChangedPosition = TRUE;

	Matrix::CreateLookAt(&Position, &Target, &Up, &View);
	Matrix::CreatePerspectiveFieldOfView(PIOVER4, mWidth / mHeight, NearPlane, FarPlane, &Projection);

	Matrix::Multiply(&View, &Projection, &ViewProjection);
};
