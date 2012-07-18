
#ifndef __Vector3_H
#define __Vector3_H

#include "EngineCommon.h"

struct Matrix;
struct Quaternion;
struct Vector2;

struct Vector3 //100%
{
public:
	FLOAT X, Y, Z; 

	Vector3();
	Vector3(FLOAT value);
	Vector3(FLOAT x, FLOAT y, FLOAT z);
	Vector3(const Vector2 *value, FLOAT z);

	static BOOL Equals(const Vector3 *value1, const Vector3 *value2);
	static FLOAT Length(const Vector3 *value);
	static FLOAT LengthSquared(const Vector3 *value);
	static FLOAT Distance(const Vector3 *value1, const Vector3 *value2);
	static FLOAT DistanceSquared(const Vector3 *value1, const Vector3 *value2);
	static FLOAT Dot(const Vector3 *value1, const Vector3 *value2);
	static void Normalize(const Vector3 *value, Vector3 *result);
	static void Cross(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Reflect(const Vector3 *value, const Vector3 *normal, Vector3 *result);
	static void Min(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Max(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Clamp(const Vector3 *value1, const Vector3 *min, const Vector3 *max, Vector3 *result);
	static void Lerp(const Vector3 *value1, const Vector3 *value2, FLOAT amount, Vector3 *result);
	static void Barycentric(const Vector3 *value1, const Vector3 *value2, const Vector3 *value3, FLOAT amount1, FLOAT amount2, Vector3 *result);
	static void SmoothStep(const Vector3 *value1, const Vector3 *value2, FLOAT amount, Vector3 *result);
	static void CatmullRom(const Vector3 *value1, const Vector3 *value2, const Vector3 *value3, const Vector3 *value4, FLOAT amount, Vector3 *result);
	static void Hermite(const Vector3 *value1, const Vector3 *tangent1, const Vector3 *value2, const Vector3 *tangent2, FLOAT amount, Vector3 *result);
	static void Transform(const Vector3 *position, const Matrix *matrix, Vector3 *result);
	static void TransformNormal(const Vector3 *normal, const Matrix *matrix, Vector3 *result);
	static void Transform(const Vector3 *value, const Quaternion *rotation, Vector3 *result);
	
	static void Transform(INT sourceArraySize, Vector3 sourceArray[], const Matrix *matrix, Vector3 destinationArray[]);
	static void Transform(Vector3 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector3 destinationArray[], INT destinationIndex, INT length);
	static void TransformNormal(INT sourceArraySize, Vector3 sourceArray[], const Matrix *matrix, Vector3 destinationArray[]);
	static void TransformNormal(Vector3 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector3 destinationArray[], INT destinationIndex, INT length);
	static void Transform(INT sourceArraySize, Vector3 sourceArray[], const Quaternion *rotation, Vector3 destinationArray[]);
	static void Transform(Vector3 sourceArray[], INT sourceIndex, const Quaternion *rotation, Vector3 destinationArray[], INT destinationIndex, INT length);

	static void Negate(const Vector3 *value, Vector3 *result);
	static void Add(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Subtract(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Multiply(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Multiply(const Vector3 *value1, FLOAT scaleFactor, Vector3 *result);
	static void Divide(const Vector3 *value1, const Vector3 *value2, Vector3 *result);
	static void Divide(const Vector3 *value1, FLOAT value2, Vector3 *result);
	static void Copy(const Vector3 *source, Vector3 *destination);

	Vector3 operator-() const;
	BOOL operator==(const Vector3 value2) const;
    BOOL operator!=(const Vector3 value2) const;
	Vector3 operator+(const Vector3 value2) const;
    Vector3 operator-(const Vector3 value2) const;
    Vector3 operator*(const Vector3 value2) const;
	Vector3 operator*(float scaleFactor) const;
    Vector3 operator/(const Vector3 value2) const;
    Vector3 operator/(float divider) const;

};

//metodos nao membros
inline Vector3 operator *(float scaleFactor, const Vector3 value2) {
	Vector3 vector;
    vector.X = value2.X * scaleFactor;
    vector.Y = value2.Y * scaleFactor;
    vector.Z = value2.Z * scaleFactor;
    return vector;	
}

#endif