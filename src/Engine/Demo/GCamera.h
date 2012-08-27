//*****************
// Criacao de uma camera para esse demo
// Creator: Daniel Gadens
// Date: 27/08/2012

#ifndef __GCAMERA_H
#define __GCAMERA_H

#include "ACCamera.h"
#include "GGameInput.h"

enum GCameraType
{
	Free,
	SideScroller
};

class GCamera : public ACCamera
{
	GCameraType mCameraType;

	Vector3 mAngle;
	Vector3 mTranslation;

	float mElapsedTime;

public:
	GCamera();
	GCamera(GCameraType type);
	~GCamera();

	void ProcessInput();
	void Update(FLOAT elapsedTime);
};

#endif