#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <math.h>

Vector4::Vector4()
{	
	X = 0;
	Y = 0;
	Z = 0;
	W = 0;
};

Vector4::Vector4(FLOAT value)
{
	X = value;
	Y = value;
	Z = value;
	W = value;
};

Vector4::Vector4(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
};

Vector4::Vector4(const Vector2 *value, FLOAT z, FLOAT w)
{
	X = value->X;
	Y = value->Y;
	Z = z;
	w = w;
};

Vector4::Vector4(const Vector3 *value, FLOAT w)
{
	X = value->X;
	Y = value->Y;
	Z = value->Z;
	W = w;
};

BOOL Vector4::Equals(const Vector4 *value1, const Vector4 *value2)
{
	return ((((value1->X == value2->X) && (value1->Y == value2->Y)) && (value1->Z == value2->Z)) && (value1->W == value2->W));
};

FLOAT Vector4::Length(const Vector4 *value)
{
	FLOAT num = (((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z)) + (value->W * value->W);
	return sqrt(num);
};

FLOAT Vector4::LengthSquared(const Vector4 *value)
{
	return (((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z)) + (value->W * value->W);
};

FLOAT Vector4::Distance(const Vector4 *value1, const Vector4 *value2)
{
	FLOAT num4 = value1->X - value2->X;
	FLOAT num3 = value1->Y - value2->Y;
	FLOAT num2 = value1->Z - value2->Z;
	FLOAT num = value1->W - value2->W;
	FLOAT num5 = (((num4 * num4) + (num3 * num3)) + (num2 * num2)) + (num * num);
	return sqrt(num5);
};

FLOAT Vector4::DistanceSquared(const Vector4 *value1, const Vector4 *value2)
{
	FLOAT num4 = value1->X - value2->X;
	FLOAT num3 = value1->Y - value2->Y;
	FLOAT num2 = value1->Z - value2->Z;
	FLOAT num = value1->W - value2->W;
	return (((num4 * num4) + (num3 * num3)) + (num2 * num2)) + (num * num);
};

FLOAT Vector4::Dot(const Vector4 *value1, const Vector4 *value2)
{
	return ((((value1->X * value2->X) + (value1->Y * value2->Y)) + (value1->Z * value2->Z)) + (value1->W * value2->W));
};

void Vector4::Normalize(const Vector4 *value, Vector4 *result)
{
	FLOAT num2 = (((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z)) + (value->W * value->W);
	FLOAT num = 1.0f / sqrt(num2);
	result->X = value->X * num;
	result->Y = value->Y * num;
	result->Z = value->Z * num;
	result->W = value->W * num;
};

void Vector4::Min(const Vector4 *value1, const Vector4 *value2, Vector4 *result)
{
	result->X = (value1->X < value2->X) ? value1->X : value2->X;
	result->Y = (value1->Y < value2->Y) ? value1->Y : value2->Y;
	result->Z = (value1->Z < value2->Z) ? value1->Z : value2->Z;
	result->W = (value1->W < value2->W) ? value1->W : value2->W;
};

void Vector4::Max(const Vector4 *value1, const Vector4 *value2, Vector4 *result)
{
	result->X = (value1->X > value2->X) ? value1->X : value2->X;
	result->Y = (value1->Y > value2->Y) ? value1->Y : value2->Y;
	result->Z = (value1->Z > value2->Z) ? value1->Z : value2->Z;
	result->W = (value1->W > value2->W) ? value1->W : value2->W;
};

void Vector4::Clamp(const Vector4 *value1, const Vector4 *min, const Vector4 *max, Vector4 *result)
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
	FLOAT w = value1->W;
	w = (w > max->W) ? max->W : w;
	w = (w < min->W) ? min->W : w;
	result->X = x;
	result->Y = y;
	result->Z = z;
	result->W = w;
};

void Vector4::Lerp(const Vector4 *value1, const Vector4 *value2, FLOAT amount, Vector4 *result)
{
	result->X = value1->X + ((value2->X - value1->X) * amount);
	result->Y = value1->Y + ((value2->Y - value1->Y) * amount);
	result->Z = value1->Z + ((value2->Z - value1->Z) * amount);
	result->W = value1->W + ((value2->W - value1->W) * amount);
};

void Vector4::Barycentric(const Vector4 *value1, const Vector4 *value2, const Vector4 *value3, FLOAT amount1, FLOAT amount2, Vector4 *result)
{
	result->X = (value1->X + (amount1 * (value2->X - value1->X))) + (amount2 * (value3->X - value1->X));
	result->Y = (value1->Y + (amount1 * (value2->Y - value1->Y))) + (amount2 * (value3->Y - value1->Y));
	result->Z = (value1->Z + (amount1 * (value2->Z - value1->Z))) + (amount2 * (value3->Z - value1->Z));
	result->W = (value1->W + (amount1 * (value2->W - value1->W))) + (amount2 * (value3->W - value1->W));

};

void Vector4::SmoothStep(const Vector4 *value1, const Vector4 *value2, FLOAT amount, Vector4 *result)
{
	amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
	amount = (amount * amount) * (3.0f - (2.0f * amount));
	result->X = value1->X + ((value2->X - value1->X) * amount);
	result->Y = value1->Y + ((value2->Y - value1->Y) * amount);
	result->Z = value1->Z + ((value2->Z - value1->Z) * amount);
	result->W = value1->W + ((value2->W - value1->W) * amount);
};

void Vector4::CatmullRom(const Vector4 *value1, const Vector4 *value2, const Vector4 *value3, const Vector4 *value4, FLOAT amount, Vector4 *result)
{
	FLOAT num = amount * amount;
	FLOAT num2 = amount * num;
	result->X = 0.5f * ((((2.0f * value2->X) + ((-value1->X + value3->X) * amount)) + (((((2.0f * value1->X) - (5.0f * value2->X)) + (4.0f * value3->X)) - value4->X) * num)) + ((((-value1->X + (3.0f * value2->X)) - (3.0f * value3->X)) + value4->X) * num2));
	result->Y = 0.5f * ((((2.0f * value2->Y) + ((-value1->Y + value3->Y) * amount)) + (((((2.0f * value1->Y) - (5.0f * value2->Y)) + (4.0f * value3->Y)) - value4->Y) * num)) + ((((-value1->Y + (3.0f * value2->Y)) - (3.0f * value3->Y)) + value4->Y) * num2));
	result->Z = 0.5f * ((((2.0f * value2->Z) + ((-value1->Z + value3->Z) * amount)) + (((((2.0f * value1->Z) - (5.0f * value2->Z)) + (4.0f * value3->Z)) - value4->Z) * num)) + ((((-value1->Z + (3.0f * value2->Z)) - (3.0f * value3->Z)) + value4->Z) * num2));
	result->W = 0.5f * ((((2.0f * value2->W) + ((-value1->W + value3->W) * amount)) + (((((2.0f * value1->W) - (5.0f * value2->W)) + (4.0f * value3->W)) - value4->W) * num)) + ((((-value1->W + (3.0f * value2->W)) - (3.0f * value3->W)) + value4->W) * num2));
};

void Vector4::Hermite(const Vector4 *value1, const Vector4 *tangent1, const Vector4 *value2, const Vector4 *tangent2, FLOAT amount, Vector4 *result)
{
	FLOAT num = amount * amount;
	FLOAT num6 = amount * num;
	FLOAT num5 = ((2.0f * num6) - (3.0f * num)) + 1.0f;
	FLOAT num4 = (-2.0f * num6) + (3.0f * num);
	FLOAT num3 = (num6 - (2.0f * num)) + amount;
	FLOAT num2 = num6 - num;
	result->X = (((value1->X * num5) + (value2->X * num4)) + (tangent1->X * num3)) + (tangent2->X * num2);
	result->Y = (((value1->Y * num5) + (value2->Y * num4)) + (tangent1->Y * num3)) + (tangent2->Y * num2);
	result->Z = (((value1->Z * num5) + (value2->Z * num4)) + (tangent1->Z * num3)) + (tangent2->Z * num2);
	result->W = (((value1->W * num5) + (value2->W * num4)) + (tangent1->W * num3)) + (tangent2->W * num2);

};

void Vector4::Transform(const Vector2 *position, const Matrix *matrix, Vector4 *result)
{
	FLOAT num4 = ((position->X * matrix->M11) + (position->Y * matrix->M21)) + matrix->M41;
	FLOAT num3 = ((position->X * matrix->M12) + (position->Y * matrix->M22)) + matrix->M42;
	FLOAT num2 = ((position->X * matrix->M13) + (position->Y * matrix->M23)) + matrix->M43;
	FLOAT num = ((position->X * matrix->M14) + (position->Y * matrix->M24)) + matrix->M44;
	result->X = num4;
	result->Y = num3;
	result->Z = num2;
	result->W = num;
};

void Vector4::Transform(const Vector3 *position, const Matrix *matrix, Vector4 *result)
{
	FLOAT num4 = (((position->X * matrix->M11) + (position->Y * matrix->M21)) + (position->Z * matrix->M31)) + matrix->M41;
	FLOAT num3 = (((position->X * matrix->M12) + (position->Y * matrix->M22)) + (position->Z * matrix->M32)) + matrix->M42;
	FLOAT num2 = (((position->X * matrix->M13) + (position->Y * matrix->M23)) + (position->Z * matrix->M33)) + matrix->M43;
	FLOAT num = (((position->X * matrix->M14) + (position->Y * matrix->M24)) + (position->Z * matrix->M34)) + matrix->M44;
	result->X = num4;
	result->Y = num3;
	result->Z = num2;
	result->W = num;
};

void Vector4::Transform(const Vector4 *vector, const Matrix *matrix, Vector4 *result)
{
	FLOAT num4 = (((vector->X * matrix->M11) + (vector->Y * matrix->M21)) + (vector->Z * matrix->M31)) + (vector->W * matrix->M41);
	FLOAT num3 = (((vector->X * matrix->M12) + (vector->Y * matrix->M22)) + (vector->Z * matrix->M32)) + (vector->W * matrix->M42);
	FLOAT num2 = (((vector->X * matrix->M13) + (vector->Y * matrix->M23)) + (vector->Z * matrix->M33)) + (vector->W * matrix->M43);
	FLOAT num = (((vector->X * matrix->M14) + (vector->Y * matrix->M24)) + (vector->Z * matrix->M34)) + (vector->W * matrix->M44);
	result->X = num4;
	result->Y = num3;
	result->Z = num2;
	result->W = num;
};

void Vector4::Transform(const Vector2 *value, const Quaternion *rotation, Vector4 *result)
{
	FLOAT num6 = rotation->X + rotation->X;
	FLOAT num2 = rotation->Y + rotation->Y;
	FLOAT num = rotation->Z + rotation->Z;
	FLOAT num15 = rotation->W * num6;
	FLOAT num14 = rotation->W * num2;
	FLOAT num5 = rotation->W * num;
	FLOAT num13 = rotation->X * num6;
	FLOAT num4 = rotation->X * num2;
	FLOAT num12 = rotation->X * num;
	FLOAT num11 = rotation->Y * num2;
	FLOAT num10 = rotation->Y * num;
	FLOAT num3 = rotation->Z * num;
	FLOAT num9 = (value->X * ((1.0f - num11) - num3)) + (value->Y * (num4 - num5));
	FLOAT num8 = (value->X * (num4 + num5)) + (value->Y * ((1.0f - num13) - num3));
	FLOAT num7 = (value->X * (num12 - num14)) + (value->Y * (num10 + num15));
	result->X = num9;
	result->Y = num8;
	result->Z = num7;
	result->W = 1.0f;
};

void Vector4::Transform(const Vector3 *value, const Quaternion *rotation, Vector4 *result)
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
	result->W = 1.0f;
};

void Vector4::Transform(const Vector4 *value, const Quaternion *rotation, Vector4 *result)
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
	result->W = value->W;
};

void Vector4::Transform(INT sourceArraySize, Vector4 sourceArray[], const Matrix *matrix, Vector4 destinationArray[])
{
	for (INT i = 0; i < sourceArraySize; i++)
	{
		FLOAT x = sourceArray[i].X;
		FLOAT y = sourceArray[i].Y;
		FLOAT z = sourceArray[i].Z;
		FLOAT w = sourceArray[i].W;
		destinationArray[i].X = (((x * matrix->M11) + (y * matrix->M21)) + (z * matrix->M31)) + (w * matrix->M41);
		destinationArray[i].Y = (((x * matrix->M12) + (y * matrix->M22)) + (z * matrix->M32)) + (w * matrix->M42);
		destinationArray[i].Z = (((x * matrix->M13) + (y * matrix->M23)) + (z * matrix->M33)) + (w * matrix->M43);
		destinationArray[i].W = (((x * matrix->M14) + (y * matrix->M24)) + (z * matrix->M34)) + (w * matrix->M44);
	}
};

void Vector4::Transform(Vector4 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector4 destinationArray[], INT destinationIndex, INT length)
{
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		FLOAT z = sourceArray[sourceIndex].Z;
		FLOAT w = sourceArray[sourceIndex].W;
		destinationArray[destinationIndex].X = (((x * matrix->M11) + (y * matrix->M21)) + (z * matrix->M31)) + (w * matrix->M41);
		destinationArray[destinationIndex].Y = (((x * matrix->M12) + (y * matrix->M22)) + (z * matrix->M32)) + (w * matrix->M42);
		destinationArray[destinationIndex].Z = (((x * matrix->M13) + (y * matrix->M23)) + (z * matrix->M33)) + (w * matrix->M43);
		destinationArray[destinationIndex].W = (((x * matrix->M14) + (y * matrix->M24)) + (z * matrix->M34)) + (w * matrix->M44);
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector4::Transform(INT sourceArraySize, Vector4 sourceArray[], const Quaternion *rotation, Vector4 destinationArray[])
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
		destinationArray[i].W = sourceArray[i].W;
	}
};

void Vector4::Transform(Vector4 sourceArray[], INT sourceIndex, const Quaternion *rotation, Vector4 destinationArray[], INT destinationIndex, INT length)
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
	FLOAT num25 = (1.0f - num8) - num6;
	FLOAT num24 = num10 - num12;
	FLOAT num23 = num9 + num13;
	FLOAT num22 = num10 + num12;
	FLOAT num21 = (1.0f - num11) - num6;
	FLOAT num20 = num7 - num14;
	FLOAT num19 = num9 - num13;
	FLOAT num18 = num7 + num14;
	FLOAT num17 = (1.0f - num11) - num8;
	while (length > 0)
	{
		FLOAT x = sourceArray[sourceIndex].X;
		FLOAT y = sourceArray[sourceIndex].Y;
		FLOAT z = sourceArray[sourceIndex].Z;
		FLOAT w = sourceArray[sourceIndex].W;
		destinationArray[destinationIndex].X = ((x * num25) + (y * num24)) + (z * num23);
		destinationArray[destinationIndex].Y = ((x * num22) + (y * num21)) + (z * num20);
		destinationArray[destinationIndex].Z = ((x * num19) + (y * num18)) + (z * num17);
		destinationArray[destinationIndex].W = w;
		sourceIndex++;
		destinationIndex++;
		length--;
	}
};

void Vector4::Negate(const Vector4 *value, Vector4 *result)
{
	result->X = -value->X;
	result->Y = -value->Y;
	result->Z = -value->Z;
	result->W = -value->W;
};

void Vector4::Add(const Vector4 *value1, const Vector4 *value2, Vector4 *result)
{
	result->X = value1->X + value2->X;
	result->Y = value1->Y + value2->Y;
	result->Z = value1->Z + value2->Z;
	result->W = value1->W + value2->W;
};

void Vector4::Subtract(const Vector4 *value1, const Vector4 *value2, Vector4 *result)
{
	result->X = value1->X - value2->X;
	result->Y = value1->Y - value2->Y;
	result->Z = value1->Z - value2->Z;
	result->W = value1->W - value2->W;
};

void Vector4::Multiply(const Vector4 *value1, const Vector4 *value2, Vector4 *result)
{
	result->X = value1->X * value2->X;
	result->Y = value1->Y * value2->Y;
	result->Z = value1->Z * value2->Z;
	result->W = value1->W * value2->W;
};

void Vector4::Multiply(const Vector4 *value1, FLOAT scaleFactor, Vector4 *result)
{
	result->X = value1->X * scaleFactor;
	result->Y = value1->Y * scaleFactor;
	result->Z = value1->Z * scaleFactor;
	result->W = value1->W * scaleFactor;
};

void Vector4::Divide(const Vector4 *value1, const Vector4 *value2, Vector4 *result)
{
	result->X = value1->X / value2->X;
	result->Y = value1->Y / value2->Y;
	result->Z = value1->Z / value2->Z;
	result->W = value1->W / value2->W;
};

void Vector4::Divide(const Vector4 *value1, FLOAT value2, Vector4 *result)
{
	FLOAT num = 1.0f / value2;
	result->X = value1->X * num;
	result->Y = value1->Y * num;
	result->Z = value1->Z * num;
	result->W = value1->W * num;
};

void Vector4::Copy(const Vector4 *source, Vector4 *destination)
{
	destination->X = source->X;
	destination->Y = source->Y;
	destination->Z = source->Z;
	destination->W = source->W;
};

//Vector4 Vector4::operator=(const Vector4 value) const
//{
//	Vector4 vector;
//	vector.X = value.X;
//	vector.Y = value.Y;
//	vector.Z = value.Z;
//	vector.W = value.W;
//	return vector;
//};

Vector4 Vector4::operator-() const
{
	Vector4 vector;
	vector.X = -this->X;
	vector.Y = -this->Y;
	vector.Z = -this->Z;
	vector.W = -this->W;
	return vector;
};

BOOL Vector4::operator==(const Vector4 value2) const
{
	return ((((this->X == value2.X) && (this->Y == value2.Y)) && (this->Z == value2.Z)) && (this->W == value2.W));
};

BOOL Vector4::operator!=(const Vector4 value2) const
{
	if (((this->X == value2.X) && (this->Y == value2.Y)) && (this->Z == value2.Z))
    {
        return (this->W != value2.W);
    }
    return true;
};

Vector4 Vector4::operator+(const Vector4 value2) const
{
	Vector4 vector;
    vector.X = this->X + value2.X;
    vector.Y = this->Y + value2.Y;
    vector.Z = this->Z + value2.Z;
    vector.W = this->W + value2.W;
    return vector;
};

Vector4 Vector4::operator-(const Vector4 value2) const
{
	Vector4 vector;
    vector.X = this->X - value2.X;
    vector.Y = this->Y - value2.Y;
    vector.Z = this->Z - value2.Z;
    vector.W = this->W - value2.W;
    return vector;
};

Vector4 Vector4::operator*(const Vector4 value2) const
{
	Vector4 vector;
    vector.X = this->X * value2.X;
    vector.Y = this->Y * value2.Y;
    vector.Z = this->Z * value2.Z;
    vector.W = this->W * value2.W;
    return vector;
};

Vector4 Vector4::operator*(float scaleFactor) const
{
	Vector4 vector;
    vector.X = this->X * scaleFactor;
    vector.Y = this->Y * scaleFactor;
    vector.Z = this->Z * scaleFactor;
    vector.W = this->W * scaleFactor;
    return vector;
};

Vector4 Vector4::operator/(const Vector4 value2) const
{
    Vector4 vector;
    vector.X = this->X / value2.X;
    vector.Y = this->Y / value2.Y;
    vector.Z = this->Z / value2.Z;
    vector.W = this->W / value2.W;
    return vector;
};

Vector4 Vector4::operator/(float divider) const
{
    Vector4 vector;
    float num = 1.0f / divider;
    vector.X = this->X * num;
    vector.Y = this->Y * num;
    vector.Z = this->Z * num;
    vector.W = this->W * num;
    return vector;
};
