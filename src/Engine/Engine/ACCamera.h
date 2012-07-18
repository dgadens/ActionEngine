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

	Vector3* mpDestination;
    FLOAT mHalfWidth;
    FLOAT mHalfHeight;
    FLOAT mRelativeZoomWidth;
    FLOAT mRelativeZoomHeight;
	FLOAT mWidth;
	FLOAT mHeight;

public:
	ACCamera();
	~ACCamera();

	Vector3 Position;
	Vector3 Target;
    Vector3 Up;

	FLOAT NearPlane;
    FLOAT FarPlane;

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

	//pega a velocidade da camera para movimentar os objetos conforme
    Vector3 CurrentCameraMoviment;

    //usado para cameras orthograficas
	void SetWidth(FLOAT value);
	void SetHeight(FLOAT value);
	void Update();
	void ResetCamera();
	
	void Release();

};

#endif