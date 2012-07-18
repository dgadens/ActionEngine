#include "BoundingFrustum.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Plane.h"
#include "Ray.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Gjk.h"

BoundingFrustum::BoundingFrustum()
{
	pPlanesArray = new Plane[6];
	pCornerArray = new Vector3[8];
	pMatrix = new Matrix();
	pGjk = new Gjk();
};

BoundingFrustum::BoundingFrustum(const Matrix *value)
{
	pPlanesArray = new Plane[BoundingFrustum::PLANE_COUNT];
	pCornerArray = new Vector3[BoundingFrustum::CORNER_COUNT];
	pGjk = new Gjk();
	pMatrix = new Matrix();
	SetMatrix(value);
};

BoundingFrustum::~BoundingFrustum()
{
	delete [] pPlanesArray;
	pPlanesArray = nullptr;
	delete [] pCornerArray;
	pCornerArray = nullptr;
	delete pGjk;
	pGjk = nullptr;
	delete pMatrix;
	pMatrix = nullptr;
};

void BoundingFrustum::SetMatrix(const Matrix *value)
{
	Matrix::Copy(value, this->pMatrix);

    pPlanesArray[2].Normal.X = -value->M14 - value->M11;
    pPlanesArray[2].Normal.Y = -value->M24 - value->M21;
    pPlanesArray[2].Normal.Z = -value->M34 - value->M31;
    pPlanesArray[2].D = -value->M44 - value->M41;
    pPlanesArray[3].Normal.X = -value->M14 + value->M11;
    pPlanesArray[3].Normal.Y = -value->M24 + value->M21;
    pPlanesArray[3].Normal.Z = -value->M34 + value->M31;
    pPlanesArray[3].D = -value->M44 + value->M41;
    pPlanesArray[4].Normal.X = -value->M14 + value->M12;
    pPlanesArray[4].Normal.Y = -value->M24 + value->M22;
    pPlanesArray[4].Normal.Z = -value->M34 + value->M32;
    pPlanesArray[4].D = -value->M44 + value->M42;
    pPlanesArray[5].Normal.X = -value->M14 - value->M12;
    pPlanesArray[5].Normal.Y = -value->M24 - value->M22;
    pPlanesArray[5].Normal.Z = -value->M34 - value->M32;
    pPlanesArray[5].D = -value->M44 - value->M42;
    pPlanesArray[0].Normal.X = -value->M13;
    pPlanesArray[0].Normal.Y = -value->M23;
    pPlanesArray[0].Normal.Z = -value->M33;
    pPlanesArray[0].D = -value->M43;
    pPlanesArray[1].Normal.X = -value->M14 + value->M13;
    pPlanesArray[1].Normal.Y = -value->M24 + value->M23;
    pPlanesArray[1].Normal.Z = -value->M34 + value->M33;
    pPlanesArray[1].D = -value->M44 + value->M43;
    for (int i = 0; i < 6; i++)
    {
		FLOAT num2 = Vector3::Length(&pPlanesArray[i].Normal);
		Vector3::Divide(&pPlanesArray[i].Normal, num2, &pPlanesArray[i].Normal);
        pPlanesArray[i].D /= num2;
    }
    Ray ray;
	ComputeIntersectionLine(&pPlanesArray[0], &pPlanesArray[2], &ray);
    ComputeIntersection(&pPlanesArray[4], &ray, &pCornerArray[0]);
    ComputeIntersection(&pPlanesArray[5], &ray, &pCornerArray[3]);
    ComputeIntersectionLine(&pPlanesArray[3], &pPlanesArray[0], &ray);
    ComputeIntersection(&pPlanesArray[4], &ray, &pCornerArray[1]);
    ComputeIntersection(&pPlanesArray[5], &ray, &pCornerArray[2]);
    ComputeIntersectionLine(&pPlanesArray[2], &pPlanesArray[1], &ray);
    ComputeIntersection(&pPlanesArray[4], &ray, &pCornerArray[4]);
    ComputeIntersection(&pPlanesArray[5], &ray, &pCornerArray[7]);
    ComputeIntersectionLine(&pPlanesArray[1], &pPlanesArray[3], &ray);
    ComputeIntersection(&pPlanesArray[4], &ray, &pCornerArray[5]);
    ComputeIntersection(&pPlanesArray[5], &ray, &pCornerArray[6]);
};

BOOL BoundingFrustum::Equals(const BoundingFrustum *value1, const BoundingFrustum *value2)
{
	return Matrix::Equals(value1->pMatrix, value2->pMatrix);
};

void BoundingFrustum::ComputeIntersection(const Plane *plane, const Ray *ray, Vector3 *result)
{
	FLOAT num = (-plane->D - Vector3::Dot(&plane->Normal, &ray->Position)) / Vector3::Dot(&plane->Normal, &ray->Direction);
	Vector3 dir;
	Vector3::Multiply(&ray->Direction, num, &dir);
	Vector3::Add(&ray->Position, &dir, result);
};

void BoundingFrustum::ComputeIntersectionLine(const Plane *p1, const Plane *p2, Ray *result)
{
	Ray ray;
	Vector3::Cross(&p1->Normal, &p2->Normal, &ray.Direction);

	FLOAT num = Vector3::LengthSquared(&ray.Direction);
	Vector3 v1;
	Vector3::Multiply(&p2->Normal, -p1->D, &v1);
	Vector3 v2;
	Vector3::Multiply(&p1->Normal, -p2->D, &v2);
	Vector3 v1v2;
	Vector3::Add(&v1, &v2, &v1v2);
	Vector3::Cross(&v1v2, &ray.Direction, &v1v2);
	Vector3::Divide(&v1v2, num, &ray.Position);

	result->Position = ray.Position;
	result->Direction = ray.Direction;
};

ContainmentType BoundingFrustum::Contains(const BoundingFrustum *frustum, BoundingBox *box)
{
    BOOL flag = FALSE;
	for (int i = 0; i < BoundingFrustum::PLANE_COUNT; i++)
	{
        switch (BoundingBox::Intersects(box, &frustum->pPlanesArray[i]))
        {
            case PIT_Front:
                return CT_Disjoint;

            case PIT_Intersecting:
                flag = true;
                break;
        }
	}

    if (!flag)
    {
        return CT_Contains;
    }

    return CT_Intersects;
};

ContainmentType BoundingFrustum::Contains(const BoundingFrustum *frustum1, const BoundingFrustum *frustum2)
{
    ContainmentType disjoint = CT_Disjoint;
    if (BoundingFrustum::Intersects(frustum1, frustum2))
    {
        disjoint = CT_Contains;
        for (int i = 0; i < BoundingFrustum::CORNER_COUNT; i++)
        {
            if (BoundingFrustum::Contains(frustum1, &frustum2->pCornerArray[i]) == CT_Disjoint)
            {
                return CT_Intersects;
            }
        }
    }
    return disjoint;
};

ContainmentType BoundingFrustum::Contains(const BoundingFrustum *frustum, const BoundingSphere *sphere)
{
	Vector3 center(sphere->Center.X,sphere->Center.Y,sphere->Center.Z);
    FLOAT radius = sphere->Radius;
    int num2 = 0;
	for (int i = 0; i < BoundingFrustum::PLANE_COUNT; i++)
	{
        FLOAT num5 = ((frustum->pPlanesArray[i].Normal.X * center.X) + (frustum->pPlanesArray[i].Normal.Y * center.Y)) + (frustum->pPlanesArray[i].Normal.Z * center.Z);
        FLOAT num3 = num5 + frustum->pPlanesArray[i].D;
        if (num3 > radius)
        {
            return CT_Disjoint;
        }
        if (num3 < -radius)
        {
            num2++;
        }
    }
    if (num2 != 6)
    {
        return CT_Intersects;
    }

    return CT_Contains;
};

ContainmentType BoundingFrustum::Contains(const BoundingFrustum *frustum, const Vector3 *point)
{
	for (int i = 0; i < BoundingFrustum::PLANE_COUNT; i++)
	{
        FLOAT num2 = (((frustum->pPlanesArray[i].Normal.X * point->X) + (frustum->pPlanesArray[i].Normal.Y * point->Y)) + (frustum->pPlanesArray[i].Normal.Z * point->Z)) + frustum->pPlanesArray[i].D;
        if (num2 > 1E-05f)
        {
            return CT_Disjoint;
        }
	}

    return CT_Contains;
};

BOOL BoundingFrustum::Intersects(const BoundingFrustum *frustum, const BoundingBox *box)
{
	BOOL result = FALSE;
    Vector3 closestPoint;
    Vector3 vector2(0);
    Vector3 vector3(0);
    Vector3 vector4(0);
    Vector3 vector5(0);

    frustum->pGjk->Reset();

    Vector3::Subtract(&frustum->pCornerArray[0], &box->Min, &closestPoint);
    if (Vector3::LengthSquared(&closestPoint) < 1E-05f)
    {
        Vector3::Subtract(&frustum->pCornerArray[0], &box->Max, &closestPoint);
    }
    FLOAT maxValue = FLOAT_MAXVALUE;
    FLOAT num3 = 0.0f;
    result = FALSE;
Label_006D:
    vector5.X = -closestPoint.X;
    vector5.Y = -closestPoint.Y;
    vector5.Z = -closestPoint.Z;
    BoundingFrustum::SupportMapping(frustum, &vector5, &vector4);
    BoundingBox::SupportMapping(box, &closestPoint, &vector3);
    Vector3::Subtract(&vector4, &vector3, &vector2);
    FLOAT num4 = ((closestPoint.X * vector2.X) + (closestPoint.Y * vector2.Y)) + (closestPoint.Z * vector2.Z);
    if (num4 <= 0.0f)
    {
        frustum->pGjk->AddSupportPoint(&vector2);
        closestPoint = *frustum->pGjk->ClosestPoint();
        FLOAT num2 = maxValue;
        maxValue = Vector3::LengthSquared(&closestPoint);
        if ((num2 - maxValue) > (1E-05f * num2))
        {
            num3 = 4E-05f * frustum->pGjk->MaxLengthSquared();
            if (!frustum->pGjk->FullSimplex() && (maxValue >= num3))
            {
                goto Label_006D;
            }
            result = TRUE;
        }
    }

	return result;
};

BOOL BoundingFrustum::Intersects(const BoundingFrustum *frustum, const BoundingSphere *sphere)
{
	BOOL result = FALSE;
	Vector3 unitX(0);
    Vector3 vector2(0);
    Vector3 vector3(0);
    Vector3 vector4(0);
    Vector3 vector5(0);
    frustum->pGjk->Reset();
    Vector3::Subtract(&frustum->pCornerArray[0], &sphere->Center, &unitX);
    if (Vector3::LengthSquared(&unitX) < 1E-05f)
    {
        unitX = Vector3(1,0,0);
    }
    FLOAT maxValue = FLOAT_MAXVALUE;
    FLOAT num3 = 0.0f;
    result = FALSE;
Label_005A:
    vector5.X = -unitX.X;
    vector5.Y = -unitX.Y;
    vector5.Z = -unitX.Z;
    BoundingFrustum::SupportMapping(frustum, &vector5, &vector4);
    BoundingSphere::SupportMapping(sphere, &unitX, &vector3);
    Vector3::Subtract(&vector4, &vector3, &vector2);
    FLOAT num4 = ((unitX.X * vector2.X) + (unitX.Y * vector2.Y)) + (unitX.Z * vector2.Z);
    if (num4 <= 0.0f)
    {
        frustum->pGjk->AddSupportPoint(&vector2);
        unitX = *frustum->pGjk->ClosestPoint();
        FLOAT num2 = maxValue;
        maxValue = Vector3::LengthSquared(&unitX);
        if ((num2 - maxValue) > (1E-05f * num2))
        {
            num3 = 4E-05f * frustum->pGjk->MaxLengthSquared();
            if (!frustum->pGjk->FullSimplex() && (maxValue >= num3))
            {
                goto Label_005A;
            }
            result = TRUE;
        }
    }

	return result;
};

BOOL BoundingFrustum::Intersects(const BoundingFrustum *frustum1, const BoundingFrustum *frustum2)
{
    Vector3 closestPoint(0);
    frustum1->pGjk->Reset();
    Vector3::Subtract(&frustum1->pCornerArray[0], &frustum2->pCornerArray[0], &closestPoint);
    if (Vector3::LengthSquared(&closestPoint) < 1E-05f)
    {
        Vector3::Subtract(&frustum1->pCornerArray[0], &frustum2->pCornerArray[1], &closestPoint);
    }
    FLOAT maxValue = FLOAT_MAXVALUE;
    FLOAT num3 = 0.0f;
    do
    {
        Vector3 vector2(0);
        Vector3 vector3(0);
        Vector3 vector4(0);
        Vector3 vector5(0);
        vector5.X = -closestPoint.X;
        vector5.Y = -closestPoint.Y;
        vector5.Z = -closestPoint.Z;
        BoundingFrustum::SupportMapping(frustum1, &vector5, &vector4);
        BoundingFrustum::SupportMapping(frustum1, &closestPoint, &vector3);
        Vector3::Subtract(&vector4, &vector3, &vector2);
        FLOAT num4 = ((closestPoint.X * vector2.X) + (closestPoint.Y * vector2.Y)) + (closestPoint.Z * vector2.Z);
        if (num4 > 0.0f)
        {
            return false;
        }
        frustum1->pGjk->AddSupportPoint(&vector2);
        closestPoint = *frustum1->pGjk->ClosestPoint();
        FLOAT num2 = maxValue;
        maxValue = Vector3::LengthSquared(&closestPoint);
        num3 = 4E-05f * frustum1->pGjk->MaxLengthSquared();
        if ((num2 - maxValue) <= (1E-05f * num2))
        {
            return false;
        }
    }
    while (!frustum1->pGjk->FullSimplex() && (maxValue >= num3));
    return true;
};

PlaneIntersectionType BoundingFrustum::Intersects(const BoundingFrustum *frustum, const Plane *plane)
{
    int num = 0;
    for (int i = 0; i < 8; i++)
    {
        FLOAT num3;
        num3 = Vector3::Dot(&frustum->pCornerArray[i], &plane->Normal);
        if ((num3 + plane->D) > 0.0f)
        {
            num |= 1;
        }
        else
        {
            num |= 2;
        }
        if (num == 3)
        {
            return PIT_Intersecting;
        }
    }
    if (num != 1)
    {
        return PIT_Back;
    }
    return PIT_Front;
};

BOOL BoundingFrustum::Intersects(const BoundingFrustum *frustum, const Ray *ray, FLOAT &result)
{
	ContainmentType type;
	type = BoundingFrustum::Contains(frustum, &ray->Position);
	if (type == CT_Contains)
	{
		result = 0.0f;
		return TRUE;
	}
	else
	{
		FLOAT minValue = FLOAT_MINVALUE;
		FLOAT maxValue = FLOAT_MAXVALUE;
		result = 0;
		for (int i = 0; i< frustum->PLANE_COUNT; i++)
		{
			FLOAT num3;
			FLOAT num6;
			Vector3 normal = frustum->pPlanesArray[i].Normal;
			num6 = Vector3::Dot(&ray->Direction, &normal);
			num3 = Vector3::Dot(&ray->Position, &normal);
			num3 += frustum->pPlanesArray[i].D;
			if (abs(num6) < 1E-05f)
			{
				if (num3 > 0.0f)
				{
					return FALSE;
				}
			}
			else
			{
				FLOAT num = -num3 / num6;
				if (num6 < 0.0f)
				{
					if (num > maxValue)
					{
						return FALSE;
					}
					if (num > minValue)
					{
						minValue = num;
					}
				}
				else
				{
					if (num < minValue)
					{
						return FALSE;
					}
					if (num < maxValue)
					{
						maxValue = num;
					}
				}
			}
		}
		FLOAT num7 = (minValue >= 0.0f) ? minValue : maxValue;
		if (num7 >= 0.0f)
		{
			result = num7;
		}
		return TRUE;
	}
};

void BoundingFrustum::SupportMapping(const BoundingFrustum *frustum, const Vector3 *v, Vector3 *result)
{
	FLOAT num3;
    int index = 0;
    num3 = Vector3::Dot(&frustum->pCornerArray[0], v);
    for (int i = 1; i < frustum->CORNER_COUNT; i++)
    {
        FLOAT num2;
        num2 = Vector3::Dot(&frustum->pCornerArray[i], v);
        if (num2 > num3)
        {
            index = i;
            num3 = num2;
        }
    }
    *result = frustum->pCornerArray[index];
};

void BoundingFrustum::Copy(const BoundingFrustum *source, BoundingFrustum *destination)
{
	for	(int i = 0; i<6; i++)
	{
		Plane::Copy(&source->pPlanesArray[i], &destination->pPlanesArray[i]);
	}

	for	(int i = 0; i<8; i++)
	{
		Vector3::Copy(&source->pCornerArray[i], &destination->pCornerArray[i]);
	}

	Matrix::Copy(source->pMatrix, destination->pMatrix);
};


BOOL BoundingFrustum::operator ==(const BoundingFrustum b) const
{
	return BoundingFrustum::Equals(this, &b);
};

BOOL BoundingFrustum::operator !=(const BoundingFrustum b) const
{
	return !BoundingFrustum::Equals(this, &b);
};
