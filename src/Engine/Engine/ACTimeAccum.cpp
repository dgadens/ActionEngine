#include "ACTimeAccum.h"

ACTimeAccum::ACTimeAccum(INT limitTime)
{
    AccumulatedTime = 0;
    LimitTime = limitTime;
};

void ACTimeAccum::Update(INT elapsedTime)
{
    AccumulatedTime += elapsedTime;
};

BOOL ACTimeAccum::End()
{
    return AccumulatedTime >= LimitTime;
};
    
void ACTimeAccum::Restart()
{
    AccumulatedTime %= LimitTime;
};

void ACTimeAccum::Reset()
{
    AccumulatedTime = 0;
};
