#include "Plane.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingFrustum.h"
#include <math.h>

Plane::Plane() 
{ 
	D = 0.0f; 
};

Plane::Plane(FLOAT a, FLOAT b, FLOAT c, FLOAT d) 
{ 
	Normal = Vector3(a,b,c); 
	D = d; 
};

Plane::Plane(const Vector3 *normal, FLOAT d) 
{ 
	Normal = Vector3(normal->X,normal->Y,normal->Z); 
	D = d; 
};

Plane::Plane(const Vector4 *value) 
{ 
	Normal = Vector3(value->X,value->Y,value->Z); 
	D = value->W; 
};

Plane::Plane(const Vector3 *point1, const Vector3 *point2, const Vector3 *point3)
{
	FLOAT num10 = point2->X - point1->X;
	FLOAT num9 = point2->Y - point1->Y;
	FLOAT num8 = point2->Z - point1->Z;
	FLOAT num7 = point3->X - point1->X;
	FLOAT num6 = point3->Y - point1->Y;
	FLOAT num5 = point3->Z - point1->Z;
	FLOAT num4 = (num9 * num5) - (num8 * num6);
	FLOAT num3 = (num8 * num7) - (num10 * num5);
	FLOAT num2 = (num10 * num6) - (num9 * num7);
	FLOAT num11 = ((num4 * num4) + (num3 * num3)) + (num2 * num2);
	FLOAT num = 1.0f / sqrtf(num11);
	Normal = Vector3(num4 * num, num3 * num, num2 * num);
	D = -(((Normal.X * point1->X) + (Normal.Y * point1->Y)) + (Normal.Z * point1->Z));
};

Plane::~Plane()
{
};

BOOL Plane::Equals(const Plane *value1, const Plane *value2)
{
	return ((((value1->Normal.X == value2->Normal.X) && (value1->Normal.Y == value2->Normal.Y)) && (value1->Normal.Z == value2->Normal.Z)) && (value1->D == value2->D));
};

void Plane::Normalize(const Plane *value, Plane *result)
{
	FLOAT num2 = ((value->Normal.X * value->Normal.X) + (value->Normal.Y * value->Normal.Y)) + (value->Normal.Z * value->Normal.Z);
	if (abs((num2 - 1.0f)) < 1.192093E-07f)
	{
		result->Normal.X = value->Normal.X;
		result->Normal.Y = value->Normal.Y;
		result->Normal.Z = value->Normal.Z;
		result->D = value->D;
	}
	else
	{
		FLOAT num = 1.0f / sqrtf(num2);
		result->Normal.X = value->Normal.X * num;
		result->Normal.Y = value->Normal.Y * num;
		result->Normal.Z = value->Normal.Z * num;
		result->D = value->D * num;
	}
};

void Plane::Transform(const Plane *plane, const Matrix *matrix, Plane *result)
{
	Matrix matrix2;
	Matrix::Invert(matrix, &matrix2);
	FLOAT x = plane->Normal.X;
	FLOAT y = plane->Normal.Y;
	FLOAT z = plane->Normal.Z;
	FLOAT d = plane->D;
	result->Normal.X = (((x * matrix2.M11) + (y * matrix2.M12)) + (z * matrix2.M13)) + (d * matrix2.M14);
	result->Normal.Y = (((x * matrix2.M21) + (y * matrix2.M22)) + (z * matrix2.M23)) + (d * matrix2.M24);
	result->Normal.Z = (((x * matrix2.M31) + (y * matrix2.M32)) + (z * matrix2.M33)) + (d * matrix2.M34);
	result->D = (((x * matrix2.M41) + (y * matrix2.M42)) + (z * matrix2.M43)) + (d * matrix2.M44);
};

void Plane::Transform(const Plane *plane, const Quaternion *rotation, Plane *result)
{
	FLOAT num15 = rotation->X + rotation->X;
	FLOAT num5 = rotation->Y + rotation->Y;
	FLOAT num = rotation->Z + rotation->Z;
	FLOAT num14 = rotation->W * num15;
	FLOAT num13 = rotation->W * num5;
	FLOAT num12 = rotation->W * num;
	FLOAT num11 = rotation->X * num15;
	FLOAT num10 = rotation->X * num5;
	FLOAT num9 = rotation->X * num;
	FLOAT num8 = rotation->Y * num5;
	FLOAT num7 = rotation->Y * num;
	FLOAT num6 = rotation->Z * num;
	FLOAT num24 = (1.0f - num8) - num6;
	FLOAT num23 = num10 - num12;
	FLOAT num22 = num9 + num13;
	FLOAT num21 = num10 + num12;
	FLOAT num20 = (1.0f - num11) - num6;
	FLOAT num19 = num7 - num14;
	FLOAT num18 = num9 - num13;
	FLOAT num17 = num7 + num14;
	FLOAT num16 = (1.0f - num11) - num8;
	FLOAT x = plane->Normal.X;
	FLOAT y = plane->Normal.Y;
	FLOAT z = plane->Normal.Z;
	result->Normal.X = ((x * num24) + (y * num23)) + (z * num22);
	result->Normal.Y = ((x * num21) + (y * num20)) + (z * num19);
	result->Normal.Z = ((x * num18) + (y * num17)) + (z * num16);
	result->D = plane->D;
};

FLOAT Plane::Dot(const Plane *plane, const Vector4 *value)
{
	return (((plane->Normal.X * value->X) + (plane->Normal.Y * value->Y)) + (plane->Normal.Z * value->Z)) + (plane->D * value->W);
};

FLOAT Plane::DotCoordinate(const Plane *plane, const Vector3 *value)
{
	return (((plane->Normal.X * value->X) + (plane->Normal.Y * value->Y)) + (plane->Normal.Z * value->Z)) + plane->D;
};

FLOAT Plane::DotNormal(const Plane *plane, const Vector3 *value)
{
	return ((plane->Normal.X * value->X) + (plane->Normal.Y * value->Y)) + (plane->Normal.Z * value->Z);
};

PlaneIntersectionType Plane::Intersects(const Plane *plane, const BoundingBox *box)
{
	Vector3 vector;
	Vector3 Vector2;
	Vector2.X = (plane->Normal.X >= 0.0f) ? box->Min.X : box->Max.X;
	Vector2.Y = (plane->Normal.Y >= 0.0f) ? box->Min.Y : box->Max.Y;
	Vector2.Z = (plane->Normal.Z >= 0.0f) ? box->Min.Z : box->Max.Z;
	vector.X =  (plane->Normal.X >= 0.0f) ? box->Max.X : box->Min.X;
	vector.Y =  (plane->Normal.Y >= 0.0f) ? box->Max.Y : box->Min.Y;
	vector.Z =  (plane->Normal.Z >= 0.0f) ? box->Max.Z : box->Min.Z;
	FLOAT num = ((plane->Normal.X * Vector2.X) + (plane->Normal.Y * Vector2.Y)) + (plane->Normal.Z * Vector2.Z);
	if ((num + plane->D) > 0.0f)
	{
		return PIT_Front;
	}
	else
	{
		num = ((plane->Normal.X * vector.X) + (plane->Normal.Y * vector.Y)) + (plane->Normal.Z * vector.Z);
		if ((num + plane->D) < 0.0f)
		{
			return PIT_Back;
		}
		else
		{
			return PIT_Intersecting;
		}
	}
};

PlaneIntersectionType Plane::Intersects(const Plane *plane, const BoundingFrustum *frustum)
{
    return BoundingFrustum::Intersects(frustum, plane);
};

PlaneIntersectionType Plane::Intersects(const Plane *plane, const BoundingSphere *sphere)
{
	FLOAT num2 = ((sphere->Center.X * plane->Normal.X) + (sphere->Center.Y * plane->Normal.Y)) + (sphere->Center.Z * plane->Normal.Z);
    FLOAT num = num2 + plane->D;
    if (num > sphere->Radius)
    {
        return PIT_Front;
    }
    if (num < -sphere->Radius)
    {
        return PIT_Back;
    }
    return PIT_Intersecting;
};

void Plane::Copy(const Plane *source, Plane *destination)
{
	destination->Normal = source->Normal;
	destination->D = source->D; 
};


BOOL Plane::operator ==(const Plane rhs) const
{
    return Plane::Equals(this, &rhs);
};

BOOL Plane::operator !=(const Plane rhs) const
{
    if (((this->Normal.X == rhs.Normal.X) && (this->Normal.Y == rhs.Normal.Y)) && (this->Normal.Z == rhs.Normal.Z))
    {
        return (this->D != rhs.D);
    }
    return true;
};
