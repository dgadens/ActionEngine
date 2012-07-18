
#ifndef __MATRIX_H
#define __MATRIX_H

#include "EngineCommon.h"

struct Vector3;
struct Quaternion;
struct Plane;

struct Matrix //100%
{
public:
	FLOAT M11, M12, M13, M14, M21, M22, M23, M24, M31, M32, M33, M34, M41, M42, M43, M44;

	Matrix(void);
	Matrix(FLOAT M11, FLOAT M12, FLOAT M13, FLOAT M14, 
			FLOAT M21, FLOAT M22, FLOAT M23, FLOAT M24, 
			FLOAT M31, FLOAT M32, FLOAT M33, FLOAT M34, 
			FLOAT M41, FLOAT M42, FLOAT M43, FLOAT M44);
	
	static void ToIdentity(Matrix *matrix);
	static BOOL Equals(const Matrix *value1, const Matrix *value2);
	static void CreateTranslation(const Vector3 *position, Matrix *result);
	static void CreateTranslation(FLOAT xPosition, FLOAT yPosition, FLOAT zPosition, Matrix *result);
	static void CreateScale(FLOAT xScale, FLOAT yScale, FLOAT zScale, Matrix *result);
	static void CreateScale(const Vector3 *scales, Matrix *result);
	static void CreateScale(FLOAT scale, Matrix *result);
	static void CreateRotationX(FLOAT radians, Matrix *result);
	static void CreateRotationY(FLOAT radians, Matrix *result);
	static void CreateRotationZ(FLOAT radians, Matrix *result);
	static void CreateFromAxisAngle(const Vector3 *axis, FLOAT angle, Matrix *result);
	static void CreatePerspectiveFieldOfView(FLOAT fieldOfView, FLOAT aspectRatio, FLOAT nearPlaneDistance, FLOAT farPlaneDistance, Matrix *result);
	static void CreatePerspective(FLOAT width, FLOAT height, FLOAT nearPlaneDistance, FLOAT farPlaneDistance, Matrix *result);
	static void CreatePerspectiveOffCenter(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT nearPlaneDistance, FLOAT farPlaneDistance, Matrix *result);
	static void CreateOrthographic(FLOAT width, FLOAT height, FLOAT zNearPlane, FLOAT zFarPlane, Matrix *result);
	static void CreateOrthographicOffCenter(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT zNearPlane, FLOAT zFarPlane, Matrix *result);
	static void CreateLookAt(const Vector3 *cameraPosition, const Vector3 *cameraTarget, const Vector3 *cameraUpVector, Matrix *result);
	static void CreateFromQuaternion(const Quaternion *quaternion, Matrix *result);
	static void CreateFromYawPitchRoll(FLOAT yaw, FLOAT pitch, FLOAT roll, Matrix *result);
	static void CreateShadow(const Vector3 *lightDirection, const Plane *plane, Matrix *result);
	static void Transform(const Matrix *value, const Quaternion *rotation, Matrix *result);
	static void Transpose(const Matrix *matrix, Matrix *result);
	static FLOAT Determinant(const Matrix *value);
	static void Invert(const Matrix *matrix, Matrix *result);
	static void Lerp(const Matrix *matrix1, const Matrix *matrix2, FLOAT amount, Matrix *result);
	static void Negate(const Matrix *matrix, Matrix *result);
	static void Add(const Matrix *matrix1, const Matrix *matrix2, Matrix *result);
	static void Subtract(const Matrix *matrix1, const Matrix *matrix2, Matrix *result);
	static void Multiply(const Matrix *matrix1, const Matrix *matrix2, Matrix *result);
	static void Multiply(const Matrix *matrix1, FLOAT scaleFactor, Matrix *result);
	static void Divide(const Matrix *matrix1, const Matrix *matrix2, Matrix *result);
	static void Divide(const Matrix *matrix1, FLOAT divider, Matrix *result);
	static void Unproject(Vector3 *source, const Matrix *projection, const Matrix *view, const Matrix *world, INT viewPortWidth, INT viewPortHeight, FLOAT nearPlane, FLOAT farPlane, Vector3 *result);
	static void Project(const Vector3 *source, const Matrix *projection, const Matrix *view, const Matrix *world, INT viewPortWidth, INT viewPortHeight, FLOAT nearPlane, FLOAT farPlane, Vector3 *result);
	static void Copy(const Matrix *source, Matrix *destination);

    Matrix operator -() const;
    BOOL operator ==(const Matrix matrix2) const;
    BOOL operator !=(const Matrix matrix2) const;
	Matrix operator +(const Matrix matrix2) const;
    Matrix operator -(const Matrix matrix2) const;
    Matrix operator *(const Matrix matrix2) const;
	Matrix operator *(float scaleFactor) const;
	Matrix operator /(const Matrix matrix2) const;
	Matrix operator /(float divider) const;

};

//metodos nao membros
inline Matrix operator *(float scaleFactor, const Matrix value2) {
    Matrix matrix2;
    float num = scaleFactor;
    matrix2.M11 = value2.M11 * num;
    matrix2.M12 = value2.M12 * num;
    matrix2.M13 = value2.M13 * num;
    matrix2.M14 = value2.M14 * num;
    matrix2.M21 = value2.M21 * num;
    matrix2.M22 = value2.M22 * num;
    matrix2.M23 = value2.M23 * num;
    matrix2.M24 = value2.M24 * num;
    matrix2.M31 = value2.M31 * num;
    matrix2.M32 = value2.M32 * num;
    matrix2.M33 = value2.M33 * num;
    matrix2.M34 = value2.M34 * num;
    matrix2.M41 = value2.M41 * num;
    matrix2.M42 = value2.M42 * num;
    matrix2.M43 = value2.M43 * num;
    matrix2.M44 = value2.M44 * num;
    return matrix2;
}

#endif