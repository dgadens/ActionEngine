
#ifndef __Vector2_H
#define __Vector2_H

#include "EngineCommon.h"

struct Matrix;
struct Quaternion;

struct Vector2 //100%
{
public:
	float X, Y;

	Vector2();
	Vector2(FLOAT value);
	Vector2(FLOAT x, FLOAT y);

	static BOOL Equals(const Vector2 *value, const Vector2 *value2);
	static FLOAT Length(const Vector2 *value);
	static FLOAT LengthSquared(const Vector2 *value);
	static FLOAT Distance(const Vector2 *value1, const Vector2 *value2);
	static FLOAT DistanceSquared(const Vector2 *value1, const Vector2 *value2);
	static FLOAT Dot(const Vector2 *value1, const Vector2 *value2);
	static void Normalize(const Vector2 *value, Vector2 *result);
	static void Reflect(const Vector2 *vector, const Vector2 *normal, Vector2 *result);
	static void Min(const Vector2 *value1, const Vector2 *value2, Vector2 *result);
	static void Max(const Vector2 *value1, const Vector2 *value2, Vector2 *result);
	static void Clamp(const Vector2 *value1, const Vector2 *min, const Vector2 *max, Vector2 *result);
	static void Lerp(const Vector2 *value1, const Vector2 *value2, FLOAT amount, Vector2 *result);
	static void Barycentric(const Vector2 *value1, const Vector2 *value2, const Vector2 *value3, FLOAT amount1, FLOAT amount2, Vector2 *result);
	static void SmoothStep(const Vector2 *value1, const Vector2 *value2, FLOAT amount, Vector2 *result);
	static void CatmullRom(const Vector2 *value1, const Vector2 *value2, const Vector2 *value3, const Vector2 *value4, FLOAT amount, Vector2 *result);
	static void Hermite(const Vector2 *value1, const Vector2 *tangent1, const Vector2 *value2, const Vector2 *tangent2, FLOAT amount, Vector2 *result);
	static void Transform(const Vector2 *position, const Matrix *matrix, Vector2 *result);
	static void TransformNormal(const Vector2 *normal, const Matrix *matrix, Vector2 *result);
	static void Transform(const Vector2 *value, const Quaternion *rotation, Vector2 *result);
	
	static void Transform(INT sourceArraySize, Vector2 sourceArray[], const Matrix *matrix, Vector2 destinationArray[]);
	static void Transform(Vector2 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector2 destinationArray[], INT destinationIndex, INT length);
	static void TransformNormal(INT sourceArraySize, Vector2 sourceArray[], const Matrix *matrix, Vector2 destinationArray[]);
	static void TransformNormal(Vector2 sourceArray[], INT sourceIndex, const Matrix *matrix, Vector2 destinationArray[], INT destinationIndex, INT length);
	static void Transform(INT sourceArraySize, Vector2 sourceArray[], const Quaternion *rotation, Vector2 destinationArray[]);
	static void Transform(Vector2 sourceArray[], INT sourceIndex, const Quaternion *rotation, Vector2 destinationArray[], INT destinationIndex, INT length);
	
	static void Negate(const Vector2 *value, Vector2 *result);
	static void Add(const Vector2 *value1, const Vector2 *value2, Vector2 *result);
	static void Subtract(const Vector2 *value1, const Vector2 *value2, Vector2 *result);
	static void Multiply(const Vector2 *value1, const Vector2 *value2, Vector2 *result);
	static void Multiply(const Vector2 *value1, FLOAT scaleFactor, Vector2 *result);
	static void Divide(const Vector2 *value1, const Vector2 *value2, Vector2 *result);
	static void Divide(const Vector2 *value1, FLOAT divider, Vector2 *result);
	static void Copy(const Vector2 *source, Vector2 *destination);

	Vector2 operator-() const;
	BOOL operator==(const Vector2 value2) const;
    BOOL operator!=(const Vector2 value2) const;
	Vector2 operator+(const Vector2 value2) const;
    Vector2 operator-(const Vector2 value2) const;
    Vector2 operator*(const Vector2 value2) const;
	Vector2 operator*(float scaleFactor) const;
    Vector2 operator/(const Vector2 value2) const;
    Vector2 operator/(float divider) const;

};

//metodos nao membros
inline Vector2 operator *(float scaleFactor, const Vector2 value2) {
	Vector2 vector;
    vector.X = value2.X * scaleFactor;
    vector.Y = value2.Y * scaleFactor;
    return vector;	
}

#endif