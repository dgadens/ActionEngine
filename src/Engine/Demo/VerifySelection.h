//*****************************
// Verifica a selecao dos itens
// creator: Daniel Gadens
// date: 19/07/2011
//******************************

#ifndef __VERIFYSELECTION_H
#define __VERIFYSELECTION_H

#include "ACCamera.h"
#include "EngineMath.h"
#include "ACModel.h"

class VerifySelection
{
public:

	static void Process(ACModel* model, Ray* ray);
};

#endif