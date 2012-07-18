#include "BoundingSphere.h"
#include "Matrix.h"
#include "EngineCommon.h"
#include "BoundingBox.h"
#include "BoundingFrustum.h"
#include "Plane.h"
#include "Ray.h"
#include <math.h>

BoundingSphere::BoundingSphere()
{
	Center = Vector3(0.0f,0.0f,0.0f);
	Radius = 0.0f;
};

BoundingSphere::BoundingSphere(const Vector3* center, FLOAT radius)
{
	Center = Vector3(center->X, center->Y, center->Z);
	Radius = radius;
};

BoundingSphere::~BoundingSphere()
{
};

BOOL BoundingSphere::Equals(const BoundingSphere* value1, const BoundingSphere* value2)
{
	return Vector3::Equals(&value1->Center, &value2->Center) && value1->Radius == value2->Radius;
};

void BoundingSphere::CreateMerged(const BoundingSphere *original, const BoundingSphere *additional, BoundingSphere *result)
{
	Vector3 Vector2;
    Vector3::Subtract(&additional->Center, &original->Center, &Vector2);
    FLOAT num = Vector3::Length(&Vector2);
    FLOAT radius = original->Radius;
    FLOAT num2 = additional->Radius;
    if ((radius + num2) >= num)
    {
        if ((radius - num2) >= num)
        {
			result = new BoundingSphere(&original->Center, original->Radius);
            return;
        }
        if ((num2 - radius) >= num)
        {
            result = new BoundingSphere(&additional->Center, additional->Radius);
            return;
        }
    }
    Vector3 vector;
	Vector3::Multiply(&Vector2, 1.0f / num, &vector);
    FLOAT num5 = GenericTools::Min(-radius, num - num2);;
	FLOAT num4 = (GenericTools::Max(radius, num + num2) - num5) * 0.5f;
	Vector3::Multiply(&vector, num4 + num5, &vector);
	Vector3::Add(&original->Center, &vector, &result->Center);
    result->Radius = num4;
};

void BoundingSphere::CreateFromBoundingBox(const BoundingBox *box, BoundingSphere *result)
{
	Vector3::Lerp(&box->Min, &box->Max, 0.5f, &result->Center);
    FLOAT num = Vector3::Distance(&box->Min, &box->Max);
    result->Radius = num * 0.5f;
};

void BoundingSphere::CreateFromPoints(INT arraySize, Vector3 points[], BoundingSphere *result)
{
    Vector3 Vector2;
    Vector3 vector5;
    Vector3 vector6;
    Vector3 vector7;
    Vector3 vector8;
    Vector3 vector9;
    
    Vector3 Vector4 = vector5 = vector6 = vector7 = vector8 = vector9 = points[0];
	for(int i = 1; i < arraySize; i++)
	{
		if (points[i].X < Vector4.X)
            Vector4 = points[i];
        if (points[i].X > vector5.X)
            vector5 = points[i];
        if (points[i].Y < vector6.Y)
            vector6 = points[i];
        if (points[i].Y > vector7.Y)
            vector7 = points[i];
        if (points[i].Z < vector8.Z)
            vector8 = points[i];
        if (points[i].Z > vector9.Z)
            vector9 = points[i];
	}
    
    FLOAT num5 = Vector3::Distance(&vector5, &Vector4);
    FLOAT num4 = Vector3::Distance(&vector7, &vector6);
	FLOAT num2 = Vector3::Distance(&vector9, &vector8);
	FLOAT num = 0;
    if (num5 > num4)
    {
        if (num5 > num2)
        {
            Vector3::Lerp(&vector5, &Vector4, 0.5f, &Vector2);
            num = num5 * 0.5f;
        }
        else
        {
            Vector3::Lerp(&vector9, &vector8, 0.5f, &Vector2);
            num = num2 * 0.5f;
        }
    }
    else if (num4 > num2)
    {
        Vector3::Lerp(&vector7, &vector6, 0.5f, &Vector2);
        num = num4 * 0.5f;
    }
    else
    {
        Vector3::Lerp(&vector9, &vector8, 0.5f, &Vector2);
        num = num2 * 0.5f;
    }
	for(int i = 0; i < arraySize; i++)
	{
		Vector3 Vector3;
        Vector3.X = points[i].X - Vector2.X;
        Vector3.Y = points[i].Y - Vector2.Y;
        Vector3.Z = points[i].Z - Vector2.Z;
        FLOAT num3 = Vector3::Length(&Vector3);
        if (num3 > num)
        {
            num = (num + num3) * 0.5f;
			Vector3::Multiply(&Vector3, 1.0f - (num / num3), &Vector3);
			Vector3::Add(&Vector2, &Vector3, &Vector2);
        }
	}
	result->Center.X = Vector2.X;
	result->Center.Y = Vector2.Y;
	result->Center.Z = Vector2.Z;
	result->Radius = num;
};

void BoundingSphere::CreateFromFrustum(const BoundingFrustum *frustum, BoundingSphere *result)
{
	BoundingSphere::CreateFromPoints(BoundingFrustum::CORNER_COUNT, frustum->pCornerArray, result);
};
	
BOOL BoundingSphere::Intersects(const BoundingSphere *sphere, const BoundingBox *box)
{
	FLOAT num = 0;
    Vector3 vector(0);
    Vector3::Clamp(&sphere->Center, &box->Min, &box->Max, &vector);
    num = Vector3::DistanceSquared(&sphere->Center, &vector);
    return (num <= (sphere->Radius * sphere->Radius));
};

BOOL BoundingSphere::Intersects(const BoundingSphere *sphere, const BoundingFrustum *frustum)
{
    return BoundingFrustum::Intersects(frustum, sphere);
};

PlaneIntersectionType BoundingSphere::Intersects(const BoundingSphere *sphere, const Plane *plane)
{
	return Plane::Intersects(plane, sphere);
};

BOOL BoundingSphere::Intersects(const BoundingSphere *sphere, const Ray *ray, FLOAT &result)
{
	return Ray::Intersects(ray, sphere, result);
};

BOOL BoundingSphere::Intersects(const BoundingSphere *sphere1, const BoundingSphere *sphere2)
{
	FLOAT num3;
	num3 = Vector3::DistanceSquared(&sphere1->Center, &sphere2->Center);
	FLOAT radius = sphere1->Radius;
	FLOAT num = sphere2->Radius;
	if ((((radius * radius) + ((2.0f * radius) * num)) + (num * num)) <= num3)
	{
		return FALSE;
	}
	return TRUE;
};

ContainmentType BoundingSphere::Contains(const BoundingSphere *sphere, const BoundingBox *box)
{
	Vector3 vector(0);
	if (!BoundingBox::Intersects(box, sphere))
	{
		return CT_Disjoint;
	}

	FLOAT num = sphere->Radius * sphere->Radius;
	vector.X = sphere->Center.X - box->Min.X;
	vector.Y = sphere->Center.Y - box->Max.Y;
	vector.Z = sphere->Center.Z - box->Max.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Max.X;
	vector.Y = sphere->Center.Y - box->Max.Y;
	vector.Z = sphere->Center.Z - box->Max.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Max.X;
	vector.Y = sphere->Center.Y - box->Min.Y;
	vector.Z = sphere->Center.Z - box->Max.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Min.X;
	vector.Y = sphere->Center.Y - box->Min.Y;
	vector.Z = sphere->Center.Z - box->Max.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Min.X;
	vector.Y = sphere->Center.Y - box->Max.Y;
	vector.Z = sphere->Center.Z - box->Min.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Max.X;
	vector.Y = sphere->Center.Y - box->Max.Y;
	vector.Z = sphere->Center.Z - box->Min.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Max.X;
	vector.Y = sphere->Center.Y - box->Min.Y;
	vector.Z = sphere->Center.Z - box->Min.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}
	vector.X = sphere->Center.X - box->Min.X;
	vector.Y = sphere->Center.Y - box->Min.Y;
	vector.Z = sphere->Center.Z - box->Min.Z;
	if (Vector3::LengthSquared(&vector) > num)
	{
		return CT_Intersects;
	}

	return CT_Contains;
};

ContainmentType BoundingSphere::Contains(const BoundingSphere *sphere, const BoundingFrustum *frustum)
{
    if (!BoundingFrustum::Intersects(frustum, sphere))
    {
        return CT_Disjoint;
    }

    FLOAT num2 = sphere->Radius * sphere->Radius;
	for (INT i = 0; i < BoundingFrustum::CORNER_COUNT; i++)
	{
        Vector3 vector(0);
        vector.X = frustum->pCornerArray[i].X - sphere->Center.X;
        vector.Y = frustum->pCornerArray[i].Y - sphere->Center.Y;
        vector.Z = frustum->pCornerArray[i].Z - sphere->Center.Z;
        if (Vector3::LengthSquared(&vector) > num2)
        {
            return CT_Intersects;
        }
	}

    return CT_Contains;
};

ContainmentType BoundingSphere::Contains(const BoundingSphere *sphere, const Vector3 *point)
{
	if (Vector3::DistanceSquared(point, &sphere->Center) >= (sphere->Radius * sphere->Radius))
	{
		return CT_Disjoint;
	}
	return CT_Contains;
};

ContainmentType BoundingSphere::Contains(const BoundingSphere *sphere1, const BoundingSphere *sphere2)
{
	FLOAT num3;
    num3 = Vector3::Distance(&sphere1->Center, &sphere2->Center);
    FLOAT radius = sphere1->Radius;
    FLOAT num = sphere2->Radius;
    if ((radius + num) < num3)
    {
        return CT_Disjoint;
    }
    if ((radius - num) < num3)
    {
        return CT_Intersects;
    }
    return CT_Contains;

};

void BoundingSphere::SupportMapping(const BoundingSphere *sphere, const Vector3 *v, Vector3 *result)
{
	FLOAT num2 = Vector3::Length(v);
    FLOAT num = sphere->Radius / num2;
    result->X = 0;
	result->Y = 0;
	result->Z = 0;

    result->X = sphere->Center.X + (v->X * num);
    result->Y = sphere->Center.Y + (v->Y * num);
    result->Z = sphere->Center.Z + (v->Z * num);
};

void BoundingSphere::Transform(const BoundingSphere *sphere, const Matrix *matrix, BoundingSphere *result)
{
	Vector3::Transform(&sphere->Center, matrix, &result->Center);
    FLOAT num4 = ((matrix->M11 * matrix->M11) + (matrix->M12 * matrix->M12)) + (matrix->M13 * matrix->M13);
    FLOAT num3 = ((matrix->M21 * matrix->M21) + (matrix->M22 * matrix->M22)) + (matrix->M23 * matrix->M23);
    FLOAT num2 = ((matrix->M31 * matrix->M31) + (matrix->M32 * matrix->M32)) + (matrix->M33 * matrix->M33);
    FLOAT num = GenericTools::Max(num4, GenericTools::Max(num3, num2));
    result->Radius = sphere->Radius * (sqrt(num));
};

void BoundingSphere::Copy(const BoundingSphere *source, BoundingSphere *destination)
{
	Vector3::Copy(&source->Center, &destination->Center);
	destination->Radius = source->Radius;
};


BOOL BoundingSphere::operator ==(const BoundingSphere b) const
{
	return BoundingSphere::Equals(this, &b);
};

BOOL BoundingSphere::operator !=(const BoundingSphere b) const
{
	if (!(this->Center != b.Center))
    {
        return (this->Radius != b.Radius);
    }
    return true;
};
