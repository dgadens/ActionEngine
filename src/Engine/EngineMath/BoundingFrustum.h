
#ifndef __BOUNDINGFRUSTUM_H
#define __BOUNDINGFRUSTUM_H

#include "EngineCommon.h"

struct Vector3;
struct BoundingSphere;
struct BoundingBox;
struct Plane;
struct Gjk;
struct Matrix;
struct Ray;

struct BoundingFrustum
{
private:
	static const INT NEAR_PLANE_INDEX = 0;
	static const INT FAR_PLANE_INDEX = 1;
	static const INT LEFT_PLANE_INDEX = 2;
	static const INT RIGHT_PLANE_INDEX = 3;
	static const INT TOP_PLANE_INDEX = 4;
	static const INT BOTTOM_PLANE_INDEX = 5;
	static const INT NUM_PLANES = 6;

public:

	static const INT CORNER_COUNT = 8;
	static const INT PLANE_COUNT = 6;

	Gjk *pGjk;
	Vector3* pCornerArray;
	Plane* pPlanesArray;
	Matrix* pMatrix;

	BoundingFrustum();
	BoundingFrustum(const Matrix *value);
	~BoundingFrustum();

	void SetMatrix(const Matrix *value);

	static BOOL Equals(const BoundingFrustum *value1, const BoundingFrustum *value2);
	static void ComputeIntersection(const Plane *plane, const Ray *ray, Vector3 *result);
	static void ComputeIntersectionLine(const Plane *p1, const Plane *p2, Ray *result);
	static ContainmentType Contains(const BoundingFrustum *frustum, BoundingBox *box);
	static ContainmentType Contains(const BoundingFrustum *frustum1, const BoundingFrustum *frustum2);
	static ContainmentType Contains(const BoundingFrustum *frustum, const BoundingSphere *sphere);
	static ContainmentType Contains(const BoundingFrustum *frustum, const Vector3 *point);
	static BOOL Intersects(const BoundingFrustum *frustum, const BoundingBox *box);
	static BOOL Intersects(const BoundingFrustum *frustum, const BoundingSphere *sphere);
	static BOOL Intersects(const BoundingFrustum *frustum1, const BoundingFrustum *frustum2);
	static PlaneIntersectionType Intersects(const BoundingFrustum *frustum, const Plane *plane);
	static BOOL Intersects(const BoundingFrustum *frustum, const Ray *ray, float &result);
    static void SupportMapping(const BoundingFrustum *frustum, const Vector3 *v, Vector3 *result);
	static void Copy(const BoundingFrustum *source, BoundingFrustum *destination);

	BOOL operator ==(const BoundingFrustum b) const;
	BOOL operator !=(const BoundingFrustum b) const;

};

#endif