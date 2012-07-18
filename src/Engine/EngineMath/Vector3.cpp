#include "Vector3.h"
#include "Vector2.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <math.h>

Vector3::Vector3()
{
	X = 0;
	Y = 0;
	Z = 0;
};

Vector3::Vector3(FLOAT value)
{
	X = value;
	Y = value;
	Z = value;
};

Vector3::Vector3(FLOAT x, FLOAT y, FLOAT z)
{
	X = x;
	Y = y;
	Z = z;
};

Vector3::Vector3(const Vector2 *value, FLOAT z)
{
	X = value->X;
	Y = value->Y;
	Z = z;
};

BOOL Vector3::Equals(const Vector3 *value1, const Vector3 *value2)
{
	return (((value1->X == value2->X) && (value1->Y == value2->Y)) && (value1->Z == value2->Z));
};

FLOAT Vector3::Length(const Vector3 *value)
{
	FLOAT num = ((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z);
	return sqrt(num);
};

FLOAT Vector3::LengthSquared(const Vector3 *value)
{
	return (((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z));
};

FLOAT Vector3::Distance(const Vector3 *value1, const Vector3 *value2)
{
	FLOAT num3 = value1->X - value2->X;
	FLOAT num2 = value1->Y - value2->Y;
	FLOAT num = value1->Z - value2->Z;
	FLOAT num4 = ((num3 * num3) + (num2 * num2)) + (num * num);
	return sqrt(num4);
};

FLOAT Vector3::DistanceSquared(const Vector3 *value1, const Vector3 *value2)
{
	FLOAT num3 = value1->X - value2->X;
	FLOAT num2 = value1->Y - value2->Y;
	FLOAT num = value1->Z - value2->Z;
	return (((num3 * num3) + (num2 * num2)) + (num * num));
};

FLOAT Vector3::Dot(const Vector3 *value1, const Vector3 *value2)
{
	return (((value1->X * value2->X) + (value1->Y * value2->Y)) + (value1->Z * value2->Z));
};

void Vector3::Normalize(const Vector3 *value, Vector3 *result)
{
	FLOAT num2 = ((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z);
	if (num2 == 0)
	{
		result->X = 0;
		result->Y = 0;
		result->Z = 0;
	}
	else
	{
		FLOAT num = 1.0f / (sqrt(num2));
		result->X = value->X * num;
		result->Y = value->Y * num;
		result->Z = value->Z * num;
	}
};

void Vector3::Cross(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	FLOAT num3 = (value1->Y * value2->Z) - (value1->Z * value2->Y);
	FLOAT num2 = (value1->Z * value2->X) - (value1->X * value2->Z);
	FLOAT num = (value1->X * value2->Y) - (value1->Y * value2->X);
	result->X = num3;
	result->Y = num2;
	result->Z = num;
};

void Vector3::Reflect(const Vector3 *value, const Vector3 *normal, Vector3 *result)
{
	FLOAT num = ((value->X * normal->X) + (value->Y * normal->Y)) + (value->Z * normal->Z);
	result->X = value->X - ((2.0f * num) * normal->X);
	result->Y = value->Y - ((2.0f * num) * normal->Y);
	result->Z = value->Z - ((2.0f * num) * normal->Z);
};

void Vector3::Min(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	result->X = (value1->X < value2->X) ? value1->X : value2->X;
	result->Y = (value1->Y < value2->Y) ? value1->Y : value2->Y;
	result->Z = (value1->Z < value2->Z) ? value1->Z : value2->Z;
};

void Vector3::Max(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	result->X = (value1->X > value2->X) ? value1->X : value2->X;
	result->Y = (value1->Y > value2->Y) ? value1->Y : value2->Y;
	result->Z = (value1->Z > value2->Z) ? value1->Z : value2->Z;
};

void Vector3::Clamp(const Vector3 *value1, const Vector3 *min, const Vector3 *max, Vector3 *result)
{
	FLOAT x = value1->X;
	x = (x > max->X) ? max->X : x;
	x = (x < min->X) ? min->X : x;
	FLOAT y = value1->Y;
	y = (y > max->Y) ? max->Y : y;
	y = (y < min->Y) ? min->Y : y;
	FLOAT z = value1->Z;
	z = (z > max->Z) ? max->Z : z;
	z = (z < min->Z) ? min->Z : z;
	result->X = x;
	result->Y = y;
	result->Z = z;
};

void Vector3::Lerp(const Vector3 *value1, const Vector3 *value2, FLOAT amount, Vector3 *result)
{
	result->X = value1->X + ((value2->X - value1->X) * amount);
	result->Y = value1->Y + ((value2->Y - value1->Y) * amount);
	result->Z = value1->Z + ((value2->Z - value1->Z) * amount);
};

void Vector3::Barycentric(const Vector3 *value1, const Vector3 *value2, const Vector3 *value3, FLOAT amount1, FLOAT amount2, Vector3 *result)
{
	result->X = (value1->X + (amount1 * (value2->X - value1->X))) + (amount2 * (value3->X - value1->X));
	result->Y = (value1->Y + (amount1 * (value2->Y - value1->Y))) + (amount2 * (value3->Y - value1->Y));
	result->Z = (value1->Z + (amount1 * (value2->Z - value1->Z))) + (amount2 * (value3->Z - value1->Z));
};

void Vector3::SmoothStep(const Vector3 *value1, const Vector3 *value2, FLOAT amount, Vector3 *result)
{
	amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
	amount = (amount * amount) * (3.0f - (2.0f * amount));
	result->X = value1->X + ((value2->X - value1->X) * amount);
	result->Y = value1->Y + ((value2->Y - value1->Y) * amount);
	result->Z = value1->Z + ((value2->Z - value1->Z) * amount);
};

void Vector3::CatmullRom(const Vector3 *value1, const Vector3 *value2, const Vector3 *value3, const Vector3 *value4, FLOAT amount, Vector3 *result)
{
	FLOAT num = amount * amount;
	FLOAT num2 = amount * num;
	result->X = 0.5f * ((((2.0f * value2->X) + ((-value1->X + value3->X) * amount)) + (((((2.0f * value1->X) - (5.0f * value2->X)) + (4.0f * value3->X)) - value4->X) * num)) + ((((-value1->X + (3.0f * value2->X)) - (3.0f * value3->X)) + value4->X) * num2));
	result->Y = 0.5f * ((((2.0f * value2->Y) + ((-value1->Y + value3->Y) * amount)) + (((((2.0f * value1->Y) - (5.0f * value2->Y)) + (4.0f * value3->Y)) - value4->Y) * num)) + ((((-value1->Y + (3.0f * value2->Y)) - (3.0f * value3->Y)) + value4->Y) * num2));
	result->Z = 0.5f * ((((2.0f * value2->Z) + ((-value1->Z + value3->Z) * amount)) + (((((2.0f * value1->Z) - (5.0f * value2->Z)) + (4.0f * value3->Z)) - value4->Z) * num)) + ((((-value1->Z + (3.0f * value2->Z)) - (3.0f * value3->Z)) + value4->Z) * num2));
};

void Vector3::Hermite(const Vector3 *value1, const Vector3 *tangent1, const Vector3 *value2, const Vector3 *tangent2, FLOAT amount, Vector3 *result)
{
	FLOAT num = amount * amount;
	FLOAT num2 = amount * num;
	FLOAT num6 = ((2.0f * num2) - (3.0f * num)) + 1.0f;
	FLOAT num5 = (-2.0f * num2) + (3.0f * num);
	FLOAT num4 = (num2 - (2.0f * num)) + amount;
	FLOAT num3 = num2 - num;
	result->X = (((value1->X * num6) + (value2->X * num5)) + (tangent1->X * num4)) + (tangent2->X * num3);
	result->Y = (((value1->Y * num6) + (value2->Y * num5)) + (tangent1->Y * num4)) + (tangent2->Y * num3);
	result->Z = (((value1->Z * num6) + (value2->Z * num5)) + (tangent1->Z * num4)) + (tangent2->Z * num3);
};

void Vector3::Transform(const Vector3 *position, const Matrix *matrix, Vector3 *result)
{
	FLOAT num3 = (((position->X * matrix->M11) + (position->Y * matrix->M21)) + (position->Z * matrix->M31)) + matrix->M41;
	FLOAT num2 = (((position->X * matrix->M12) + (position->Y * matrix->M22)) + (position->Z * matrix->M32)) + matrix->M42;
	FLOAT num = (((position->X * matrix->M13) + (position->Y * matrix->M23)) + (position->Z * matrix->M33)) + matrix->M43;
	result->X = num3;
	result->Y = num2;
	result->Z = num;
};

void Vector3::TransformNormal(const Vector3 *normal, const Matrix *matrix, Vector3 *result)
{
	FLOAT num3 = ((normal->X * matrix->M11) + (normal->Y * matrix->M21)) + (normal->Z * matrix->M31);
	FLOAT num2 = ((normal->X * matrix->M12) + (normal->Y * matrix->M22)) + (normal->Z * matrix->M32);
	FLOAT num = ((normal->X * matrix->M13) + (normal->Y * matrix->M23)) + (normal->Z * matrix->M33);
	result->X = num3;
	result->Y = num2;
	result->Z = num;
};

void Vector3::Transform(const Vector3 *value, const Quaternion *rotation, Vector3 *result)
{
	FLOAT num12 = rotation->X + rotation->X;
	FLOAT num2 = rotation->Y + rotation->Y;
	FLOAT num = rotation->Z + rotation->Z;
	FLOAT num11 = rotation->W * num12;
	FLOAT num10 = rotation->W * num2;
	FLOAT num9 = rotation->W * num;
	FLOAT num8 = rotation->X * num12;
	FLOAT num7 = rotation->X * num2;
	FLOAT num6 = rotation->X * num;
	FLOAT num5 = rotation->Y * num2;
	FLOAT num4 = rotation->Y * num;
	FLOAT num3 = rotation->Z * num;
	FLOAT num15 = ((value->X * ((1.0f - num5) - num3)) + (value->Y * (num7 - num9))) + (value->Z * (num6 + num10));
	FLOAT num14 = ((value->X * (num7 + num9)) + (value->Y * ((1.0f - num8) - num3))) + (value->Z * (num4 - num11));
	FLOAT num13 = ((value->X * (num6 - num10)) + (value->Y * (num4 + num11))) + (value->Z * ((1.0f - num8) - num5));
	result->X = num15;
	result->Y = num14;
	result->Z = num13;
};

void Vector3::Transform(INT sourceArraySize, Vector3 sourceArray[], const Matrix *matrix, Vector3 destinationArray[])
{
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		FLOAT z = sourceArray[i].Z;
		destinationArray[i].X = (((x * matrix->M11) + (y * matrix->M21)) + (z * matrix->M31)) + matrix->M41;
		destinationArray[i].Y = (((x * matrix->M12) + (y * matrix->M22)) + (z * matrix->M32)) + matrix->M42;
		destinationArray[i].Z = (((x * matrix->M13) + (y * matrix->M23)) + (z * matrix->M33)) + matrix->M43;
	}
};

void Vector3::Transform(Vector3 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector3 destinationArray[], INT destinationIndex, INT length)
{
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		FLOAT z = sourceArray[sourceIndex].Z;
		destinationArray[destinationIndex].X = (((x * matrix->M11) + (y * matrix->M21)) + (z * matrix->M31)) + matrix->M41;
		destinationArray[destinationIndex].Y = (((x * matrix->M12) + (y * matrix->M22)) + (z * matrix->M32)) + matrix->M42;
		destinationArray[destinationIndex].Z = (((x * matrix->M13) + (y * matrix->M23)) + (z * matrix->M33)) + matrix->M43;
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector3::TransformNormal(INT sourceArraySize, Vector3 sourceArray[], const Matrix *matrix, Vector3 destinationArray[])
{
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		FLOAT z = sourceArray[i].Z;
		destinationArray[i].X = ((x * matrix->M11) + (y * matrix->M21)) + (z * matrix->M31);
		destinationArray[i].Y = ((x * matrix->M12) + (y * matrix->M22)) + (z * matrix->M32);
		destinationArray[i].Z = ((x * matrix->M13) + (y * matrix->M23)) + (z * matrix->M33);
	}
};

void Vector3::TransformNormal(Vector3 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector3 destinationArray[], INT destinationIndex, INT length)
{
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		FLOAT z = sourceArray[sourceIndex].Z;
		destinationArray[destinationIndex].X = ((x * matrix->M11) + (y * matrix->M21)) + (z * matrix->M31);
		destinationArray[destinationIndex].Y = ((x * matrix->M12) + (y * matrix->M22)) + (z * matrix->M32);
		destinationArray[destinationIndex].Z = ((x * matrix->M13) + (y * matrix->M23)) + (z * matrix->M33);
		sourceIndex++;
		destinationIndex++;
		length--;
	}

};

void Vector3::Transform(INT sourceArraySize, Vector3 sourceArray[], const Quaternion *rotation, Vector3 destinationArray[])
{
	FLOAT num16 = rotation->X + rotation->X;
	FLOAT num6 = rotation->Y + rotation->Y;
	FLOAT num2 = rotation->Z + rotation->Z;
	FLOAT num15 = rotation->W * num16;
	FLOAT num14 = rotation->W * num6;
	FLOAT num13 = rotation->W * num2;
	FLOAT num12 = rotation->X * num16;
	FLOAT num11 = rotation->X * num6;
	FLOAT num10 = rotation->X * num2;
	FLOAT num9 = rotation->Y * num6;
	FLOAT num8 = rotation->Y * num2;
	FLOAT num7 = rotation->Z * num2;
	FLOAT num25 = (1.0f - num9) - num7;
	FLOAT num24 = num11 - num13;
	FLOAT num23 = num10 + num14;
	FLOAT num22 = num11 + num13;
	FLOAT num21 = (1.0f - num12) - num7;
	FLOAT num20 = num8 - num15;
	FLOAT num19 = num10 - num14;
	FLOAT num18 = num8 + num15;
	FLOAT num17 = (1.0f - num12) - num9;
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		FLOAT z = sourceArray[i].Z;
		destinationArray[i].X = ((x * num25) + (y * num24)) + (z * num23);
		destinationArray[i].Y = ((x * num22) + (y * num21)) + (z * num20);
		destinationArray[i].Z = ((x * num19) + (y * num18)) + (z * num17);
	}
};

void Vector3::Transform(Vector3 sourceArray[], INT sourceIndex, const Quaternion *rotation, Vector3 destinationArray[], INT destinationIndex, INT length)
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
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		FLOAT z = sourceArray[sourceIndex].Z;
		destinationArray[destinationIndex].X = ((x * num24) + (y * num23)) + (z * num22);
		destinationArray[destinationIndex].Y = ((x * num21) + (y * num20)) + (z * num19);
		destinationArray[destinationIndex].Z = ((x * num18) + (y * num17)) + (z * num16);
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector3::Negate(const Vector3 *value, Vector3 *result)
{
	result->X = -value->X;
	result->Y = -value->Y;
	result->Z = -value->Z;
};

void Vector3::Add(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	result->X = value1->X + value2->X;
	result->Y = value1->Y + value2->Y;
	result->Z = value1->Z + value2->Z;
};

void Vector3::Subtract(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	result->X = value1->X - value2->X;
	result->Y = value1->Y - value2->Y;
	result->Z = value1->Z - value2->Z;
};

void Vector3::Multiply(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	result->X = value1->X * value2->X;
	result->Y = value1->Y * value2->Y;
	result->Z = value1->Z * value2->Z;
};

void Vector3::Multiply(const Vector3 *value1, FLOAT scaleFactor, Vector3 *result)
{
	result->X = value1->X * scaleFactor;
	result->Y = value1->Y * scaleFactor;
	result->Z = value1->Z * scaleFactor;
};

void Vector3::Divide(const Vector3 *value1, const Vector3 *value2, Vector3 *result)
{
	result->X = value1->X / value2->X;
	result->Y = value1->Y / value2->Y;
	result->Z = value1->Z / value2->Z;
};

void Vector3::Divide(const Vector3 *value1, FLOAT value2, Vector3 *result)
{
	FLOAT num = 1.0f / value2;
	result->X = value1->X * num;
	result->Y = value1->Y * num;
	result->Z = value1->Z * num;
};

void Vector3::Copy(const Vector3 *source, Vector3 *destination)
{
	destination->X = source->X;
	destination->Y = source->Y;
	destination->Z = source->Z;
};


Vector3 Vector3::operator-() const
{
	Vector3 vector;
	vector.X = -this->X;
	vector.Y = -this->Y;
	vector.Z = -this->Z;
	return vector;
};

BOOL Vector3::operator==(const Vector3 value2) const
{
	return (((this->X == value2.X) && (this->Y == value2.Y)) && (this->Z == value2.Z));
};

BOOL Vector3::operator!=(const Vector3 value2) const
{
	if ((this->X == value2.X) && (this->Y == value2.Y))
    {
        return (this->Z != value2.Z);
    }
    return true;
};

Vector3 Vector3::operator+(const Vector3 value2) const
{
	Vector3 vector;
    vector.X = this->X + value2.X;
    vector.Y = this->Y + value2.Y;
    vector.Z = this->Z + value2.Z;
    return vector;
};

Vector3 Vector3::operator-(const Vector3 value2) const
{
	Vector3 vector;
    vector.X = this->X - value2.X;
    vector.Y = this->Y - value2.Y;
    vector.Z = this->Z - value2.Z;
    return vector;
};

Vector3 Vector3::operator*(const Vector3 value2) const
{
	Vector3 vector;
    vector.X = this->X * value2.X;
    vector.Y = this->Y * value2.Y;
    vector.Z = this->Z * value2.Z;
    return vector;
};

Vector3 Vector3::operator*(float scaleFactor) const
{
	Vector3 vector;
    vector.X = this->X * scaleFactor;
    vector.Y = this->Y * scaleFactor;
    vector.Z = this->Z * scaleFactor;
    return vector;
};

Vector3 Vector3::operator/(const Vector3 value2) const
{
    Vector3 vector;
    vector.X = this->X / value2.X;
    vector.Y = this->Y / value2.Y;
    vector.Z = this->Z / value2.Z;
    return vector;
};

Vector3 Vector3::operator/(float divider) const
{
    Vector3 vector;
    float num = 1.0f / divider;
    vector.X = this->X * num;
    vector.Y = this->Y * num;
    vector.Z = this->Z * num;
    return vector;
};
