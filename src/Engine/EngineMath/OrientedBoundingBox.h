
#ifndef __ORIENTEDBOUNDINGBOX_H
#define __ORIENTEDBOUNDINGBOX_H

#include "EngineCommon.h"
#include "Vector3.h"
#include "BoundingBox.h"
#include "Matrix.h"

struct OrientedBoundingBox
{

public:
	static const INT CORNER_COUNT = 8;

	Vector3 Edges[8];

	OrientedBoundingBox();
	OrientedBoundingBox(const BoundingBox* baseBB);
	~OrientedBoundingBox();

	static void CreateFromBB(const BoundingBox *box2, OrientedBoundingBox* outOBB);
	static void TransformOBB(OrientedBoundingBox* OBB, const Matrix* world, OrientedBoundingBox* outOBB);

	BOOL operator ==(const OrientedBoundingBox b) const;
	BOOL operator !=(const OrientedBoundingBox b) const;

};

#endif