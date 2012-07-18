
#ifndef __Vector4_H
#define __Vector4_H

#include "EngineCommon.h"

struct Matrix;
struct Quaternion;
struct Vector2;
struct Vector3;

struct Vector4 //100%
{
public:
	FLOAT X, Y, Z, W;

	Vector4();
	Vector4(FLOAT value);
	Vector4(FLOAT x, FLOAT y, FLOAT z, FLOAT w);
	Vector4(const Vector2 *value, FLOAT z, FLOAT w);
	Vector4(const Vector3 *value, FLOAT w);

	static BOOL Equals(const Vector4 *value1, const Vector4 *value2);
	static FLOAT Length(const Vector4 *value);
	static FLOAT LengthSquared(const Vector4 *value);
	static FLOAT Distance(const Vector4 *value1, const Vector4 *value2);
	static FLOAT DistanceSquared(const Vector4 *value1, const Vector4 *value2);
	static FLOAT Dot(const Vector4 *value1, const Vector4 *value2);
	static void Normalize(const Vector4 *value, Vector4 *result);
	static void Min(const Vector4 *value1, const Vector4 *value2, Vector4 *result);
	static void Max(const Vector4 *value1, const Vector4 *value2, Vector4 *result);
	static void Clamp(const Vector4 *value1, const Vector4 *min, const Vector4 *max, Vector4 *result);
	static void Lerp(const Vector4 *value1,const  Vector4 *value2, FLOAT amount, Vector4 *result);
	static void Barycentric(const Vector4 *value1, const Vector4 *value2, const Vector4 *value3, FLOAT amount1, FLOAT amount2, Vector4 *result);
	static void SmoothStep(const Vector4 *value1, const Vector4 *value2, FLOAT amount, Vector4 *result);
	static void CatmullRom(const Vector4 *value1, const Vector4 *value2, const Vector4 *value3, const Vector4 *value4, FLOAT amount, Vector4 *result);
	static void Hermite(const Vector4 *value1, const Vector4 *tangent1, const Vector4 *value2, const Vector4 *tangent2, FLOAT amount, Vector4 *result);
	static void Transform(const Vector2 *position, const Matrix *matrix, Vector4 *result);
	static void Transform(const Vector3 *position, const Matrix *matrix, Vector4 *result);
	static void Transform(const Vector4 *vector, const Matrix *matrix, Vector4 *result);
	static void Transform(const Vector2 *value, const Quaternion *rotation, Vector4 *result);
	static void Transform(const Vector3 *value, const Quaternion *rotation, Vector4 *result);
	static void Transform(const Vector4 *value, const Quaternion *rotation, Vector4 *result);
	static void Transform(INT sourceArraySize, Vector4 sourceArray[], const Matrix *matrix, Vector4 destinationArray[]);
	static void Transform(Vector4 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector4 destinationArray[], INT destinationIndex, INT length);
	static void Transform(INT sourceArraySize, Vector4 sourceArray[], const Quaternion *rotation, Vector4 destinationArray[]);
	static void Transform(Vector4 sourceArray[], INT sourceIndex, const Quaternion *rotation, Vector4 destinationArray[], INT destinationIndex, INT length);
	static void Negate(const Vector4 *value, Vector4 *result);
	static void Add(const Vector4 *value1, const Vector4 *value2, Vector4 *result);
	static void Subtract(const Vector4 *value1, const Vector4 *value2, Vector4 *result);
	static void Multiply(const Vector4 *value1, const Vector4 *value2, Vector4 *result);
	static void Multiply(const Vector4 *value1, FLOAT scaleFactor, Vector4 *result);
	static void Divide(const Vector4 *value1, const Vector4 *value2, Vector4 *result);
	static void Divide(const Vector4 *value1, FLOAT value2, Vector4 *result);
	static void Copy(const Vector4 *source, Vector4 *destination);

	//Vector4 operator=(const Vector4 value) const;
	Vector4 operator-() const;
	BOOL operator==(const Vector4 value2) const;
    BOOL operator!=(const Vector4 value2) const;
	Vector4 operator+(const Vector4 value2) const;
    Vector4 operator-(const Vector4 value2) const;
    Vector4 operator*(const Vector4 value2) const;
	Vector4 operator*(float scaleFactor) const;
    Vector4 operator/(const Vector4 value2) const;
    Vector4 operator/(float divider) const;

};

//metodos nao membros
inline Vector4 operator *(float scaleFactor, const Vector4 value2) {
	Vector4 vector;
    vector.X = value2.X * scaleFactor;
    vector.Y = value2.Y * scaleFactor;
    vector.Z = value2.Z * scaleFactor;
    vector.W = value2.W * scaleFactor;
    return vector;	
}

#endif