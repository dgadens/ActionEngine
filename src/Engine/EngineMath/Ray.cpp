#include "Ray.h"
#include "Vector2.h"
#include "Matrix.h"
#include "BoundingBox.h"
#include "BoundingFrustum.h"
#include "BoundingSphere.h"
#include "Plane.h"
#include <math.h>

Ray::Ray()
{
	Position = Vector3();
	Direction = Vector3();
};

Ray::Ray(const Vector3 *position, const Vector3 *direction)
{
	Position = Vector3(Position.X, Position.Y, Position.Z);
	Direction = Vector3(Direction.X, Direction.Y, Direction.Z);
};

Ray::~Ray()
{
};

BOOL Ray::Equals(const Ray *value1, const Ray *value2)
{
	return Vector3::Equals(&value1->Position, &value2->Position) && Vector3::Equals(&value1->Direction, &value2->Direction);
};

BOOL Ray::Intersects(const Ray *ray, const BoundingBox *box, FLOAT &result)
{
	return BoundingBox::Intersects(box, ray, result);
};

BOOL Ray::Intersects(const Ray *ray, const BoundingFrustum *frustum, FLOAT &result)
{
	return BoundingFrustum::Intersects(frustum, ray, result);
};

BOOL Ray::Intersects(const Ray *ray, const Plane *plane, FLOAT &result)
{
	FLOAT num2 = ((plane->Normal.X * ray->Direction.X) + (plane->Normal.Y * ray->Direction.Y)) + (plane->Normal.Z * ray->Direction.Z);
	if (abs(num2) < 1E-05f)
	{
		result = 0;
		return FALSE;
	}
	else
	{
		FLOAT num3 = ((plane->Normal.X * ray->Position.X) + (plane->Normal.Y * ray->Position.Y)) + (plane->Normal.Z * ray->Position.Z);
		FLOAT num = (-plane->D - num3) / num2;
		if (num < 0.0f)
		{
			if (num < -1E-05f)
			{
				result = 0;
				return FALSE;
			}
		}

		result = num;
		return TRUE;
	}
};

BOOL Ray::Intersects(const Ray *ray, const BoundingSphere *sphere, FLOAT &result)
{
	FLOAT num5 = sphere->Center.X - ray->Position.X;
	FLOAT num4 = sphere->Center.Y - ray->Position.Y;
	FLOAT num3 = sphere->Center.Z - ray->Position.Z;
	FLOAT num7 = ((num5 * num5) + (num4 * num4)) + (num3 * num3);
	FLOAT num2 = sphere->Radius * sphere->Radius;
	if (num7 <= num2)
	{
		result = 0.0f;
		return FALSE;
	}
	else
	{
		result = 0;
		FLOAT num = ((num5 * ray->Direction.X) + (num4 * ray->Direction.Y)) + (num3 * ray->Direction.Z);
		if (num >= 0.0f)
		{
			FLOAT num6 = num7 - (num * num);
			if (num6 <= num2)
			{
				FLOAT num8 = sqrt(num2 - num6);
				result = num - num8;
				return TRUE;
			}
		}

		return FALSE;
	}

};

void Ray::GetPickRay(const Vector2 *mousePosition, const Matrix *projection, const Matrix *view, INT viewPortWidth, INT viewPortHeight, FLOAT nearPlane, FLOAT farPlane, Ray *result)
{
	INT mouseX = (INT)mousePosition->X;
	INT mouseY = (INT)mousePosition->Y;

	Vector3 nearsource((FLOAT)mouseX, (FLOAT)mouseY, 0.0f);
	Vector3 farsource((FLOAT)mouseX, (FLOAT)mouseY, 1.0f);

	Matrix world;
	Matrix::CreateTranslation(0, 0, 0, &world);

	Matrix::Unproject(&nearsource, projection, view, &world, viewPortWidth, viewPortHeight, nearPlane, farPlane, &result->Position);
	Vector3 farPoint;
	Matrix::Unproject(&farsource, projection, view, &world, viewPortWidth, viewPortHeight, nearPlane, farPlane, &farPoint);

	// Create a ray from the near clip plane to the far clip plane.
	Vector3::Subtract(&farPoint, &result->Position, &result->Direction);
	Vector3::Normalize(&result->Direction, &result->Direction);
};	

void Ray::Copy(const Ray *source, Ray *destination)
{
	destination->Position = source->Position;
	destination->Direction = source->Direction;
};


BOOL Ray::operator ==(const Ray b) const
{
	return (this->Position.X == b.Position.X && this->Position.Y == b.Position.Y && this->Position.Z == b.Position.Z) && (this->Direction.X == b.Direction.X && this->Direction.Y == b.Direction.Y && this->Direction.Z == b.Direction.Z);
};

BOOL Ray::operator !=(const Ray b) const
{
	return (this->Position.X != b.Position.X || this->Position.Y != b.Position.Y || this->Position.Z != b.Position.Z) || (this->Direction.X != b.Direction.X || this->Direction.Y != b.Direction.Y || this->Direction.Z != b.Direction.Z);
};
