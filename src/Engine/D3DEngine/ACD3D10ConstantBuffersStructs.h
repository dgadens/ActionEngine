//*********************************
// Estruturas de constant buffer usados no directx 10 -11
// Creator: Daniel Gadens
// Date: 29/05/2012
//*********************************

#ifndef __ACD3D10CONSTANTBUFFERSTRUCT_H
#define __ACD3D10CONSTANTBUFFERSTRUCT_H

#include "EngineMath.h"

//constant budder do VS 
struct VS_CB_PERFRAME
{
    __declspec(align(16)) Vector3   xCameraPosition;
	__declspec(align(16)) Vector3	xLightPosition;
	__declspec(align(16)) Vector3	xLightDirection;
	__declspec(align(16)) Vector3	xLightColor;
	float							xLightRange;
	float							xLightFalloff;
	float							xTime;
};

struct VS_CB_PERMODEL
{
	Matrix xWorld;
};

struct VS_CB_PERPASS
{
	Matrix xViewProjection;
};

//constant buffer do PS para os materiais
struct PS_CB_MATERIAL
{
	__declspec(align(16)) Vector3 xMaterialAmbient;
	__declspec(align(16)) Vector3 xMaterialEmissive;
	__declspec(align(16)) Vector4 xMaterialDiffuse;
	Vector3						  xMaterialSpecular;
	float						  xMaterialSpecularPower;
};

#endif