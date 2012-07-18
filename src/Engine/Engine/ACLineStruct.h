//***************************
// Estrutura que mantem os dados da linhas
// Creator: Daniel Gadens
// date: 16/06/2011
//***************************


#ifndef __ACLINESTRUCT_H
#define __ACLINESTRUCT_H

#include "EngineMath.h"

struct ACLineStruct
{
public:
	Vector3 StartPoint;
	Vector3 EndPoint;
	Vector4 StartColor;
	Vector4 EndColor;

	ACLineStruct(Vector3 startPoint, Vector4 startColor, Vector3 endPoint, Vector4 endColor); 
	void SetColor(Vector4 color);

	BOOL operator==(const ACLineStruct value2) const;
    BOOL operator!=(const ACLineStruct value2) const;

};

#endif