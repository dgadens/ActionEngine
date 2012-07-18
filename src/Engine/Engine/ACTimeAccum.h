//*************************
// Acumulador de tempo, trabalha exclusivamente com millisegundos
// creator : Daniel Gadens
// date: 18/05/2011
//****************************


#ifndef __ACTIMEACCUM_H
#define __ACTIMEACCUM_H

#include "EngineCommon.h"

class ACTimeAccum
{
public:
    INT AccumulatedTime;
    INT LimitTime;

    ACTimeAccum(INT limitTime);

	void Update(INT elapsedTime);
	BOOL End();
	void Restart();
    void Reset();
};

#endif