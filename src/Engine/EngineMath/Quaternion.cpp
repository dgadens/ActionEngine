#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix.h"
#include <math.h>

Quaternion::Quaternion()
{
	X = 0;
	Y = 0;
	Z = 0;
	W = 1;
};

Quaternion::Quaternion(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
};

Quaternion::Quaternion(const Vector3 *vectorPart, FLOAT scalarPart)
{
	X = vectorPart->X;
	Y = vectorPart->Y;
	Z = vectorPart->Z;
	W = scalarPart;
};

BOOL Quaternion::Equals(const Quaternion *value1, const Quaternion *value2)
{
	return ((((value1->X == value2->X) && (value1->Y == value2->Y)) && (value1->Z == value2->Z)) && (value1->W == value2->W));
};

FLOAT Quaternion::LengthSquared(const Quaternion *value)
{
	return ((((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z)) + (value->W * value->W));
};

FLOAT Quaternion::Length(const Quaternion *value)
{
	FLOAT num = ((((value->X * value->X) + (value->Y * value->Y)) + (value->Z * value->Z)) + (value->W * value->W));
	return sqrtf(num);
};

void Quaternion::Normalize(const Quaternion *quaternion, Quaternion *result)
{
	FLOAT num2 = (((quaternion->X * quaternion->X) + (quaternion->Y * quaternion->Y)) + (quaternion->Z * quaternion->Z)) + (quaternion->W * quaternion->W);
	FLOAT num = 1.0f / sqrtf(num2);
	result->X = quaternion->X * num;
	result->Y = quaternion->Y * num;
	result->Z = quaternion->Z * num;
	result->W = quaternion->W * num;
};

void Quaternion::Conjugate(const Quaternion *value, Quaternion *result)
{
	result->X = -value->X;
	result->Y = -value->Y;
	result->Z = -value->Z;
	result->W = value->W;
};

void Quaternion::Inverse(const Quaternion *quaternion, Quaternion *result)
{
	FLOAT num2 = (((quaternion->X * quaternion->X) + (quaternion->Y * quaternion->Y)) + (quaternion->Z * quaternion->Z)) + (quaternion->W * quaternion->W);
	FLOAT num = 1.0f / num2;
	result->X = -quaternion->X * num;
	result->Y = -quaternion->Y * num;
	result->Z = -quaternion->Z * num;
	result->W = quaternion->W * num;
};

void Quaternion::CreateFromAxisAngle(const Vector3 *axis, FLOAT angle, Quaternion *result)
{
	FLOAT num2 = angle * 0.5f;
	FLOAT num = sinf(num2);
	FLOAT num3 = cosf(num2);
	result->X = axis->X * num;
	result->Y = axis->Y * num;
	result->Z = axis->Z * num;
	result->W = num3;
};

void Quaternion::CreateFromYawPitchRoll(FLOAT yaw, FLOAT pitch, FLOAT roll, Quaternion *result)
{
	FLOAT num9 = roll * 0.5f;
	FLOAT num6 = sinf(num9);
	FLOAT num5 = cosf(num9);
	FLOAT num8 = pitch * 0.5f;
	FLOAT num4 = sinf(num8);
	FLOAT num3 = cosf(num8);
	FLOAT num7 = yaw * 0.5f;
	FLOAT num2 = sinf(num7);
	FLOAT num = cosf(num7);
	result->X = ((num * num4) * num5) + ((num2 * num3) * num6);
	result->Y = ((num2 * num3) * num5) - ((num * num4) * num6);
	result->Z = ((num * num3) * num6) - ((num2 * num4) * num5);
	result->W = ((num * num3) * num5) + ((num2 * num4) * num6);
};

void Quaternion::CreateFromRotationMatrix(const Matrix *matrix, Quaternion *result)
{
	FLOAT num8 = (matrix->M11 + matrix->M22) + matrix->M33;
	if (num8 > 0.0f)
	{
		FLOAT num = sqrtf(num8 + 1.0f);
		result->W = num * 0.5f;
		num = 0.5f / num;
		result->X = (matrix->M23 - matrix->M32) * num;
		result->Y = (matrix->M31 - matrix->M13) * num;
		result->Z = (matrix->M12 - matrix->M21) * num;
	}
	else if ((matrix->M11 >= matrix->M22) && (matrix->M11 >= matrix->M33))
	{
		FLOAT num7 = sqrtf(((1.0f + matrix->M11) - matrix->M22) - matrix->M33);
		FLOAT num4 = 0.5f / num7;
		result->X = 0.5f * num7;
		result->Y = (matrix->M12 + matrix->M21) * num4;
		result->Z = (matrix->M13 + matrix->M31) * num4;
		result->W = (matrix->M23 - matrix->M32) * num4;
	}
	else if (matrix->M22 > matrix->M33)
	{
		FLOAT num6 = sqrtf(((1.0f + matrix->M22) - matrix->M11) - matrix->M33);
		FLOAT num3 = 0.5f / num6;
		result->X = (matrix->M21 + matrix->M12) * num3;
		result->Y = 0.5f * num6;
		result->Z = (matrix->M32 + matrix->M23) * num3;
		result->W = (matrix->M31 - matrix->M13) * num3;
	}
	else
	{
		FLOAT num5 = sqrtf(((1.0f + matrix->M33) - matrix->M11) - matrix->M22);
		FLOAT num2 = 0.5f / num5;
		result->X = (matrix->M31 + matrix->M13) * num2;
		result->Y = (matrix->M32 + matrix->M23) * num2;
		result->Z = 0.5f * num5;
		result->W = (matrix->M12 - matrix->M21) * num2;
	}
};

FLOAT Quaternion::Dot(const Quaternion *quaternion1, const Quaternion *quaternion2)
{
	return ((((quaternion1->X * quaternion2->X) + (quaternion1->Y * quaternion2->Y)) + (quaternion1->Z * quaternion2->Z)) + (quaternion1->W * quaternion2->W));
};

void Quaternion::Slerp(const Quaternion *quaternion1, const Quaternion *quaternion2, FLOAT amount, Quaternion *result)
{
	FLOAT num2;
	FLOAT num3;
	FLOAT num = amount;
	FLOAT num4 = (((quaternion1->X * quaternion2->X) + (quaternion1->Y * quaternion2->Y)) + (quaternion1->Z * quaternion2->Z)) + (quaternion1->W * quaternion2->W);
	BOOL flag = FALSE;
	if (num4 < 0.0f)
	{
		flag = TRUE;
		num4 = -num4;
	}
	if (num4 > 0.999999f)
	{
		num3 = 1.0f - num;
		num2 = flag ? -num : num;
	}
	else
	{
		FLOAT num5 = acosf(num4);
		FLOAT num6 = 1.0f / sinf(num5);
		num3 = sinf((1.0f - num) * num5) * num6;
		num2 = flag ? (-sinf(num * num5) * num6) : (sinf(num * num5) * num6);
	}
	result->X = (num3 * quaternion1->X) + (num2 * quaternion2->X);
	result->Y = (num3 * quaternion1->Y) + (num2 * quaternion2->Y);
	result->Z = (num3 * quaternion1->Z) + (num2 * quaternion2->Z);
	result->W = (num3 * quaternion1->W) + (num2 * quaternion2->W);
};

void Quaternion::Lerp(const Quaternion *quaternion1, const Quaternion *quaternion2, FLOAT amount, Quaternion *result)
{
	FLOAT num = amount;
	FLOAT num2 = 1.0f - num;
	FLOAT num5 = (((quaternion1->X * quaternion2->X) + (quaternion1->Y * quaternion2->Y)) + (quaternion1->Z * quaternion2->Z)) + (quaternion1->W * quaternion2->W);
	if (num5 >= 0.0f)
	{
		result->X = (num2 * quaternion1->X) + (num * quaternion2->X);
		result->Y = (num2 * quaternion1->Y) + (num * quaternion2->Y);
		result->Z = (num2 * quaternion1->Z) + (num * quaternion2->Z);
		result->W = (num2 * quaternion1->W) + (num * quaternion2->W);
	}
	else
	{
		result->X = (num2 * quaternion1->X) - (num * quaternion2->X);
		result->Y = (num2 * quaternion1->Y) - (num * quaternion2->Y);
		result->Z = (num2 * quaternion1->Z) - (num * quaternion2->Z);
		result->W = (num2 * quaternion1->W) - (num * quaternion2->W);
	}
	FLOAT num4 = (((result->X * result->X) + (result->Y * result->Y)) + (result->Z * result->Z)) + (result->W * result->W);
	FLOAT num3 = 1.0f / sqrtf(num4);
	result->X *= num3;
	result->Y *= num3;
	result->Z *= num3;
	result->W *= num3;
};

void Quaternion::Concatenate(const Quaternion *value1, const Quaternion *value2, Quaternion *result)
{
	FLOAT x = value2->X;
	FLOAT y = value2->Y;
	FLOAT z = value2->Z;
	FLOAT w = value2->W;
	FLOAT num4 = value1->X;
	FLOAT num3 = value1->Y;
	FLOAT num2 = value1->Z;
	FLOAT num = value1->W;
	FLOAT num12 = (y * num2) - (z * num3);
	FLOAT num11 = (z * num4) - (x * num2);
	FLOAT num10 = (x * num3) - (y * num4);
	FLOAT num9 = ((x * num4) + (y * num3)) + (z * num2);
	result->X = ((x * num) + (num4 * w)) + num12;
	result->Y = ((y * num) + (num3 * w)) + num11;
	result->Z = ((z * num) + (num2 * w)) + num10;
	result->W = (w * num) - num9;
}

void Quaternion::Negate(const Quaternion *quaternion, Quaternion *result)
{
	result->X = -quaternion->X;
	result->Y = -quaternion->Y;
	result->Z = -quaternion->Z;
	result->W = -quaternion->W;
};

void Quaternion::Add(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result)
{
	result->X = quaternion1->X + quaternion2->X;
	result->Y = quaternion1->Y + quaternion2->Y;
	result->Z = quaternion1->Z + quaternion2->Z;
	result->W = quaternion1->W + quaternion2->W;
};

void Quaternion::Subtract(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result)
{
	result->X = quaternion1->X - quaternion2->X;
	result->Y = quaternion1->Y - quaternion2->Y;
	result->Z = quaternion1->Z - quaternion2->Z;
	result->W = quaternion1->W - quaternion2->W;
};

void Quaternion::Multiply(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result)
{
	FLOAT x = quaternion1->X;
	FLOAT y = quaternion1->Y;
	FLOAT z = quaternion1->Z;
	FLOAT w = quaternion1->W;
	FLOAT num4 = quaternion2->X;
	FLOAT num3 = quaternion2->Y;
	FLOAT num2 = quaternion2->Z;
	FLOAT num = quaternion2->W;
	FLOAT num12 = (y * num2) - (z * num3);
	FLOAT num11 = (z * num4) - (x * num2);
	FLOAT num10 = (x * num3) - (y * num4);
	FLOAT num9 = ((x * num4) + (y * num3)) + (z * num2);
	result->X = ((x * num) + (num4 * w)) + num12;
	result->Y = ((y * num) + (num3 * w)) + num11;
	result->Z = ((z * num) + (num2 * w)) + num10;
	result->W = (w * num) - num9;
};

void Quaternion::Multiply(const Quaternion *quaternion1, FLOAT scaleFactor, Quaternion *result)
{
	result->X = quaternion1->X * scaleFactor;
	result->Y = quaternion1->Y * scaleFactor;
	result->Z = quaternion1->Z * scaleFactor;
	result->W = quaternion1->W * scaleFactor;
};

void Quaternion::Divide(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result)
{
	FLOAT x = quaternion1->X;
	FLOAT y = quaternion1->Y;
	FLOAT z = quaternion1->Z;
	FLOAT w = quaternion1->W;
	FLOAT num14 = (((quaternion2->X * quaternion2->X) + (quaternion2->Y * quaternion2->Y)) + (quaternion2->Z * quaternion2->Z)) + (quaternion2->W * quaternion2->W);
	FLOAT num5 = 1.0f / num14;
	FLOAT num4 = -quaternion2->X * num5;
	FLOAT num3 = -quaternion2->Y * num5;
	FLOAT num2 = -quaternion2->Z * num5;
	FLOAT num = quaternion2->W * num5;
	FLOAT num13 = (y * num2) - (z * num3);
	FLOAT num12 = (z * num4) - (x * num2);
	FLOAT num11 = (x * num3) - (y * num4);
	FLOAT num10 = ((x * num4) + (y * num3)) + (z * num2);
	result->X = ((x * num) + (num4 * w)) + num13;
	result->Y = ((y * num) + (num3 * w)) + num12;
	result->Z = ((z * num) + (num2 * w)) + num11;
	result->W = (w * num) - num10;
};

void Quaternion::Copy(const Quaternion *source, Quaternion *destination)
{
	destination->X = source->X;
	destination->Y = source->Y;
	destination->Z = source->Z;
	destination->W = source->W;
};


Quaternion Quaternion::operator -() const
{
    Quaternion quaternion2;
    quaternion2.X = -this->X;
    quaternion2.Y = -this->Y;
    quaternion2.Z = -this->Z;
    quaternion2.W = -this->W;
    return quaternion2;
};

BOOL Quaternion::operator ==(const Quaternion quaternion2) const
{
    return ((((this->X == quaternion2.X) && (this->Y == quaternion2.Y)) && (this->Z == quaternion2.Z)) && (this->W == quaternion2.W));
};

BOOL Quaternion::operator !=(const Quaternion quaternion2) const
{
    if (((this->X == quaternion2.X) && (this->Y == quaternion2.Y)) && (this->Z == quaternion2.Z))
    {
        return (this->W != quaternion2.W);
    }
    return true;
};

Quaternion Quaternion::operator +(const Quaternion quaternion2) const
{
    Quaternion quaternion;
    quaternion.X = this->X + quaternion2.X;
    quaternion.Y = this->Y + quaternion2.Y;
    quaternion.Z = this->Z + quaternion2.Z;
    quaternion.W = this->W + quaternion2.W;
    return quaternion;
};

Quaternion Quaternion::operator -(const Quaternion quaternion2) const
{
    Quaternion quaternion;
    quaternion.X = this->X - quaternion2.X;
    quaternion.Y = this->Y - quaternion2.Y;
    quaternion.Z = this->Z - quaternion2.Z;
    quaternion.W = this->W - quaternion2.W;
    return quaternion;
};

Quaternion Quaternion::operator *(const Quaternion quaternion2) const
{
    Quaternion quaternion;
    float x = this->X;
    float y = this->Y;
    float z = this->Z;
    float w = this->W;
    float num4 = quaternion2.X;
    float num3 = quaternion2.Y;
    float num2 = quaternion2.Z;
    float num = quaternion2.W;
    float num12 = (y * num2) - (z * num3);
    float num11 = (z * num4) - (x * num2);
    float num10 = (x * num3) - (y * num4);
    float num9 = ((x * num4) + (y * num3)) + (z * num2);
    quaternion.X = ((x * num) + (num4 * w)) + num12;
    quaternion.Y = ((y * num) + (num3 * w)) + num11;
    quaternion.Z = ((z * num) + (num2 * w)) + num10;
    quaternion.W = (w * num) - num9;
    return quaternion;
};

Quaternion Quaternion::operator *(float scaleFactor) const
{
    Quaternion quaternion;
    quaternion.X = this->X * scaleFactor;
    quaternion.Y = this->Y * scaleFactor;
    quaternion.Z = this->Z * scaleFactor;
    quaternion.W = this->W * scaleFactor;
    return quaternion;
};

Quaternion Quaternion::operator /( const Quaternion quaternion2) const
{
    Quaternion quaternion;
    float x = this->X;
    float y = this->Y;
    float z = this->Z;
    float w = this->W;
    float num14 = (((quaternion2.X * quaternion2.X) + (quaternion2.Y * quaternion2.Y)) + (quaternion2.Z * quaternion2.Z)) + (quaternion2.W * quaternion2.W);
    float num5 = 1.0f / num14;
    float num4 = -quaternion2.X * num5;
    float num3 = -quaternion2.Y * num5;
    float num2 = -quaternion2.Z * num5;
    float num = quaternion2.W * num5;
    float num13 = (y * num2) - (z * num3);
    float num12 = (z * num4) - (x * num2);
    float num11 = (x * num3) - (y * num4);
    float num10 = ((x * num4) + (y * num3)) + (z * num2);
    quaternion.X = ((x * num) + (num4 * w)) + num13;
    quaternion.Y = ((y * num) + (num3 * w)) + num12;
    quaternion.Z = ((z * num) + (num2 * w)) + num11;
    quaternion.W = (w * num) - num10;
    return quaternion;
};
