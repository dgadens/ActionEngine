
#ifndef __BOUNDINGSPHERE_H
#define __BOUNDINGSPHERE_H

#include "EngineCommon.h"
#include "Vector3.h"

struct BoundingBox;
struct BoundingFrustum;
struct Ray;
struct Matrix;
struct Plane;

struct BoundingSphere
{
public:
	Vector3 Center;
    FLOAT Radius;

	BoundingSphere();
	BoundingSphere(const Vector3 *center, float radius);
	~BoundingSphere();

	static BOOL Equals(const BoundingSphere *value1, const BoundingSphere *value2);
	static void CreateMerged(const BoundingSphere *original, const BoundingSphere *additional, BoundingSphere *result);
	static void CreateFromBoundingBox(const BoundingBox *box, BoundingSphere *result);
	static void CreateFromPoints(INT arraySize, Vector3 points[], BoundingSphere *result);
	static void CreateFromFrustum(const BoundingFrustum *frustum, BoundingSphere *result);
	static BOOL Intersects(const BoundingSphere *sphere, const BoundingBox *box);
	static BOOL Intersects(const BoundingSphere *sphere, const BoundingFrustum *frustum);
	static PlaneIntersectionType Intersects(const BoundingSphere *sphere, const Plane *plane);
	static BOOL Intersects(const BoundingSphere *sphere, const Ray *ray, float &result);
	static BOOL Intersects(const BoundingSphere *sphere1, const BoundingSphere *sphere2);
	static ContainmentType Contains(const BoundingSphere *sphere, const BoundingBox *box);
	static ContainmentType Contains(const BoundingSphere *sphere, const BoundingFrustum *frustum);
	static ContainmentType Contains(const BoundingSphere *sphere, const Vector3 *point);
	static ContainmentType Contains(const BoundingSphere *sphere1, const BoundingSphere *sphere2);
	static void SupportMapping(const BoundingSphere *sphere, const Vector3 *v, Vector3 *result);
	static void Transform(const BoundingSphere *sphere, const Matrix *matrix, BoundingSphere *result);
	static void Copy(const BoundingSphere *source, BoundingSphere *destination);

    BOOL operator ==(const BoundingSphere b) const;
	BOOL operator !=(const BoundingSphere b) const;

};

#endif