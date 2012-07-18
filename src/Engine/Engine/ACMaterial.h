//************************************
// Estrutura o material
// creator: Daniel Gadens
// date: 01/04/2012
//**********************************

#ifndef __ACMATERIAL_H
#define __ACMATERIAL_H

#include "EngineMath.h"

struct ACMaterial
{
	Vector3      AmbientColor;
	Vector4      DiffuseColor;
	Vector3      SpecularColor;
	Vector3      EmissiveColor;
	float        SpecularPower;
};

#endif
