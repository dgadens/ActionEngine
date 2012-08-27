//*****************************************
// classe da camera
// creator: Daniel Gadens
// date: 19/05/2011
//****************************************

#ifndef __ACCAMERA_H
#define __ACCAMERA_H

#include "ActionTypeDef.h"
#include "EngineMath.h"

enum ACCameraProjection
{
	ACCP_Perspective,
	ACCP_Orthographic
};

class ACCamera
{
private:
	ACCameraProjection mCameraProjection;

	Matrix mView;
    Matrix mProjection;
	Matrix mViewProjection;

	FLOAT mFOV;

protected:

	BOOL mChangedProjection;
	BOOL mChangedView;

	FLOAT mWidth;
	FLOAT mHeight;

	Vector3 mPosition;
	Vector3 mTarget;
    Vector3 mUp;

	FLOAT mNearPlane;
    FLOAT mFarPlane;

public:
	ACCamera();
	virtual ~ACCamera();

	//sets
	void SetCameraProjection(ACCameraProjection value);
	void SetFOV(FLOAT value);
	void SetWidth(FLOAT value);
	void SetHeight(FLOAT value);
	void SetNear(FLOAT value);
	void SetFar(FLOAT value);
	void SetPosition(const Vector3& value);
	void SetTarget(const Vector3& value);
	void SetUp(const Vector3& value);

	//gets
	ACCameraProjection GetCameraProjection();
	FLOAT GetFOV();
	FLOAT GetWidth();
	FLOAT GetHeight();
	FLOAT GetNear();
	FLOAT GetFar();

	const Vector3& GetPosition();
	const Vector3& GetTarget();
	const Vector3& GetUp();

	const Matrix& GetView();
	const Matrix& GetProjection();
	const Matrix& GetViewProjection();

	void Update();
};

#endif