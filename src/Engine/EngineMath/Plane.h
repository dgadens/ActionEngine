
#ifndef __PLANE_H
#define __PLANE_H

#include "EngineCommon.h"
#include "Vector3.h"

struct Vector4;
struct Matrix;
struct Quaternion;
struct BoundingBox;
struct BoundingSphere;
struct BoundingFrustum;

struct Plane
{
public:
	Vector3 Normal;
	FLOAT D;

	Plane();
	Plane(FLOAT a, FLOAT b, FLOAT c, FLOAT d);
	Plane(const Vector3 *normal, FLOAT d);
	Plane(const Vector4 *value);
	Plane(const Vector3 *point1, const Vector3 *point2, const Vector3 *point3);
	~Plane();

	static BOOL Equals(const Plane *value1, const Plane *value2);
	static void Normalize(const Plane *value, Plane *result);
	static void Transform(const Plane *plane, const Matrix *matrix, Plane *result);
	static void Transform(const Plane *plane, const Quaternion *rotation, Plane *result);
	static FLOAT Dot(const Plane *plane, const Vector4 *value);
	static FLOAT DotCoordinate(const Plane *plane, const Vector3 *value);
	static FLOAT DotNormal(const Plane *plane, const Vector3 *value);
	static PlaneIntersectionType Intersects(const Plane *plane, const BoundingBox *box);
	static PlaneIntersectionType Intersects(const Plane *plane, const BoundingFrustum *frustum);
	static PlaneIntersectionType Intersects(const Plane *plane, const BoundingSphere *sphere);
	static void Copy(const Plane *source, Plane *destination);

	BOOL Plane::operator ==(const Plane rhs) const;
	BOOL Plane::operator !=(const Plane rhs) const;
	
};

#endif