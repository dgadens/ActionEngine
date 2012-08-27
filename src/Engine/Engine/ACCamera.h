//*****************************************
// classe da camera
// creator: Daniel Gadens
// date: 19/05/2011
//****************************************

#ifndef __ACCAMERA_H
#define __ACCAMERA_H

#include "ActionTypeDef.h"
#include "EngineMath.h"

class ACCamera
{
private:
	BOOL mChangedVPDimension;
	BOOL mChangedZoom;
	BOOL mChangedPosition;

    FLOAT mHalfWidth;
    FLOAT mHalfHeight;
    FLOAT mRelativeZoomWidth;
    FLOAT mRelativeZoomHeight;
	FLOAT mWidth;
	FLOAT mHeight;

	Vector3 mPosition;
	Vector3 mTarget;
    Vector3 mUp;

	FLOAT mNearPlane;
    FLOAT mFarPlane;

public:
	ACCamera();
	~ACCamera();

    Matrix View;
    Matrix Projection;
	Matrix ViewProjection;

    Vector3 PreviousPosition;
    FLOAT Zoom;

    //boundingbox da camera
	//BOUNDINGBOX BB;
	//boundingbox dos limites do mapa
	//BOUNDINGBOX BBLimits;
	//frustum da camera
	//BOUNDINGFRUSTUM Frustum;

	//movimento suave da camera
    BOOL SmoothMoviment;

	void SetWidth(FLOAT value);
	void SetHeight(FLOAT value);
	void SetNear(FLOAT value);
	void SetFar(FLOAT value);
	void SetPosition(const Vector3& value);
	void SetTarget(const Vector3& value);
	void SetUp(const Vector3& value);

	FLOAT GetWidth();
	FLOAT GetHeight();
	FLOAT GetNear();
	FLOAT GetFar();
	const Vector3& GetPosition();
	const Vector3& GetTarget();
	const Vector3& GetUp();

	void Update();
	void ResetCamera();
	
	void Release();

};

#endif