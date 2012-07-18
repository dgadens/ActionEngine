//*********************************************
//Implementacao de um algoritmo Gilbert-Johnson-Keerthi para deteccao de colisao entre objetos convexos
//*********************************************

#ifndef __GJK_H
#define __GJK_H

#include "EngineCommon.h"

struct Vector3;

struct Gjk
{
    // Fields
private:
	static int BitsToIndices[];

    Vector3 *closestPoint;
    FLOAT *det[0X10];

    FLOAT edgeLengthSq[4][4];
    Vector3 **edges; 
    FLOAT maxLengthSq;
    INT simplexBits;
    Vector3 *y;
    FLOAT yLengthSq[4];

	void ComputeClosestPoint(Vector3 *result);
	BOOL IsSatisfiesRule(INT xBits, INT yBits);
	void UpdateDeterminant(INT xmIdx);
	BOOL UpdateSimplex(INT newIndex);

public:

	Gjk();
	~Gjk();

	BOOL AddSupportPoint(Vector3 *newPoint);
	void Reset();

	// Properties
	Vector3 *ClosestPoint(){ return closestPoint; };
	BOOL FullSimplex(){ return (this->simplexBits == 15); };
    FLOAT MaxLengthSquared() { return maxLengthSq; };
};

#endif