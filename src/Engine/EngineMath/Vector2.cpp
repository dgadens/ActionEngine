#include "Vector2.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <math.h>

Vector2::Vector2()
{
	X = 0;
	Y = 0;
};

Vector2::Vector2(FLOAT value)
{
	X = value;
	Y = value;
};

Vector2::Vector2(FLOAT x, FLOAT y)
{
	X = x;
	Y = y;
};

BOOL Vector2::Equals(const Vector2 *value1, const Vector2 *value2)
{
	return ((value1->X == value2->X) && (value1->Y == value2->Y));
};

FLOAT Vector2::Length(const Vector2 *value)
{
	FLOAT num = (value->X * value->X) + (value->Y * value->Y);
	return sqrtf(num);
};

FLOAT Vector2::LengthSquared(const Vector2 *value)
{
	return (value->X * value->X) + (value->Y * value->Y);
};

FLOAT Vector2::Distance(const Vector2 *value1, const Vector2 *value2)
{
	FLOAT num2 = value1->X - value2->X;
	FLOAT num = value1->Y - value2->Y;
	FLOAT num3 = (num2 * num2) + (num * num);
	return sqrtf(num3);
};

FLOAT Vector2::DistanceSquared(const Vector2 *value1, const Vector2 *value2)
{
	FLOAT num2 = value1->X - value2->X;
	FLOAT num = value1->Y - value2->Y;
	return (num2 * num2) + (num * num);
};

FLOAT Vector2::Dot(const Vector2 *value1, const Vector2 *value2)
{
	return ((value1->X * value2->X) + (value1->Y * value2->Y));
};

void Vector2::Normalize(const Vector2 *value, Vector2 *result)
{
	FLOAT num2 = (value->X * value->X) + (value->Y * value->Y);
	FLOAT num = 1.0f / sqrtf(num2);
	result->X = value->X * num;
	result->Y = value->Y * num;
};

void Vector2::Reflect(const Vector2 *vector, const Vector2 *normal, Vector2 *result)
{
	FLOAT num = (vector->X * normal->X) + (vector->Y * normal->Y);
	result->X = vector->X - ((2.0f * num) * normal->X);
	result->Y = vector->Y - ((2.0f * num) * normal->Y);
};

void Vector2::Min(const Vector2 *value1, const Vector2 *value2, Vector2 *result)
{
	result->X = (value1->X < value2->X) ? value1->X : value2->X;
	result->Y = (value1->Y < value2->Y) ? value1->Y : value2->Y;
};

void Vector2::Max(const Vector2 *value1, const Vector2 *value2, Vector2 *result)
{
	result->X = (value1->X > value2->X) ? value1->X : value2->X;
	result->Y = (value1->Y > value2->Y) ? value1->Y : value2->Y;
};

void Vector2::Clamp(const Vector2 *value1, const Vector2 *min, const Vector2 *max, Vector2 *result)
{
	FLOAT x = value1->X;
	x = (x > max->X) ? max->X : x;
	x = (x < min->X) ? min->X : x;
	FLOAT y = value1->Y;
	y = (y > max->Y) ? max->Y : y;
	y = (y < min->Y) ? min->Y : y;
	result->X = x;
	result->Y = y;
};

void Vector2::Lerp(const Vector2 *value1, const Vector2 *value2, FLOAT amount, Vector2 *result)
{
	result->X = value1->X + ((value2->X - value1->X) * amount);
	result->Y = value1->Y + ((value2->Y - value1->Y) * amount);
};

void Vector2::Barycentric(const Vector2 *value1, const Vector2 *value2, const Vector2 *value3, FLOAT amount1, FLOAT amount2, Vector2 *result)
{
	result->X = (value1->X + (amount1 * (value2->X - value1->X))) + (amount2 * (value3->X - value1->X));
	result->Y = (value1->Y + (amount1 * (value2->Y - value1->Y))) + (amount2 * (value3->Y - value1->Y));
};

void Vector2::SmoothStep(const Vector2 *value1, const Vector2 *value2, FLOAT amount, Vector2 *result)
{
	amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
	amount = (amount * amount) * (3.0f - (2.0f * amount));
	result->X = value1->X + ((value2->X - value1->X) * amount);
	result->Y = value1->Y + ((value2->Y - value1->Y) * amount);
};

void Vector2::CatmullRom(const Vector2 *value1, const Vector2 *value2, const Vector2 *value3, const Vector2 *value4, FLOAT amount, Vector2 *result)
{
	FLOAT num = amount * amount;
	FLOAT num2 = amount * num;
	result->X = 0.5f * ((((2.0f * value2->X) + ((-value1->X + value3->X) * amount)) + (((((2.0f * value1->X) - (5.0f * value2->X)) + (4.0f * value3->X)) - value4->X) * num)) + ((((-value1->X + (3.0f * value2->X)) - (3.0f * value3->X)) + value4->X) * num2));
	result->Y = 0.5f * ((((2.0f * value2->Y) + ((-value1->Y + value3->Y) * amount)) + (((((2.0f * value1->Y) - (5.0f * value2->Y)) + (4.0f * value3->Y)) - value4->Y) * num)) + ((((-value1->Y + (3.0f * value2->Y)) - (3.0f * value3->Y)) + value4->Y) * num2));
};

void Vector2::Hermite(const Vector2 *value1, const Vector2 *tangent1, const Vector2 *value2, const Vector2 *tangent2, FLOAT amount, Vector2 *result)
{
	FLOAT num = amount * amount;
	FLOAT num2 = amount * num;
	FLOAT num6 = ((2.0f * num2) - (3.0f * num)) + 1.0f;
	FLOAT num5 = (-2.0f * num2) + (3.0f * num);
	FLOAT num4 = (num2 - (2.0f * num)) + amount;
	FLOAT num3 = num2 - num;
	result->X = (((value1->X * num6) + (value2->X * num5)) + (tangent1->X * num4)) + (tangent2->X * num3);
	result->Y = (((value1->Y * num6) + (value2->Y * num5)) + (tangent1->Y * num4)) + (tangent2->Y * num3);
};

void Vector2::Transform(const Vector2 *position, const Matrix *matrix, Vector2 *result)
{
	FLOAT num2 = ((position->X * matrix->M11) + (position->Y * matrix->M21)) + matrix->M41;
	FLOAT num = ((position->X * matrix->M12) + (position->Y * matrix->M22)) + matrix->M42;
	result->X = num2;
	result->Y = num;
};

void Vector2::TransformNormal(const Vector2 *normal, const Matrix *matrix, Vector2 *result)
{
	FLOAT num2 = (normal->X * matrix->M11) + (normal->Y * matrix->M21);
	FLOAT num = (normal->X * matrix->M12) + (normal->Y * matrix->M22);
	result->X = num2;
	result->Y = num;
};

void Vector2::Transform(const Vector2 *value, const Quaternion *rotation, Vector2 *result)
{
	FLOAT num10 = rotation->X + rotation->X;
	FLOAT num5 = rotation->Y + rotation->Y;
	FLOAT num4 = rotation->Z + rotation->Z;
	FLOAT num3 = rotation->W * num4;
	FLOAT num9 = rotation->X * num10;
	FLOAT num2 = rotation->X * num5;
	FLOAT num8 = rotation->Y * num5;
	FLOAT num = rotation->Z * num4;
	FLOAT num7 = (value->X * ((1.0f - num8) - num)) + (value->Y * (num2 - num3));
	FLOAT num6 = (value->X * (num2 + num3)) + (value->Y * ((1.0f - num9) - num));
	result->X = num7;
	result->Y = num6;
};

void Vector2::Transform(INT sourceArraySize, Vector2 sourceArray[], const Matrix *matrix, Vector2 destinationArray[])
{
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		destinationArray[i].X = ((x * matrix->M11) + (y * matrix->M21)) + matrix->M41;
		destinationArray[i].Y = ((x * matrix->M12) + (y * matrix->M22)) + matrix->M42;
	}
}

void Vector2::Transform(Vector2 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector2 destinationArray[], INT destinationIndex, INT length)
{
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		destinationArray[destinationIndex].X = ((x * matrix->M11) + (y * matrix->M21)) + matrix->M41;
		destinationArray[destinationIndex].Y = ((x * matrix->M12) + (y * matrix->M22)) + matrix->M42;
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector2::TransformNormal(INT sourceArraySize, Vector2 sourceArray[], const Matrix *matrix, Vector2 destinationArray[])
{
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		destinationArray[i].X = (x * matrix->M11) + (y * matrix->M21);
		destinationArray[i].Y = (x * matrix->M12) + (y * matrix->M22);
	}
};

void Vector2::TransformNormal(Vector2 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector2 destinationArray[], INT destinationIndex, INT length)
{
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		destinationArray[destinationIndex].X = (x * matrix->M11) + (y * matrix->M21);
		destinationArray[destinationIndex].Y = (x * matrix->M12) + (y * matrix->M22);
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector2::Transform(INT sourceArraySize, Vector2 sourceArray[], const Quaternion *rotation, Vector2 destinationArray[])
{
	FLOAT num15 = rotation->X + rotation->X;
	FLOAT num8 = rotation->Y + rotation->Y;
	FLOAT num7 = rotation->Z + rotation->Z;
	FLOAT num6 = rotation->W * num7;
	FLOAT num14 = rotation->X * num15;
	FLOAT num5 = rotation->X * num8;
	FLOAT num13 = rotation->Y * num8;
	FLOAT num4 = rotation->Z * num7;
	FLOAT num12 = (1.0f - num13) - num4;
	FLOAT num11 = num5 - num6;
	FLOAT num10 = num5 + num6;
	FLOAT num9 = (1.0f - num14) - num4;
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		destinationArray[i].X = (x * num12) + (y * num11);
		destinationArray[i].Y = (x * num10) + (y * num9);
	}
};

void Vector2::Transform(Vector2 sourceArray[], INT sourceIndex, const Quaternion *rotation, Vector2 destinationArray[], INT destinationIndex, INT length)
{
	FLOAT num14 = rotation->X + rotation->X;
	FLOAT num7 = rotation->Y + rotation->Y;
	FLOAT num6 = rotation->Z + rotation->Z;
	FLOAT num5 = rotation->W * num6;
	FLOAT num13 = rotation->X * num14;
	FLOAT num4 = rotation->X * num7;
	FLOAT num12 = rotation->Y * num7;
	FLOAT num3 = rotation->Z * num6;
	FLOAT num11 = (1.0f - num12) - num3;
	FLOAT num10 = num4 - num5;
	FLOAT num9 = num4 + num5;
	FLOAT num8 = (1.0f - num13) - num3;
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		destinationArray[destinationIndex].X = (x * num11) + (y * num10);
		destinationArray[destinationIndex].Y = (x * num9) + (y * num8);
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector2::Negate(const Vector2 *value, Vector2 *result)
{
	result->X = -value->X;
	result->Y = -value->Y;
};

void Vector2::Add(const Vector2 *value1, const Vector2 *value2, Vector2 *result)
{
	result->X = value1->X + value2->X;
	result->Y = value1->Y + value2->Y;
};

void Vector2::Subtract(const Vector2 *value1, const Vector2 *value2, Vector2 *result)
{
	result->X = value1->X - value2->X;
	result->Y = value1->Y - value2->Y;
};

void Vector2::Multiply(const Vector2 *value1, const Vector2 *value2, Vector2 *result)
{
	result->X = value1->X * value2->X;
	result->Y = value1->Y * value2->Y;
};

void Vector2::Multiply(const Vector2 *value1, FLOAT scaleFactor, Vector2 *result)
{
	result->X = value1->X * scaleFactor;
	result->Y = value1->Y * scaleFactor;
};

void Vector2::Divide(const Vector2 *value1, const Vector2 *value2, Vector2 *result)
{
	result->X = value1->X / value2->X;
	result->Y = value1->Y / value2->Y;
};

void Vector2::Divide(const Vector2 *value1, FLOAT divider, Vector2 *result)
{
	FLOAT num = 1.0f / divider;
	result->X = value1->X * num;
	result->Y = value1->Y * num;
};

void Vector2::Copy(const Vector2 *source, Vector2 *destination)
{
	destination->X = source->X;
	destination->Y = source->Y;
};


Vector2 Vector2::operator-() const
{
	Vector2 vector;
	vector.X = -this->X;
	vector.Y = -this->Y;
	return vector;
};

BOOL Vector2::operator==(const Vector2 value2) const
{
	return (this->X == value2.X) && (this->Y == value2.Y);
};

BOOL Vector2::operator!=(const Vector2 value2) const
{
	if (this->X == value2.X)
    {
        return (this->Y != value2.Y);
    }
    return true;
};

Vector2 Vector2::operator+(const Vector2 value2) const
{
	Vector2 vector;
    vector.X = this->X + value2.X;
    vector.Y = this->Y + value2.Y;
    return vector;
};

Vector2 Vector2::operator-(const Vector2 value2) const
{
	Vector2 vector;
    vector.X = this->X - value2.X;
    vector.Y = this->Y - value2.Y;
    return vector;
};

Vector2 Vector2::operator*(const Vector2 value2) const
{
	Vector2 vector;
    vector.X = this->X * value2.X;
    vector.Y = this->Y * value2.Y;
    return vector;
};

Vector2 Vector2::operator*(float scaleFactor) const
{
	Vector2 vector;
    vector.X = this->X * scaleFactor;
    vector.Y = this->Y * scaleFactor;
    return vector;
};

Vector2 Vector2::operator/(const Vector2 value2) const
{
    Vector2 vector;
    vector.X = this->X / value2.X;
    vector.Y = this->Y / value2.Y;
    return vector;
};

Vector2 Vector2::operator/(float divider) const
{
    Vector2 vector;
    float num = 1.0f / divider;
    vector.X = this->X * num;
    vector.Y = this->Y * num;
    return vector;
};
