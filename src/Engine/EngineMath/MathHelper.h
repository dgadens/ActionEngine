
#ifndef __MATHHELPER_H
#define __MATHHELPER_H

#include "EngineCommon.h"
#include <math.h>

struct Vector3;
struct Quaternion;
struct Plane;

static const FLOAT E = 2.718282f;
static const FLOAT LOG10E = 0.4342945f;
static const FLOAT LOG2E = 1.442695f;
static const FLOAT PI = 3.141593f;
static const FLOAT PIOVER2 = 1.570796f;
static const FLOAT PIOVER4 = 0.7853982f;
static const FLOAT TWOPI = 6.283185f;

struct MathHelper
{
    // Fields
public:

        // Methods
    static FLOAT Barycentric(FLOAT value1, FLOAT value2, FLOAT value3, FLOAT amount1, FLOAT amount2)
    {
        return ((value1 + (amount1 * (value2 - value1))) + (amount2 * (value3 - value1)));
    };

    static FLOAT CatmullRom(FLOAT value1, FLOAT value2, FLOAT value3, FLOAT value4, FLOAT amount)
    {
        FLOAT num = amount * amount;
        FLOAT num2 = amount * num;
        return (0.5f * ((((2.0f * value2) + ((-value1 + value3) * amount)) + (((((2.0f * value1) - (5.0f * value2)) + (4.0f * value3)) - value4) * num)) + ((((-value1 + (3.0f * value2)) - (3.0f * value3)) + value4) * num2)));
    };

    static FLOAT Clamp(FLOAT value, FLOAT min, FLOAT max)
    {
        value = (value > max) ? max : value;
        value = (value < min) ? min : value;
        return value;
    }

    static FLOAT Distance(FLOAT value1, FLOAT value2)
    {
        return abs((FLOAT)(value1 - value2));
    }

    static FLOAT Hermite(FLOAT value1, FLOAT tangent1, FLOAT value2, FLOAT tangent2, FLOAT amount)
    {
        FLOAT num3 = amount;
        FLOAT num = num3 * num3;
        FLOAT num2 = num3 * num;
        FLOAT num7 = ((2.0f * num2) - (3.0f * num)) + 1.0f;
        FLOAT num6 = (-2.0f * num2) + (3.0f * num);
        FLOAT num5 = (num2 - (2.0f * num)) + num3;
        FLOAT num4 = num2 - num;
        return ((((value1 * num7) + (value2 * num6)) + (tangent1 * num5)) + (tangent2 * num4));
    }

    static FLOAT Lerp(FLOAT value1, FLOAT value2, FLOAT amount)
    {
        return (value1 + ((value2 - value1) * amount));
    }

    static FLOAT Max(FLOAT value1, FLOAT value2)
    {
        return GenericTools::Max(value1, value2);
    }

    static FLOAT Min(FLOAT value1, FLOAT value2)
    {
        return GenericTools::Min(value1, value2);
    }

    static FLOAT SmoothStep(FLOAT value1, FLOAT value2, FLOAT amount)
    {
        FLOAT num = Clamp(amount, 0.0f, 1.0f);
        return Lerp(value1, value2, (num * num) * (3.0f - (2.0f * num)));
    }

    static FLOAT ToDegrees(FLOAT radians)
    {
        return (radians * 57.29578f);
    }

    static FLOAT ToRadians(FLOAT degrees)
    {
        return (degrees * 0.01745329f);
    }

    static FLOAT WrapAngle(FLOAT angle)
    {
        angle = GenericTools::IEEERemainder(angle, 6.283185f);
        if (angle <= -3.141593f)
        {
            angle += 6.283185f;
            return angle;
        }
        if (angle > 3.141593f)
        {
            angle -= 6.283185f;
        }
        return angle;
    }

	static BOOL EqualValue(FLOAT value1, FLOAT value2, FLOAT tolerancia)
    {
        return (
                ((value1 >= (value2 - tolerancia)) && (value1 <= (value2 + tolerancia)))
                ||
                ((value2 >= (value1 - tolerancia)) && (value2 <= (value1 + tolerancia)))
                );
    }

	static BOOL EqualValue(FLOAT value1, FLOAT value2)
    {
        return EqualValue(value1, value2, 0.001f);
    }

    static FLOAT TwoPointsAngle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, BOOL evaluateNearZeroValues)
    {
        FLOAT anguloRadiano;

        FLOAT diffX = x2 - x1;
        FLOAT diffY = y2 - y1;
        if (EqualValue(diffX, 0, 0.01f)) // Diferenca em X muito proxima de 0 angulo igual a 90 ou 270
        {
            anguloRadiano = ((y2 > y1) ? MathHelper::ToDegrees(90) : MathHelper::ToDegrees(270));
        }
        else
        {
            anguloRadiano = atan(diffY / diffX);
            if (diffX < 0)
                anguloRadiano += PI;

            if (!evaluateNearZeroValues && EqualValue(anguloRadiano, 0))
                anguloRadiano = 0;
        }


        return anguloRadiano;
    }

	static FLOAT GetNorma(Vector3 *v)
    {
        FLOAT norma = sqrt((v->X * v->X) + (v->Y * v->Y) + (v->Z * v->Z));
        
		return norma;
    }

    static FLOAT GetAngle(Vector3 *origem, Vector3 *p1, Vector3 *p2)
    {
        FLOAT dotProduct = Vector3::Dot(p1, p2);
        FLOAT x = dotProduct / (GetNorma(p1) * GetNorma(p2));

        //cos(q) = x, como o objetivo é achar o angulo q, temos que 
        // q = arcocosseno(x), o valor obtido será em radianos
        FLOAT q = acos(x);

        //retorno do valor do angulo
        return q;
    }

    static float GetAngle(Vector3 *p1, Vector3 *p2)
    {
        FLOAT angleX = p2->X - p1->X;
        FLOAT angleY = p2->Y - p1->Y;

        float angle = atan2(angleY, angleX);
        angle += MathHelper::ToRadians(90);

        return angle;
    }


    static void GetVersor(Vector3 *v, Vector3 *result)
    {
        FLOAT modulo = GetNorma(v);

        if (modulo == 0)
			result->X = result->Y = result->Z = 0;
        else
		{
			result->X = v->X / modulo;
			result->Y = v->Y / modulo;
			result->Z = v->Z / modulo;
		}
    }

    static void GetNormal(Vector3 *p1, Vector3 *p2, Vector3 *p3, Vector3 *result)
    {
        Vector3 v1;
		Vector3::Subtract(p2, p1, &v1);
        Vector3 v2;
		Vector3::Subtract(p3, p2, &v2);

        Vector3::Cross(&v1, &v2, result);
		Vector3::Normalize(result, result);
    }

};

#endif