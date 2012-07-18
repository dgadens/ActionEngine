
#ifndef __RAY_H
#define __RAY_H

#include "EngineCommon.h"
#include "Vector3.h"

struct Matrix;
struct BoundingBox;
struct BoundingFrustum;
struct BoundingSphere;
struct Plane;

struct Ray
{
public:
	Vector3 Position;
	Vector3 Direction;
    
	Ray();
	Ray(const Vector3 *position, const Vector3 *direction);
	~Ray();

	static BOOL Equals(const Ray *value1, const Ray *value2);

	static BOOL Intersects(const Ray *ray, const BoundingBox *box, FLOAT &result);
	static BOOL Intersects(const Ray *ray, const BoundingFrustum *frustum, FLOAT &result);
	static BOOL Intersects(const Ray *ray, const Plane *plane, FLOAT &result);
	static BOOL Intersects(const Ray *ray, const BoundingSphere *sphere, FLOAT &result);
	static void GetPickRay(const Vector2 *mousePosition, const Matrix *projection, const Matrix *view, INT viewPortWidth, INT viewPortHeight, FLOAT nearPlane, FLOAT farPlane, Ray *result);
	static void Copy(const Ray *source, Ray *destination);

	BOOL operator ==(const Ray b) const;
    BOOL operator !=(const Ray b) const;

};

#endif