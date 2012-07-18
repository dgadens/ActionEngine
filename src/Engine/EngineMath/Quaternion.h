
#ifndef __QUATERNION_H
#define __QUATERNION_H

#include "EngineCommon.h"

struct Matrix;
struct Vector3;

struct Quaternion //100%
{
public:
	FLOAT X, Y, Z, W;

	Quaternion();
	Quaternion(FLOAT x, FLOAT y, FLOAT z, FLOAT w);
	Quaternion(const Vector3 *vectorPart, FLOAT scalarPart);

	static BOOL Equals(const Quaternion *value1, const Quaternion *value2);
	static FLOAT LengthSquared(const Quaternion *value);
	static FLOAT Length(const Quaternion *value);
	static void Normalize(const Quaternion *quaternion, Quaternion *result);
	static void Conjugate(const Quaternion *value, Quaternion *result);
	static void Inverse(const Quaternion *quaternion, Quaternion *result);
	static void CreateFromAxisAngle(const Vector3 *axis, FLOAT angle, Quaternion *result);
	static void CreateFromYawPitchRoll(FLOAT yaw, FLOAT pitch, FLOAT roll, Quaternion *result);
	static void CreateFromRotationMatrix(const Matrix *matrix, Quaternion *result);
	static FLOAT Dot(const Quaternion *quaternion1, const Quaternion *quaternion2);
	static void Slerp(const Quaternion *quaternion1, const Quaternion *quaternion2, FLOAT amount, Quaternion *result);
	static void Lerp(const Quaternion *quaternion1, const Quaternion *quaternion2, FLOAT amount, Quaternion *result);
	static void Concatenate(const Quaternion *value1, const Quaternion *value2, Quaternion *result);
	static void Negate(const Quaternion *quaternion, Quaternion *result);
	static void Add(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result);
	static void Subtract(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result);
	static void Multiply(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result);
	static void Multiply(const Quaternion *quaternion1, FLOAT scaleFactor, Quaternion *result);
	static void Divide(const Quaternion *quaternion1, const Quaternion *quaternion2, Quaternion *result);
	static void Copy(const Quaternion *source, Quaternion *destination);

	Quaternion operator-() const;
	BOOL operator==(const Quaternion value2) const;
    BOOL operator!=(const Quaternion value2) const;
	Quaternion operator+(const Quaternion value2) const;
    Quaternion operator-(const Quaternion value2) const;
    Quaternion operator*(const Quaternion value2) const;
	Quaternion operator*(float scaleFactor) const;
    Quaternion operator/(const Quaternion value2) const;
    Quaternion operator/(float divider) const;

};

#endif