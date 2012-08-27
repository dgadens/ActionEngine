#include "ACCamera.h"

ACCamera::ACCamera()
{
	mCameraProjection = ACCameraProjection::ACCP_Perspective;

	mFOV = PIOVER4;
    mNearPlane = 1.0f;
    mFarPlane = 1000.0f;
    mPosition.X  = 0.0f;
	mPosition.Y  = 0.0f;
	mPosition.Z  = 500.0f;
	
    mTarget = Vector3(0, 0, -1);
    mUp = Vector3(0, 1, 0);

	mChangedView = TRUE;
    mChangedProjection = TRUE;
};

ACCamera::~ACCamera()
{

};

void ACCamera::Update()
{
	//se altera a matrix de visualizacao entao ele recalcula
	if (mChangedView)
		Matrix::CreateLookAt(&mPosition, &mTarget, &mUp, &mView);

	//se mudou a matrix de projecao entao recalcula
	if (mChangedProjection)
	{
		if (mCameraProjection == ACCameraProjection::ACCP_Perspective)
			Matrix::CreatePerspectiveFieldOfView(mFOV, mWidth / mHeight, mNearPlane, mFarPlane, &mProjection);
		else
			Matrix::CreateOrthographic(mWidth, mHeight, mNearPlane, mFarPlane, &mProjection);
	}

	//se alterou qq uma entao ele recalcula a viewprojection
	if (mChangedView || mChangedProjection)
		Matrix::Multiply(&mView, &mProjection, &mViewProjection);

	mChangedProjection = FALSE;
	mChangedView = FALSE;	
};

void ACCamera::SetCameraProjection(ACCameraProjection value)
{
	if (mCameraProjection!=value)
	{
		mCameraProjection = value;
		mChangedProjection = TRUE;
	}
};

void ACCamera::SetFOV(FLOAT value)
{
	if (mFOV != value)
	{
		mFOV = value;
		mChangedProjection = TRUE;
	}
};

void ACCamera::SetWidth(FLOAT value)
{
	if (mWidth!=value)
	{
		mWidth = value;
		mChangedProjection = TRUE;
	}
};
		
void ACCamera::SetHeight(FLOAT value)
{
	if (mHeight!=value)
	{
		mHeight = value;
		mChangedProjection = TRUE;
	}
};

void ACCamera::SetNear(FLOAT value)
{
	if (mNearPlane != value)
	{
		mNearPlane = value;
		mChangedProjection = TRUE;
	}
};

void ACCamera::SetFar(FLOAT value)
{
	if (mFarPlane != value)
	{
		mFarPlane = value;
		mChangedProjection = TRUE;
	}
};

void ACCamera::SetPosition(const Vector3& value)
{
	if (mPosition != value)
	{
		mPosition = value;
		mChangedView = TRUE;
	}
};

void ACCamera::SetTarget(const Vector3& value)
{
	if (mTarget != value)
	{
		mTarget = value;
		mChangedView = TRUE;
	}
};

void ACCamera::SetUp(const Vector3& value)
{
	if (mUp != value)
	{
		mUp = value;
		mChangedView = TRUE;
	}
};

ACCameraProjection ACCamera::GetCameraProjection()
{
	return mCameraProjection;
};

FLOAT ACCamera::GetFOV()
{
	return mFOV;
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

const Matrix& ACCamera::GetView()
{
	return mView;
};

const Matrix& ACCamera::GetProjection()
{
	return mProjection;
};

const Matrix& ACCamera::GetViewProjection()
{
	return mViewProjection;
};


