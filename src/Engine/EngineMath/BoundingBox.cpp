#include "BoundingBox.h"
#include "Vector3.h"
#include "BoundingSphere.h"
#include "BoundingFrustum.h"
#include "Plane.h"
#include "Ray.h"
#include "MathHelper.h"
#include <math.h>

BoundingBox::BoundingBox()
{
	Min = Vector3(-1);
	Max = Vector3(1);
};

BoundingBox::BoundingBox(const Vector3 *min, const Vector3 *max)
{
	Min = Vector3(min->X, min->Y, min->Z);
	Max = Vector3(max->X, max->Y, max->Z);
};

BoundingBox::~BoundingBox()
{
};

void BoundingBox::GetCorners(const BoundingBox *box, Vector3 corners[])
{
    corners[0].X = box->Min.X;
    corners[0].Y = box->Max.Y;
    corners[0].Z = box->Max.Z;
    corners[1].X = box->Max.X;
    corners[1].Y = box->Max.Y;
    corners[1].Z = box->Max.Z;
    corners[2].X = box->Max.X;
    corners[2].Y = box->Min.Y;
    corners[2].Z = box->Max.Z;
    corners[3].X = box->Min.X;
    corners[3].Y = box->Min.Y;
    corners[3].Z = box->Max.Z;
    corners[4].X = box->Min.X;
    corners[4].Y = box->Max.Y;
    corners[4].Z = box->Min.Z;
    corners[5].X = box->Max.X;
    corners[5].Y = box->Max.Y;
    corners[5].Z = box->Min.Z;
    corners[6].X = box->Max.X;
    corners[6].Y = box->Min.Y;
    corners[6].Z = box->Min.Z;
    corners[7].X = box->Min.X;
    corners[7].Y = box->Min.Y;
    corners[7].Z = box->Min.Z;
};

BOOL BoundingBox::Equals(const BoundingBox* box1, const BoundingBox* box2)
{
	return (Vector3::Equals(&box1->Min, &box2->Min) && Vector3::Equals(&box1->Max, &box2->Max));
};

void BoundingBox::CreateMerged(const BoundingBox *original, const BoundingBox *additional, BoundingBox *result)
{
    Vector3 vector;
    Vector3 Vector2;
    Vector3::Min(&original->Min, &additional->Min, &Vector2);
    Vector3::Max(&original->Max, &additional->Max, &vector);
    result->Min.X = Vector2.X;
	result->Min.Y = Vector2.Y;
	result->Min.Z = Vector2.Z;
    result->Max.X = vector.X;
	result->Max.Y = vector.Y;
	result->Max.Z = vector.Z;
};

void BoundingBox::CreateFromSphere(const BoundingBox *box, const BoundingSphere *sphere, BoundingBox *result)
{
    result->Min.X = sphere->Center.X - sphere->Radius;
    result->Min.Y = sphere->Center.Y - sphere->Radius;
    result->Min.Z = sphere->Center.Z - sphere->Radius;
									 			 
    result->Max.X = sphere->Center.X + sphere->Radius;
    result->Max.Y = sphere->Center.Y + sphere->Radius;
    result->Max.Z = sphere->Center.Z + sphere->Radius;
};

void BoundingBox::CreateFromPoints(INT arraySize, Vector3 points[], BoundingBox *result)
{
    Vector3 vector3(FLOAT_MAXVALUE);
    Vector3 vector2(FLOAT_MINVALUE);

	for (int i = 0; i < arraySize; i++)
    {
        Vector3 vector4 = points[i];
        Vector3::Min(&vector3, &vector4, &vector3);
        Vector3::Max(&vector2, &vector4, &vector2);
    }

	result->Min.X = vector3.X;
	result->Min.Y = vector3.Y;
	result->Min.Z = vector3.Z;

	result->Max.X = vector2.X;
	result->Max.Y = vector2.Y;
	result->Max.Z = vector2.Z;
};

BOOL BoundingBox::Intersects(const BoundingBox *box1, const BoundingBox *box2)
{
	if ((box1->Max.X < box2->Min.X) || (box1->Min.X > box2->Max.X))
	{
		return FALSE;
	}
	if ((box1->Max.Y < box2->Min.Y) || (box1->Min.Y > box2->Max.Y))
	{
		return FALSE;
	}
	return ((box1->Max.Z >= box2->Min.Z) && (box1->Min.Z <= box2->Max.Z));
};

BOOL BoundingBox::Intersects(const BoundingBox *box, const BoundingFrustum *frustum)
{
	return BoundingFrustum::Intersects(frustum, box);
};

PlaneIntersectionType BoundingBox::Intersects(const BoundingBox *box, const Plane *plane)
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
    num = ((plane->Normal.X * vector.X) + (plane->Normal.Y * vector.Y)) + (plane->Normal.Z * vector.Z);
    if ((num + plane->D) < 0.0f)
    {
        return PIT_Back;
    }
    return PIT_Intersecting;
};

BOOL BoundingBox::Intersects(const BoundingBox *box, const Ray *ray, FLOAT &result)
{
    FLOAT num = 0.0f;
    FLOAT maxValue = FLOAT_MAXVALUE;
    if (abs(ray->Direction.X) < 1E-06f)
    {
        if ((ray->Position.X < box->Min.X) || (ray->Position.X > box->Max.X))
        {
            return FALSE;
        }
    }
    else
    {
        FLOAT num11 = 1.0f / ray->Direction.X;
        FLOAT num8 = (box->Min.X - ray->Position.X) * num11;
        FLOAT num7 = (box->Max.X - ray->Position.X) * num11;
        if (num8 > num7)
        {
            FLOAT num14 = num8;
            num8 = num7;
            num7 = num14;
        }
        num = MathHelper::Max(num8, num);
        maxValue = MathHelper::Min(num7, maxValue);
        if (num > maxValue)
        {
            return FALSE;
        }
    }
    if (abs(ray->Direction.Y) < 1E-06f)
    {
        if ((ray->Position.Y < box->Min.Y) || (ray->Position.Y > box->Max.Y))
        {
            return FALSE;
        }
    }
    else
    {
        FLOAT num10 = 1.0f / ray->Direction.Y;
        FLOAT num6 = (box->Min.Y - ray->Position.Y) * num10;
        FLOAT num5 = (box->Max.Y - ray->Position.Y) * num10;
        if (num6 > num5)
        {
            FLOAT num13 = num6;
            num6 = num5;
            num5 = num13;
        }
        num = MathHelper::Max(num6, num);
        maxValue = MathHelper::Min(num5, maxValue);
        if (num > maxValue)
        {
            return FALSE;
        }
    }
    if (abs(ray->Direction.Z) < 1E-06f)
    {
        if ((ray->Position.Z < box->Min.Z) || (ray->Position.Z > box->Max.Z))
        {
            return FALSE;
        }
    }
    else
    {
        FLOAT num9 = 1.0f / ray->Direction.Z;
        FLOAT num4 = (box->Min.Z - ray->Position.Z) * num9;
        FLOAT num3 = (box->Max.Z - ray->Position.Z) * num9;
        if (num4 > num3)
        {
            FLOAT num12 = num4;
            num4 = num3;
            num3 = num12;
        }
        num = MathHelper::Max(num4, num);
        maxValue = MathHelper::Min(num3, maxValue);
        if (num > maxValue)
        {
            return FALSE;
        }
    }

	result = num;
	return TRUE;
};

BOOL BoundingBox::Intersects(const BoundingBox *box, const BoundingSphere *sphere)
{
	FLOAT num;
	Vector3 vector;
	Vector3::Clamp(&sphere->Center, &box->Min, &box->Max, &vector);
	num = Vector3::DistanceSquared(&sphere->Center, &vector);
	return (num <= (sphere->Radius * sphere->Radius));
};

ContainmentType BoundingBox::Contains(const BoundingBox *box1, const BoundingBox *box2)
{
	if ((box1->Max.X < box2->Min.X) || (box1->Min.X > box2->Max.X))
	{
		return CT_Disjoint;
	}
	if ((box1->Max.Y < box2->Min.Y) || (box1->Min.Y > box2->Max.Y))
	{
		return CT_Disjoint;
	}
	if ((box1->Max.Z < box2->Min.Z) || (box1->Min.Z > box2->Max.Z))
	{
		return CT_Disjoint;
	}
	if ((((box1->Min.X <= box2->Min.X) && (box2->Max.X <= box1->Max.X)) && ((box1->Min.Y <= box2->Min.Y) && (box2->Max.Y <= box1->Max.Y))) && ((box1->Min.Z <= box2->Min.Z) && (box2->Max.Z <= box1->Max.Z)))
	{
		return CT_Contains;
	}

	return CT_Intersects;
};

ContainmentType BoundingBox::Contains(const BoundingBox *box, const Vector3 *point)
{
    if ((((box->Min.X <= point->X) && (point->X <= box->Max.X)) && ((box->Min.Y <= point->Y) && (point->Y <= box->Max.Y))) && ((box->Min.Z <= point->Z) && (point->Z <= box->Max.Z)))
    {
        return CT_Contains;
    }
    return CT_Disjoint;
};

ContainmentType BoundingBox::Contains(const BoundingBox *box, const BoundingFrustum *frustum)
{
	if (!BoundingFrustum::Intersects(frustum, box))
	{
		return CT_Disjoint;
	}

	for (int i = 0; i< BoundingFrustum::CORNER_COUNT; i++)
	{
		if (BoundingBox::Contains(box, &frustum->pCornerArray[i]) == CT_Disjoint)
		{
			return CT_Intersects;
		}
	}

	return CT_Contains;

};

ContainmentType BoundingBox::Contains(const BoundingBox *box, const BoundingSphere *sphere)
{
    FLOAT num2;
    Vector3 vector;
    Vector3::Clamp(&sphere->Center, &box->Min, &box->Max, &vector);
    num2 = Vector3::DistanceSquared(&sphere->Center, &vector);
    FLOAT radius = sphere->Radius;
    if (num2 > (radius * radius))
    {
        return CT_Disjoint;
    }
    if (((((box->Min.X + radius) <= sphere->Center.X) && (sphere->Center.X <= (box->Max.X - radius))) && (((box->Max.X - box->Min.X) > radius) && ((box->Min.Y + radius) <= sphere->Center.Y))) && (((sphere->Center.Y <= (box->Max.Y - radius)) && ((box->Max.Y - box->Min.Y) > radius)) && ((((box->Min.Z + radius) <= sphere->Center.Z) && (sphere->Center.Z <= (box->Max.Z - radius))) && ((box->Max.X - box->Min.X) > radius))))
    {
        return CT_Contains;
    }
    return CT_Intersects;

};

void BoundingBox::SupportMapping(const BoundingBox *box, const Vector3 *v, Vector3 *result)
{
	result->X = (v->X >= 0.0f) ? box->Max.X : box->Min.X;
	result->Y = (v->Y >= 0.0f) ? box->Max.Y : box->Min.Y;
	result->Z = (v->Z >= 0.0f) ? box->Max.Z : box->Min.Z;
};

void BoundingBox::Copy(const BoundingBox *source, BoundingBox *destination)
{
	destination->Min = source->Min;
	destination->Max = source->Max;
};

BOOL BoundingBox::operator ==(const BoundingBox b) const
{
	return BoundingBox::Equals(this, &b);
};

BOOL BoundingBox::operator !=(const BoundingBox b) const
{
	if (!(this->Min != b.Min))
    {
        return (this->Max != b.Max);
    }
    return true;
};
