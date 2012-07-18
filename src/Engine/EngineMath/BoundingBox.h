
#ifndef __BOUNDINGBOX_H
#define __BOUNDINGBOX_H

#include "EngineCommon.h"
#include "Vector3.h"

struct BoundingSphere;
struct BoundingFrustum;
struct Ray;
struct Plane;

struct BoundingBox
{
public:
	static const INT CORNER_COUNT = 8;

	Vector3 Min;
	Vector3 Max;

	BoundingBox();
	BoundingBox(const Vector3 *min, const Vector3 *max);
	~BoundingBox();

	static void GetCorners(const BoundingBox *box, Vector3 corners[]);
	static BOOL Equals(const BoundingBox *box1, const BoundingBox *box2);
	static void CreateMerged(const BoundingBox *original, const BoundingBox *additional, BoundingBox *result);
	static void CreateFromSphere(const BoundingBox *box, const BoundingSphere *sphere, BoundingBox *result);
	static void CreateFromPoints(INT arraySize, Vector3 points[], BoundingBox *result);
	static BOOL Intersects(const BoundingBox *box1, const BoundingBox *box2);
	static BOOL Intersects(const BoundingBox *box, const BoundingFrustum *frustum);
	static PlaneIntersectionType Intersects(const BoundingBox *box, const Plane *plane);
	static BOOL Intersects(const BoundingBox *box, const Ray *ray, float &result);
	static BOOL Intersects(const BoundingBox *box, const BoundingSphere *sphere);
	static ContainmentType Contains(const BoundingBox *box1, const BoundingBox *box2);
	static ContainmentType Contains(const BoundingBox *box, const Vector3 *point);
	static ContainmentType Contains(const BoundingBox *box, const BoundingFrustum *frustum);
	static ContainmentType Contains(const BoundingBox *box, const BoundingSphere *sphere);
	static void SupportMapping(const BoundingBox *box, const Vector3 *v, Vector3 *result);
	static void Copy(const BoundingBox *source, BoundingBox *destination);

	BOOL operator ==(const BoundingBox b) const;
	BOOL operator !=(const BoundingBox b) const;

};

#endif