
#ifndef __ACRANDON_H
#define __ACRANDON_H

#include <cstdlib>

class ACRandom
{
public:
	static void Randomize(UINT seed);
	static int NextInt();
	static int NextInt(int min, int max);
	static float NextFloat();
	static float NextFloat(float min, float max);
};

inline void ACRandom::Randomize(UINT seed)
{
	srand(seed);
};

inline int ACRandom::NextInt()
{
	return rand();
};

inline int ACRandom::NextInt(int min, int max)
{
	int range=(max-min)+1; 
	return min + int(range*rand()/(RAND_MAX + 1.0)); 
};

inline float ACRandom::NextFloat()
{
	float v = (float)NextInt() / RAND_MAX;

	return v;
};

inline float ACRandom::NextFloat(float min, float max)
{	
	float v = NextFloat();

	return min + v * (max-min);
};

#endif