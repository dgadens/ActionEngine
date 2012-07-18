#include "Matrix.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Plane.h"
#include <math.h>

Matrix::Matrix()
{
	M11 = 1.0f;
	M12 = 0.0f;
	M13 = 0.0f;
	M14 = 0.0f;
	M21 = 0.0f;
	M22 = 1.0f;
	M23 = 0.0f;
	M24 = 0.0f;
	M31 = 0.0f;
	M32 = 0.0f;
	M33 = 1.0f;
	M34 = 0.0f;
	M41 = 0.0f;
	M42 = 0.0f;
	M43 = 0.0f;
	M44 = 1.0f;
};

Matrix::Matrix(FLOAT m11, FLOAT m12, FLOAT m13, FLOAT m14, 
				FLOAT m21, FLOAT m22, FLOAT m23, FLOAT m24, 
				FLOAT m31, FLOAT m32, FLOAT m33, FLOAT m34, 
				FLOAT m41, FLOAT m42, FLOAT m43, FLOAT m44)
{
	M11 = m11;
	M12 = m12;
	M13 = m13;
	M14 = m14;
	M21 = m21;
	M22 = m22;
	M23 = m23;
	M24 = m24;
	M31 = m31;
	M32 = m32;
	M33 = m33;
	M34 = m34;
	M41 = m41;
	M42 = m42;
	M43 = m43;
	M44 = m44;
};

void Matrix::ToIdentity(Matrix *matrix)
{
	matrix->M11 = 1.0f;
	matrix->M12 = 0.0f;
	matrix->M13 = 0.0f;
	matrix->M14 = 0.0f;
	matrix->M21 = 0.0f;
	matrix->M22 = 1.0f;
	matrix->M23 = 0.0f;
	matrix->M24 = 0.0f;
	matrix->M31 = 0.0f;
	matrix->M32 = 0.0f;
	matrix->M33 = 1.0f;
	matrix->M34 = 0.0f;
	matrix->M41 = 0.0f;
	matrix->M42 = 0.0f;
	matrix->M43 = 0.0f;
	matrix->M44 = 1.0f;
};

BOOL Matrix::Equals(const Matrix *value1, const Matrix *value2)
{
	return ((((((value1->M11 == value2->M11) && (value1->M22 == value2->M22)) && ((value1->M33 == value2->M33) && (value1->M44 == value2->M44))) && (((value1->M12 == value2->M12) && (value1->M13 == value2->M13)) && ((value1->M14 == value2->M14) && (value1->M21 == value2->M21)))) && ((((value1->M23 == value2->M23) && (value1->M24 == value2->M24)) && ((value1->M31 == value2->M31) && (value1->M32 == value2->M32))) && (((value1->M34 == value2->M34) && (value1->M41 == value2->M41)) && (value1->M42 == value2->M42)))) && (value1->M43 == value2->M43));
}

void Matrix::CreateTranslation(const Vector3 *position, Matrix *result)
{
	result->M11 = 1.0f;
	result->M12 = 0.0f;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = 1.0f;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = 0.0f;
	result->M33 = 1.0f;
	result->M34 = 0.0f;
	result->M41 = position->X;
	result->M42 = position->Y;
	result->M43 = position->Z;
	result->M44 = 1.0f;
};

void Matrix::CreateTranslation(FLOAT xPosition, FLOAT yPosition, FLOAT zPosition, Matrix *result)
{
	result->M11 = 1.0f;
	result->M12 = 0.0f;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = 1.0f;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = 0.0f;
	result->M33 = 1.0f;
	result->M34 = 0.0f;
	result->M41 = xPosition;
	result->M42 = yPosition;
	result->M43 = zPosition;
	result->M44 = 1.0f;
};

void Matrix::CreateScale(FLOAT xScale, FLOAT yScale, FLOAT zScale, Matrix *result)
{
	FLOAT num3 = xScale;
	FLOAT num2 = yScale;
	FLOAT num = zScale;
	result->M11 = num3;
	result->M12 = 0.0f;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = num2;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = 0.0f;
	result->M33 = num;
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateScale(const Vector3 *scales, Matrix *result)
{
	FLOAT x = scales->X;
	FLOAT y = scales->Y;
	FLOAT z = scales->Z;
	result->M11 = x;
	result->M12 = 0.0f;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = y;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = 0.0f;
	result->M33 = z;
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateScale(FLOAT scale, Matrix *result)
{
	FLOAT num = scale;
	result->M11 = num;
	result->M12 = 0.0f;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = num;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = 0.0f;
	result->M33 = num;
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateRotationX(FLOAT radians, Matrix *result)
{
	FLOAT num2 = cosf(radians);
	FLOAT num = sinf(radians);
	result->M11 = 1.0f;
	result->M12 = 0.0f;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = num2;
	result->M23 = num;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = -num;
	result->M33 = num2;
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateRotationY(FLOAT radians, Matrix *result)
{
	FLOAT num2 = cosf(radians);
	FLOAT num = sinf(radians);
	result->M11 = num2;
	result->M12 = 0.0f;
	result->M13 = -num;
	result->M14 = 0.0f;
	result->M21 = 0.0f;
	result->M22 = 1.0f;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = num;
	result->M32 = 0.0f;
	result->M33 = num2;
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateRotationZ(FLOAT radians, Matrix *result)
{
	FLOAT num2 = cosf(radians);
	FLOAT num = sinf(radians);
	result->M11 = num2;
	result->M12 = num;
	result->M13 = 0.0f;
	result->M14 = 0.0f;
	result->M21 = -num;
	result->M22 = num2;
	result->M23 = 0.0f;
	result->M24 = 0.0f;
	result->M31 = 0.0f;
	result->M32 = 0.0f;
	result->M33 = 1.0f;
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateFromAxisAngle(const Vector3 *axis, FLOAT angle, Matrix *result)
{
	FLOAT x = axis->X;
	FLOAT y = axis->Y;
	FLOAT z = axis->Z;
	FLOAT num2 = sinf(angle);
	FLOAT num = cosf(angle);
	FLOAT num11 = x * x;
	FLOAT num10 = y * y;
	FLOAT num9 = z * z;
	FLOAT num8 = x * y;
	FLOAT num7 = x * z;
	FLOAT num6 = y * z;
	result->M11 = num11 + (num * (1.0f - num11));
	result->M12 = (num8 - (num * num8)) + (num2 * z);
	result->M13 = (num7 - (num * num7)) - (num2 * y);
	result->M14 = 0.0f;
	result->M21 = (num8 - (num * num8)) - (num2 * z);
	result->M22 = num10 + (num * (1.0f - num10));
	result->M23 = (num6 - (num * num6)) + (num2 * x);
	result->M24 = 0.0f;
	result->M31 = (num7 - (num * num7)) + (num2 * y);
	result->M32 = (num6 - (num * num6)) - (num2 * x);
	result->M33 = num9 + (num * (1.0f - num9));
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreatePerspectiveFieldOfView(FLOAT fieldOfView, FLOAT aspectRatio, FLOAT nearPlaneDistance, FLOAT farPlaneDistance, Matrix *result)
{
	FLOAT num = 1.0f / tanf(fieldOfView * 0.5f);
	FLOAT num9 = num / aspectRatio;
	result->M11 = num9;
	result->M12 = result->M13 = result->M14 = 0.0f;
	result->M22 = num;
	result->M21 = result->M23 = result->M24 = 0.0f;
	result->M31 = result->M32 = 0.0f;
	result->M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
	result->M34 = -1.0f;
	result->M41 = result->M42 = result->M44 = 0.0f;
	result->M43 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
};

void Matrix::CreatePerspective(FLOAT width, FLOAT height, FLOAT nearPlaneDistance, FLOAT farPlaneDistance, Matrix *result)
{
	result->M11 = (2.0f * nearPlaneDistance) / width;
	result->M12 = result->M13 = result->M14 = 0.0f;
	result->M22 = (2.0f * nearPlaneDistance) / height;
	result->M21 = result->M23 = result->M24 = 0.0f;
	result->M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
	result->M31 = result->M32 = 0.0f;
	result->M34 = -1.0f;
	result->M41 = result->M42 = result->M44 = 0.0f;
	result->M43 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
};

void Matrix::CreatePerspectiveOffCenter(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT nearPlaneDistance, FLOAT farPlaneDistance, Matrix *result)
{
	result->M11 = (2.0f * nearPlaneDistance) / (right - left);
	result->M12 = result->M13 = result->M14 = 0.0f;
	result->M22 = (2.0f * nearPlaneDistance) / (top - bottom);
	result->M21 = result->M23 = result->M24 = 0.0f;
	result->M31 = (left + right) / (right - left);
	result->M32 = (top + bottom) / (top - bottom);
	result->M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
	result->M34 = -1.0f;
	result->M43 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
	result->M41 = result->M42 = result->M44 = 0.0f;
};

void Matrix::CreateOrthographic(FLOAT width, FLOAT height, FLOAT zNearPlane, FLOAT zFarPlane, Matrix *result)
{
	result->M11 = 2.0f / width;
	result->M12 = result->M13 = result->M14 = 0.0f;
	result->M22 = 2.0f / height;
	result->M21 = result->M23 = result->M24 = 0.0f;
	result->M33 = 1.0f / (zNearPlane - zFarPlane);
	result->M31 = result->M32 = result->M34 = 0.0f;
	result->M41 = result->M42 = 0.0f;
	result->M43 = zNearPlane / (zNearPlane - zFarPlane);
	result->M44 = 1.0f;
};

void Matrix::CreateOrthographicOffCenter(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT zNearPlane, FLOAT zFarPlane, Matrix *result)
{
	result->M11 = 2.0f / (right - left);
	result->M12 = result->M13 = result->M14 = 0.0f;
	result->M22 = 2.0f / (top - bottom);
	result->M21 = result->M23 = result->M24 = 0.0f;
	result->M33 = 1.0f / (zNearPlane - zFarPlane);
	result->M31 = result->M32 = result->M34 = 0.0f;
	result->M41 = (left + right) / (left - right);
	result->M42 = (top + bottom) / (bottom - top);
	result->M43 = zNearPlane / (zNearPlane - zFarPlane);
	result->M44 = 1.0f;
};

void Matrix::CreateLookAt(const Vector3 *cameraPosition, const Vector3 *cameraTarget, const Vector3 *cameraUpVector, Matrix *result)
{
	Vector3 vector(0);
	Vector3 Vector2(0);
	Vector3 Vector3(0);
	
	Vector3::Subtract(cameraPosition, cameraTarget, &vector);
	Vector3::Normalize(&vector, &vector);
	Vector3::Cross(cameraUpVector, &vector, &Vector2);
	Vector3::Normalize(&Vector2, &Vector2);
	Vector3::Cross(&vector, &Vector2, &Vector3);
	result->M11 = Vector2.X;
	result->M12 = Vector3.X;
	result->M13 = vector.X;
	result->M14 = 0.0f;
	result->M21 = Vector2.Y;
	result->M22 = Vector3.Y;
	result->M23 = vector.Y;
	result->M24 = 0.0f;
	result->M31 = Vector2.Z;
	result->M32 = Vector3.Z;
	result->M33 = vector.Z;
	result->M34 = 0.0f;
	result->M41 = -(Vector3::Dot(&Vector2, cameraPosition));
	result->M42 = -(Vector3::Dot(&Vector3, cameraPosition));
	result->M43 = -(Vector3::Dot(&vector, cameraPosition));
	result->M44 = 1.0f;
};

void Matrix::CreateFromQuaternion(const Quaternion *quaternion, Matrix *result)
{
	FLOAT num9 = quaternion->X * quaternion->X;
	FLOAT num8 = quaternion->Y * quaternion->Y;
	FLOAT num7 = quaternion->Z * quaternion->Z;
	FLOAT num6 = quaternion->X * quaternion->Y;
	FLOAT num5 = quaternion->Z * quaternion->W;
	FLOAT num4 = quaternion->Z * quaternion->X;
	FLOAT num3 = quaternion->Y * quaternion->W;
	FLOAT num2 = quaternion->Y * quaternion->Z;
	FLOAT num = quaternion->X * quaternion->W;
	result->M11 = 1.0f - (2.0f * (num8 + num7));
	result->M12 = 2.0f * (num6 + num5);
	result->M13 = 2.0f * (num4 - num3);
	result->M14 = 0.0f;
	result->M21 = 2.0f * (num6 - num5);
	result->M22 = 1.0f - (2.0f * (num7 + num9));
	result->M23 = 2.0f * (num2 + num);
	result->M24 = 0.0f;
	result->M31 = 2.0f * (num4 + num3);
	result->M32 = 2.0f * (num2 - num);
	result->M33 = 1.0f - (2.0f * (num8 + num9));
	result->M34 = 0.0f;
	result->M41 = 0.0f;
	result->M42 = 0.0f;
	result->M43 = 0.0f;
	result->M44 = 1.0f;
};

void Matrix::CreateFromYawPitchRoll(FLOAT yaw, FLOAT pitch, FLOAT roll, Matrix *result)
{
	Quaternion quaternion;
	Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll, &quaternion);
	CreateFromQuaternion(&quaternion, result);
};

void Matrix::CreateShadow(const Vector3 *lightDirection,const Plane *plane, Matrix *result)
{
	Plane plane2;
	Plane::Normalize(plane, &plane2);
	FLOAT num = ((plane2.Normal.X * lightDirection->X) + (plane2.Normal.Y * lightDirection->Y)) + (plane2.Normal.Z * lightDirection->Z);
	FLOAT num5 = -plane2.Normal.X;
	FLOAT num4 = -plane2.Normal.Y;
	FLOAT num3 = -plane2.Normal.Z;
	FLOAT num2 = -plane2.D;
	result->M11 = (num5 * lightDirection->X) + num;
	result->M21 = num4 * lightDirection->X;
	result->M31 = num3 * lightDirection->X;
	result->M41 = num2 * lightDirection->X;
	result->M12 = num5 * lightDirection->Y;
	result->M22 = (num4 * lightDirection->Y) + num;
	result->M32 = num3 * lightDirection->Y;
	result->M42 = num2 * lightDirection->Y;
	result->M13 = num5 * lightDirection->Z;
	result->M23 = num4 * lightDirection->Z;
	result->M33 = (num3 * lightDirection->Z) + num;
	result->M43 = num2 * lightDirection->Z;
	result->M14 = 0.0f;
	result->M24 = 0.0f;
	result->M34 = 0.0f;
	result->M44 = num;
};

void Matrix::Transform(const Matrix *value, const Quaternion *rotation, Matrix *result)
{
	FLOAT num21 = rotation->X + rotation->X;
	FLOAT num11 = rotation->Y + rotation->Y;
	FLOAT num10 = rotation->Z + rotation->Z;
	FLOAT num20 = rotation->W * num21;
	FLOAT num19 = rotation->W * num11;
	FLOAT num18 = rotation->W * num10;
	FLOAT num17 = rotation->X * num21;
	FLOAT num16 = rotation->X * num11;
	FLOAT num15 = rotation->X * num10;
	FLOAT num14 = rotation->Y * num11;
	FLOAT num13 = rotation->Y * num10;
	FLOAT num12 = rotation->Z * num10;
	FLOAT num9 = (1.0f - num14) - num12;
	FLOAT num8 = num16 - num18;
	FLOAT num7 = num15 + num19;
	FLOAT num6 = num16 + num18;
	FLOAT num5 = (1.0f - num17) - num12;
	FLOAT num4 = num13 - num20;
	FLOAT num3 = num15 - num19;
	FLOAT num2 = num13 + num20;
	FLOAT num = (1.0f - num17) - num14;
	FLOAT num37 = ((value->M11 * num9) + (value->M12 * num8)) + (value->M13 * num7);
	FLOAT num36 = ((value->M11 * num6) + (value->M12 * num5)) + (value->M13 * num4);
	FLOAT num35 = ((value->M11 * num3) + (value->M12 * num2)) + (value->M13 * num);
	FLOAT num34 = value->M14;
	FLOAT num33 = ((value->M21 * num9) + (value->M22 * num8)) + (value->M23 * num7);
	FLOAT num32 = ((value->M21 * num6) + (value->M22 * num5)) + (value->M23 * num4);
	FLOAT num31 = ((value->M21 * num3) + (value->M22 * num2)) + (value->M23 * num);
	FLOAT num30 = value->M24;
	FLOAT num29 = ((value->M31 * num9) + (value->M32 * num8)) + (value->M33 * num7);
	FLOAT num28 = ((value->M31 * num6) + (value->M32 * num5)) + (value->M33 * num4);
	FLOAT num27 = ((value->M31 * num3) + (value->M32 * num2)) + (value->M33 * num);
	FLOAT num26 = value->M34;
	FLOAT num25 = ((value->M41 * num9) + (value->M42 * num8)) + (value->M43 * num7);
	FLOAT num24 = ((value->M41 * num6) + (value->M42 * num5)) + (value->M43 * num4);
	FLOAT num23 = ((value->M41 * num3) + (value->M42 * num2)) + (value->M43 * num);
	FLOAT num22 = value->M44;
	result->M11 = num37;
	result->M12 = num36;
	result->M13 = num35;
	result->M14 = num34;
	result->M21 = num33;
	result->M22 = num32;
	result->M23 = num31;
	result->M24 = num30;
	result->M31 = num29;
	result->M32 = num28;
	result->M33 = num27;
	result->M34 = num26;
	result->M41 = num25;
	result->M42 = num24;
	result->M43 = num23;
	result->M44 = num22;
};

void Matrix::Transpose(const Matrix *matrix, Matrix *result)
{
	FLOAT num16 = matrix->M11;
	FLOAT num15 = matrix->M12;
	FLOAT num14 = matrix->M13;
	FLOAT num13 = matrix->M14;
	FLOAT num12 = matrix->M21;
	FLOAT num11 = matrix->M22;
	FLOAT num10 = matrix->M23;
	FLOAT num9 = matrix->M24;
	FLOAT num8 = matrix->M31;
	FLOAT num7 = matrix->M32;
	FLOAT num6 = matrix->M33;
	FLOAT num5 = matrix->M34;
	FLOAT num4 = matrix->M41;
	FLOAT num3 = matrix->M42;
	FLOAT num2 = matrix->M43;
	FLOAT num = matrix->M44;
	result->M11 = num16;
	result->M12 = num12;
	result->M13 = num8;
	result->M14 = num4;
	result->M21 = num15;
	result->M22 = num11;
	result->M23 = num7;
	result->M24 = num3;
	result->M31 = num14;
	result->M32 = num10;
	result->M33 = num6;
	result->M34 = num2;
	result->M41 = num13;
	result->M42 = num9;
	result->M43 = num5;
	result->M44 = num;
};

FLOAT Matrix::Determinant(const Matrix *value)
{
	FLOAT num22 = value->M11;
	FLOAT num21 = value->M12;
	FLOAT num20 = value->M13;
	FLOAT num19 = value->M14;
	FLOAT num12 = value->M21;
	FLOAT num11 = value->M22;
	FLOAT num10 = value->M23;
	FLOAT num9 = value->M24;
	FLOAT num8 = value->M31;
	FLOAT num7 = value->M32;
	FLOAT num6 = value->M33;
	FLOAT num5 = value->M34;
	FLOAT num4 = value->M41;
	FLOAT num3 = value->M42;
	FLOAT num2 = value->M43;
	FLOAT num = value->M44;
	FLOAT num18 = (num6 * num) - (num5 * num2);
	FLOAT num17 = (num7 * num) - (num5 * num3);
	FLOAT num16 = (num7 * num2) - (num6 * num3);
	FLOAT num15 = (num8 * num) - (num5 * num4);
	FLOAT num14 = (num8 * num2) - (num6 * num4);
	FLOAT num13 = (num8 * num3) - (num7 * num4);
	return ((((num22 * (((num11 * num18) - (num10 * num17)) + (num9 * num16))) - (num21 * (((num12 * num18) - (num10 * num15)) + (num9 * num14)))) + (num20 * (((num12 * num17) - (num11 * num15)) + (num9 * num13)))) - (num19 * (((num12 * num16) - (num11 * num14)) + (num10 * num13))));
};

void Matrix::Invert(const Matrix *matrix, Matrix *result)
{
	FLOAT num5 = matrix->M11;
	FLOAT num4 = matrix->M12;
	FLOAT num3 = matrix->M13;
	FLOAT num2 = matrix->M14;
	FLOAT num9 = matrix->M21;
	FLOAT num8 = matrix->M22;
	FLOAT num7 = matrix->M23;
	FLOAT num6 = matrix->M24;
	FLOAT num17 = matrix->M31;
	FLOAT num16 = matrix->M32;
	FLOAT num15 = matrix->M33;
	FLOAT num14 = matrix->M34;
	FLOAT num13 = matrix->M41;
	FLOAT num12 = matrix->M42;
	FLOAT num11 = matrix->M43;
	FLOAT num10 = matrix->M44;
	FLOAT num23 = (num15 * num10) - (num14 * num11);
	FLOAT num22 = (num16 * num10) - (num14 * num12);
	FLOAT num21 = (num16 * num11) - (num15 * num12);
	FLOAT num20 = (num17 * num10) - (num14 * num13);
	FLOAT num19 = (num17 * num11) - (num15 * num13);
	FLOAT num18 = (num17 * num12) - (num16 * num13);
	FLOAT num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
	FLOAT num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
	FLOAT num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
	FLOAT num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
	FLOAT num = 1.0f / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));
	result->M11 = num39 * num;
	result->M21 = num38 * num;
	result->M31 = num37 * num;
	result->M41 = num36 * num;
	result->M12 = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
	result->M22 = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
	result->M32 = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
	result->M42 = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;
	FLOAT num35 = (num7 * num10) - (num6 * num11);
	FLOAT num34 = (num8 * num10) - (num6 * num12);
	FLOAT num33 = (num8 * num11) - (num7 * num12);
	FLOAT num32 = (num9 * num10) - (num6 * num13);
	FLOAT num31 = (num9 * num11) - (num7 * num13);
	FLOAT num30 = (num9 * num12) - (num8 * num13);
	result->M13 = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
	result->M23 = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
	result->M33 = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
	result->M43 = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;
	FLOAT num29 = (num7 * num14) - (num6 * num15);
	FLOAT num28 = (num8 * num14) - (num6 * num16);
	FLOAT num27 = (num8 * num15) - (num7 * num16);
	FLOAT num26 = (num9 * num14) - (num6 * num17);
	FLOAT num25 = (num9 * num15) - (num7 * num17);
	FLOAT num24 = (num9 * num16) - (num8 * num17);
	result->M14 = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
	result->M24 = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
	result->M34 = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
	result->M44 = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;
};

void Matrix::Lerp(const Matrix *matrix1, const Matrix *matrix2, FLOAT amount, Matrix *result)
{
	result->M11 = matrix1->M11 + ((matrix2->M11 - matrix1->M11) * amount);
	result->M12 = matrix1->M12 + ((matrix2->M12 - matrix1->M12) * amount);
	result->M13 = matrix1->M13 + ((matrix2->M13 - matrix1->M13) * amount);
	result->M14 = matrix1->M14 + ((matrix2->M14 - matrix1->M14) * amount);
	result->M21 = matrix1->M21 + ((matrix2->M21 - matrix1->M21) * amount);
	result->M22 = matrix1->M22 + ((matrix2->M22 - matrix1->M22) * amount);
	result->M23 = matrix1->M23 + ((matrix2->M23 - matrix1->M23) * amount);
	result->M24 = matrix1->M24 + ((matrix2->M24 - matrix1->M24) * amount);
	result->M31 = matrix1->M31 + ((matrix2->M31 - matrix1->M31) * amount);
	result->M32 = matrix1->M32 + ((matrix2->M32 - matrix1->M32) * amount);
	result->M33 = matrix1->M33 + ((matrix2->M33 - matrix1->M33) * amount);
	result->M34 = matrix1->M34 + ((matrix2->M34 - matrix1->M34) * amount);
	result->M41 = matrix1->M41 + ((matrix2->M41 - matrix1->M41) * amount);
	result->M42 = matrix1->M42 + ((matrix2->M42 - matrix1->M42) * amount);
	result->M43 = matrix1->M43 + ((matrix2->M43 - matrix1->M43) * amount);
	result->M44 = matrix1->M44 + ((matrix2->M44 - matrix1->M44) * amount);
};

void Matrix::Negate(const Matrix *matrix, Matrix *result)
{
	result->M11 = -matrix->M11;
	result->M12 = -matrix->M12;
	result->M13 = -matrix->M13;
	result->M14 = -matrix->M14;
	result->M21 = -matrix->M21;
	result->M22 = -matrix->M22;
	result->M23 = -matrix->M23;
	result->M24 = -matrix->M24;
	result->M31 = -matrix->M31;
	result->M32 = -matrix->M32;
	result->M33 = -matrix->M33;
	result->M34 = -matrix->M34;
	result->M41 = -matrix->M41;
	result->M42 = -matrix->M42;
	result->M43 = -matrix->M43;
	result->M44 = -matrix->M44;
};

void Matrix::Add(const Matrix *matrix1, const Matrix *matrix2, Matrix *result)
{
	result->M11 = matrix1->M11 + matrix2->M11;
	result->M12 = matrix1->M12 + matrix2->M12;
	result->M13 = matrix1->M13 + matrix2->M13;
	result->M14 = matrix1->M14 + matrix2->M14;
	result->M21 = matrix1->M21 + matrix2->M21;
	result->M22 = matrix1->M22 + matrix2->M22;
	result->M23 = matrix1->M23 + matrix2->M23;
	result->M24 = matrix1->M24 + matrix2->M24;
	result->M31 = matrix1->M31 + matrix2->M31;
	result->M32 = matrix1->M32 + matrix2->M32;
	result->M33 = matrix1->M33 + matrix2->M33;
	result->M34 = matrix1->M34 + matrix2->M34;
	result->M41 = matrix1->M41 + matrix2->M41;
	result->M42 = matrix1->M42 + matrix2->M42;
	result->M43 = matrix1->M43 + matrix2->M43;
	result->M44 = matrix1->M44 + matrix2->M44;
};

void Matrix::Subtract(const Matrix *matrix1, const Matrix *matrix2, Matrix *result)
{
	result->M11 = matrix1->M11 - matrix2->M11;
	result->M12 = matrix1->M12 - matrix2->M12;
	result->M13 = matrix1->M13 - matrix2->M13;
	result->M14 = matrix1->M14 - matrix2->M14;
	result->M21 = matrix1->M21 - matrix2->M21;
	result->M22 = matrix1->M22 - matrix2->M22;
	result->M23 = matrix1->M23 - matrix2->M23;
	result->M24 = matrix1->M24 - matrix2->M24;
	result->M31 = matrix1->M31 - matrix2->M31;
	result->M32 = matrix1->M32 - matrix2->M32;
	result->M33 = matrix1->M33 - matrix2->M33;
	result->M34 = matrix1->M34 - matrix2->M34;
	result->M41 = matrix1->M41 - matrix2->M41;
	result->M42 = matrix1->M42 - matrix2->M42;
	result->M43 = matrix1->M43 - matrix2->M43;
	result->M44 = matrix1->M44 - matrix2->M44;
};

void Matrix::Multiply(const Matrix *matrix1, const Matrix *matrix2, Matrix *result)
{
	FLOAT num16 = (((matrix1->M11 * matrix2->M11) + (matrix1->M12 * matrix2->M21)) + (matrix1->M13 * matrix2->M31)) + (matrix1->M14 * matrix2->M41);
	FLOAT num15 = (((matrix1->M11 * matrix2->M12) + (matrix1->M12 * matrix2->M22)) + (matrix1->M13 * matrix2->M32)) + (matrix1->M14 * matrix2->M42);
	FLOAT num14 = (((matrix1->M11 * matrix2->M13) + (matrix1->M12 * matrix2->M23)) + (matrix1->M13 * matrix2->M33)) + (matrix1->M14 * matrix2->M43);
	FLOAT num13 = (((matrix1->M11 * matrix2->M14) + (matrix1->M12 * matrix2->M24)) + (matrix1->M13 * matrix2->M34)) + (matrix1->M14 * matrix2->M44);
	FLOAT num12 = (((matrix1->M21 * matrix2->M11) + (matrix1->M22 * matrix2->M21)) + (matrix1->M23 * matrix2->M31)) + (matrix1->M24 * matrix2->M41);
	FLOAT num11 = (((matrix1->M21 * matrix2->M12) + (matrix1->M22 * matrix2->M22)) + (matrix1->M23 * matrix2->M32)) + (matrix1->M24 * matrix2->M42);
	FLOAT num10 = (((matrix1->M21 * matrix2->M13) + (matrix1->M22 * matrix2->M23)) + (matrix1->M23 * matrix2->M33)) + (matrix1->M24 * matrix2->M43);
	FLOAT num9 = (((matrix1->M21 * matrix2->M14) + (matrix1->M22 * matrix2->M24)) + (matrix1->M23 * matrix2->M34)) + (matrix1->M24 * matrix2->M44);
	FLOAT num8 = (((matrix1->M31 * matrix2->M11) + (matrix1->M32 * matrix2->M21)) + (matrix1->M33 * matrix2->M31)) + (matrix1->M34 * matrix2->M41);
	FLOAT num7 = (((matrix1->M31 * matrix2->M12) + (matrix1->M32 * matrix2->M22)) + (matrix1->M33 * matrix2->M32)) + (matrix1->M34 * matrix2->M42);
	FLOAT num6 = (((matrix1->M31 * matrix2->M13) + (matrix1->M32 * matrix2->M23)) + (matrix1->M33 * matrix2->M33)) + (matrix1->M34 * matrix2->M43);
	FLOAT num5 = (((matrix1->M31 * matrix2->M14) + (matrix1->M32 * matrix2->M24)) + (matrix1->M33 * matrix2->M34)) + (matrix1->M34 * matrix2->M44);
	FLOAT num4 = (((matrix1->M41 * matrix2->M11) + (matrix1->M42 * matrix2->M21)) + (matrix1->M43 * matrix2->M31)) + (matrix1->M44 * matrix2->M41);
	FLOAT num3 = (((matrix1->M41 * matrix2->M12) + (matrix1->M42 * matrix2->M22)) + (matrix1->M43 * matrix2->M32)) + (matrix1->M44 * matrix2->M42);
	FLOAT num2 = (((matrix1->M41 * matrix2->M13) + (matrix1->M42 * matrix2->M23)) + (matrix1->M43 * matrix2->M33)) + (matrix1->M44 * matrix2->M43);
	FLOAT num = (((matrix1->M41 * matrix2->M14) + (matrix1->M42 * matrix2->M24)) + (matrix1->M43 * matrix2->M34)) + (matrix1->M44 * matrix2->M44);
	result->M11 = num16;
	result->M12 = num15;
	result->M13 = num14;
	result->M14 = num13;
	result->M21 = num12;
	result->M22 = num11;
	result->M23 = num10;
	result->M24 = num9;
	result->M31 = num8;
	result->M32 = num7;
	result->M33 = num6;
	result->M34 = num5;
	result->M41 = num4;
	result->M42 = num3;
	result->M43 = num2;
	result->M44 = num;
};

void Matrix::Multiply(const Matrix *matrix1, FLOAT scaleFactor, Matrix *result)
{
	FLOAT num = scaleFactor;
	result->M11 = matrix1->M11 * num;
	result->M12 = matrix1->M12 * num;
	result->M13 = matrix1->M13 * num;
	result->M14 = matrix1->M14 * num;
	result->M21 = matrix1->M21 * num;
	result->M22 = matrix1->M22 * num;
	result->M23 = matrix1->M23 * num;
	result->M24 = matrix1->M24 * num;
	result->M31 = matrix1->M31 * num;
	result->M32 = matrix1->M32 * num;
	result->M33 = matrix1->M33 * num;
	result->M34 = matrix1->M34 * num;
	result->M41 = matrix1->M41 * num;
	result->M42 = matrix1->M42 * num;
	result->M43 = matrix1->M43 * num;
	result->M44 = matrix1->M44 * num;
};

void Matrix::Divide(const Matrix *matrix1, const Matrix *matrix2, Matrix *result)
{
	result->M11 = matrix1->M11 / matrix2->M11;
	result->M12 = matrix1->M12 / matrix2->M12;
	result->M13 = matrix1->M13 / matrix2->M13;
	result->M14 = matrix1->M14 / matrix2->M14;
	result->M21 = matrix1->M21 / matrix2->M21;
	result->M22 = matrix1->M22 / matrix2->M22;
	result->M23 = matrix1->M23 / matrix2->M23;
	result->M24 = matrix1->M24 / matrix2->M24;
	result->M31 = matrix1->M31 / matrix2->M31;
	result->M32 = matrix1->M32 / matrix2->M32;
	result->M33 = matrix1->M33 / matrix2->M33;
	result->M34 = matrix1->M34 / matrix2->M34;
	result->M41 = matrix1->M41 / matrix2->M41;
	result->M42 = matrix1->M42 / matrix2->M42;
	result->M43 = matrix1->M43 / matrix2->M43;
	result->M44 = matrix1->M44 / matrix2->M44;
};

void Matrix::Divide(const Matrix *matrix1, FLOAT divider, Matrix *result)
{
	FLOAT num = 1.0f / divider;
	result->M11 = matrix1->M11 * num;
	result->M12 = matrix1->M12 * num;
	result->M13 = matrix1->M13 * num;
	result->M14 = matrix1->M14 * num;
	result->M21 = matrix1->M21 * num;
	result->M22 = matrix1->M22 * num;
	result->M23 = matrix1->M23 * num;
	result->M24 = matrix1->M24 * num;
	result->M31 = matrix1->M31 * num;
	result->M32 = matrix1->M32 * num;
	result->M33 = matrix1->M33 * num;
	result->M34 = matrix1->M34 * num;
	result->M41 = matrix1->M41 * num;
	result->M42 = matrix1->M42 * num;
	result->M43 = matrix1->M43 * num;
	result->M44 = matrix1->M44 * num;
};

void Matrix::Unproject(Vector3 *source, const Matrix *projection, const Matrix *view, const Matrix *world, INT viewPortWidth, INT viewPortHeight, FLOAT nearPlane, FLOAT farPlane, Vector3 *result)
{
	FLOAT X = 0;
	FLOAT Y = 0;

	Matrix finalMatrix;
	Matrix::Multiply(world, view, &finalMatrix);
	Matrix::Multiply(&finalMatrix, projection, &finalMatrix);
	Matrix::Invert(&finalMatrix, &finalMatrix);

	source->X = (((source->X - X) / ((FLOAT)viewPortWidth)) * 2.0f) - 1.0f;
	source->Y = -((((source->Y - Y) / ((FLOAT)viewPortHeight)) * 2.0f) - 1.0f);
	source->Z = (source->Z - nearPlane) / (farPlane - nearPlane);

	Vector3::Transform(source, &finalMatrix, result);

	FLOAT a = (((source->X * finalMatrix.M14) + (source->Y * finalMatrix.M24)) + (source->Z * finalMatrix.M34)) + finalMatrix.M44;
	if (!GenericTools::WithinEpsilon(a, 1.0f))
		Vector3::Divide(result, a, result);
};

void Matrix::Project(const Vector3 *source, const Matrix *projection, const Matrix *view, const Matrix *world, INT viewPortWidth, INT viewPortHeight, FLOAT nearPlane, FLOAT farPlane, Vector3 *result)
{
	FLOAT X = 0;
	FLOAT Y = 0;

	Matrix finalMatrix;
	Matrix::Multiply(world, view, &finalMatrix);
	Matrix::Multiply(&finalMatrix, projection, &finalMatrix);

	Vector3::Transform(source, &finalMatrix, result);

	FLOAT a = (((source->X * finalMatrix.M14) + (source->Y * finalMatrix.M24)) + (source->Z * finalMatrix.M34)) + finalMatrix.M44;
	if (!GenericTools::WithinEpsilon(a, 1.0f))
		Vector3::Divide(result, a, result);

	result->X = (((result->X + 1.0f) * 0.5f) * viewPortWidth) + X;
	result->Y = (((-result->Y + 1.0f) * 0.5f) * viewPortHeight) + Y;
	result->Z = (result->Z * (farPlane - nearPlane)) + nearPlane;
};

void Matrix::Copy(const Matrix *source, Matrix *destination)
{
	destination->M11 = source->M11;
	destination->M12 = source->M12;
	destination->M13 = source->M13;
	destination->M14 = source->M14;

	destination->M21 = source->M21;
	destination->M22 = source->M22;
	destination->M23 = source->M23;
	destination->M24 = source->M24;

	destination->M31 = source->M31;
	destination->M32 = source->M32;
	destination->M33 = source->M33;
	destination->M34 = source->M34;

	destination->M41 = source->M41;
	destination->M42 = source->M42;
	destination->M43 = source->M43;
	destination->M44 = source->M44;
};


Matrix Matrix::operator -() const
{
	Matrix matrix;
    matrix.M11 = -this->M11;
    matrix.M12 = -this->M12;
    matrix.M13 = -this->M13;
    matrix.M14 = -this->M14;
    matrix.M21 = -this->M21;
    matrix.M22 = -this->M22;
    matrix.M23 = -this->M23;
    matrix.M24 = -this->M24;
    matrix.M31 = -this->M31;
    matrix.M32 = -this->M32;
    matrix.M33 = -this->M33;
    matrix.M34 = -this->M34;
    matrix.M41 = -this->M41;
    matrix.M42 = -this->M42;
    matrix.M43 = -this->M43;
    matrix.M44 = -this->M44;
    return matrix;
};

BOOL Matrix::operator ==(const Matrix matrix2) const
{
	return ((((((this->M11 == matrix2.M11) && (this->M22 == matrix2.M22)) && ((this->M33 == matrix2.M33) && (this->M44 == matrix2.M44))) && (((this->M12 == matrix2.M12) && (this->M13 == matrix2.M13)) && ((this->M14 == matrix2.M14) && (this->M21 == matrix2.M21)))) && ((((this->M23 == matrix2.M23) && (this->M24 == matrix2.M24)) && ((this->M31 == matrix2.M31) && (this->M32 == matrix2.M32))) && (((this->M34 == matrix2.M34) && (this->M41 == matrix2.M41)) && (this->M42 == matrix2.M42)))) && (this->M43 == matrix2.M43));
};

BOOL Matrix::operator !=(const Matrix matrix2) const
{
    if (((((this->M11 == matrix2.M11) && (this->M12 == matrix2.M12)) && ((this->M13 == matrix2.M13) && (this->M14 == matrix2.M14))) && (((this->M21 == matrix2.M21) && (this->M22 == matrix2.M22)) && ((this->M23 == matrix2.M23) && (this->M24 == matrix2.M24)))) && ((((this->M31 == matrix2.M31) && (this->M32 == matrix2.M32)) && ((this->M33 == matrix2.M33) && (this->M34 == matrix2.M34))) && (((this->M41 == matrix2.M41) && (this->M42 == matrix2.M42)) && (this->M43 == matrix2.M43))))
	{
		return (this->M44 != matrix2.M44);
	}
	return true;
};

Matrix Matrix::operator +(const Matrix matrix2) const
{
	Matrix matrix;
    matrix.M11 = this->M11 + matrix2.M11;
    matrix.M12 = this->M12 + matrix2.M12;
    matrix.M13 = this->M13 + matrix2.M13;
    matrix.M14 = this->M14 + matrix2.M14;
    matrix.M21 = this->M21 + matrix2.M21;
    matrix.M22 = this->M22 + matrix2.M22;
    matrix.M23 = this->M23 + matrix2.M23;
    matrix.M24 = this->M24 + matrix2.M24;
    matrix.M31 = this->M31 + matrix2.M31;
    matrix.M32 = this->M32 + matrix2.M32;
    matrix.M33 = this->M33 + matrix2.M33;
    matrix.M34 = this->M34 + matrix2.M34;
    matrix.M41 = this->M41 + matrix2.M41;
    matrix.M42 = this->M42 + matrix2.M42;
    matrix.M43 = this->M43 + matrix2.M43;
    matrix.M44 = this->M44 + matrix2.M44;
    return matrix;
};

Matrix Matrix::operator -(const Matrix matrix2) const
{
	Matrix matrix;
    matrix.M11 = this->M11 - matrix2.M11;
    matrix.M12 = this->M12 - matrix2.M12;
    matrix.M13 = this->M13 - matrix2.M13;
    matrix.M14 = this->M14 - matrix2.M14;
    matrix.M21 = this->M21 - matrix2.M21;
    matrix.M22 = this->M22 - matrix2.M22;
    matrix.M23 = this->M23 - matrix2.M23;
    matrix.M24 = this->M24 - matrix2.M24;
    matrix.M31 = this->M31 - matrix2.M31;
    matrix.M32 = this->M32 - matrix2.M32;
    matrix.M33 = this->M33 - matrix2.M33;
    matrix.M34 = this->M34 - matrix2.M34;
    matrix.M41 = this->M41 - matrix2.M41;
    matrix.M42 = this->M42 - matrix2.M42;
    matrix.M43 = this->M43 - matrix2.M43;
    matrix.M44 = this->M44 - matrix2.M44;
    return matrix;
};

Matrix Matrix::operator *(const Matrix matrix2) const
{
    Matrix matrix;
    matrix.M11 = (((this->M11 * matrix2.M11) + (this->M12 * matrix2.M21)) + (this->M13 * matrix2.M31)) + (this->M14 * matrix2.M41);
    matrix.M12 = (((this->M11 * matrix2.M12) + (this->M12 * matrix2.M22)) + (this->M13 * matrix2.M32)) + (this->M14 * matrix2.M42);
    matrix.M13 = (((this->M11 * matrix2.M13) + (this->M12 * matrix2.M23)) + (this->M13 * matrix2.M33)) + (this->M14 * matrix2.M43);
    matrix.M14 = (((this->M11 * matrix2.M14) + (this->M12 * matrix2.M24)) + (this->M13 * matrix2.M34)) + (this->M14 * matrix2.M44);
    matrix.M21 = (((this->M21 * matrix2.M11) + (this->M22 * matrix2.M21)) + (this->M23 * matrix2.M31)) + (this->M24 * matrix2.M41);
    matrix.M22 = (((this->M21 * matrix2.M12) + (this->M22 * matrix2.M22)) + (this->M23 * matrix2.M32)) + (this->M24 * matrix2.M42);
    matrix.M23 = (((this->M21 * matrix2.M13) + (this->M22 * matrix2.M23)) + (this->M23 * matrix2.M33)) + (this->M24 * matrix2.M43);
    matrix.M24 = (((this->M21 * matrix2.M14) + (this->M22 * matrix2.M24)) + (this->M23 * matrix2.M34)) + (this->M24 * matrix2.M44);
    matrix.M31 = (((this->M31 * matrix2.M11) + (this->M32 * matrix2.M21)) + (this->M33 * matrix2.M31)) + (this->M34 * matrix2.M41);
    matrix.M32 = (((this->M31 * matrix2.M12) + (this->M32 * matrix2.M22)) + (this->M33 * matrix2.M32)) + (this->M34 * matrix2.M42);
    matrix.M33 = (((this->M31 * matrix2.M13) + (this->M32 * matrix2.M23)) + (this->M33 * matrix2.M33)) + (this->M34 * matrix2.M43);
    matrix.M34 = (((this->M31 * matrix2.M14) + (this->M32 * matrix2.M24)) + (this->M33 * matrix2.M34)) + (this->M34 * matrix2.M44);
    matrix.M41 = (((this->M41 * matrix2.M11) + (this->M42 * matrix2.M21)) + (this->M43 * matrix2.M31)) + (this->M44 * matrix2.M41);
    matrix.M42 = (((this->M41 * matrix2.M12) + (this->M42 * matrix2.M22)) + (this->M43 * matrix2.M32)) + (this->M44 * matrix2.M42);
    matrix.M43 = (((this->M41 * matrix2.M13) + (this->M42 * matrix2.M23)) + (this->M43 * matrix2.M33)) + (this->M44 * matrix2.M43);
    matrix.M44 = (((this->M41 * matrix2.M14) + (this->M42 * matrix2.M24)) + (this->M43 * matrix2.M34)) + (this->M44 * matrix2.M44);
    return matrix;
};

Matrix Matrix::operator *(float scaleFactor) const
{
    Matrix matrix2;
    float num = scaleFactor;
    matrix2.M11 = this->M11 * num;
    matrix2.M12 = this->M12 * num;
    matrix2.M13 = this->M13 * num;
    matrix2.M14 = this->M14 * num;
    matrix2.M21 = this->M21 * num;
    matrix2.M22 = this->M22 * num;
    matrix2.M23 = this->M23 * num;
    matrix2.M24 = this->M24 * num;
    matrix2.M31 = this->M31 * num;
    matrix2.M32 = this->M32 * num;
    matrix2.M33 = this->M33 * num;
    matrix2.M34 = this->M34 * num;
    matrix2.M41 = this->M41 * num;
    matrix2.M42 = this->M42 * num;
    matrix2.M43 = this->M43 * num;
    matrix2.M44 = this->M44 * num;
    return matrix2;
};

Matrix Matrix::operator /(const Matrix matrix2) const
{
    Matrix matrix;
    matrix.M11 = this->M11 / matrix2.M11;
    matrix.M12 = this->M12 / matrix2.M12;
    matrix.M13 = this->M13 / matrix2.M13;
    matrix.M14 = this->M14 / matrix2.M14;
    matrix.M21 = this->M21 / matrix2.M21;
    matrix.M22 = this->M22 / matrix2.M22;
    matrix.M23 = this->M23 / matrix2.M23;
    matrix.M24 = this->M24 / matrix2.M24;
    matrix.M31 = this->M31 / matrix2.M31;
    matrix.M32 = this->M32 / matrix2.M32;
    matrix.M33 = this->M33 / matrix2.M33;
    matrix.M34 = this->M34 / matrix2.M34;
    matrix.M41 = this->M41 / matrix2.M41;
    matrix.M42 = this->M42 / matrix2.M42;
    matrix.M43 = this->M43 / matrix2.M43;
    matrix.M44 = this->M44 / matrix2.M44;
    return matrix;
};

Matrix Matrix::operator /(float divider) const
{
    Matrix matrix;
    float num = 1.0f / divider;
    matrix.M11 = this->M11 * num;
    matrix.M12 = this->M12 * num;
    matrix.M13 = this->M13 * num;
    matrix.M14 = this->M14 * num;
    matrix.M21 = this->M21 * num;
    matrix.M22 = this->M22 * num;
    matrix.M23 = this->M23 * num;
    matrix.M24 = this->M24 * num;
    matrix.M31 = this->M31 * num;
    matrix.M32 = this->M32 * num;
    matrix.M33 = this->M33 * num;
    matrix.M34 = this->M34 * num;
    matrix.M41 = this->M41 * num;
    matrix.M42 = this->M42 * num;
    matrix.M43 = this->M43 * num;
    matrix.M44 = this->M44 * num;
    return matrix;
};
