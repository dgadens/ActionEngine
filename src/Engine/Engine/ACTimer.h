
#ifndef __ACTIMER_H
#define __ACTIMER_H

#include <WinBase.h>

class ACTimer 
{

private:
    double mFreq;
    unsigned __int64 mBaseTime;
	double mPausedTime;
	double mInitPause;
	bool mIsRunning;

public:

	ACTimer()
	{
		::timeBeginPeriod(1);

		mPausedTime = 0;
		mInitPause = 0;
		mIsRunning = FALSE;
		Reset();
	};

	~ACTimer()
	{
		::timeEndPeriod(1);
	};

	void Stop();

	void Start();

    // comeca do segundo 0.
    void Reset();

    // retorna o numero de seguntos em uma alta resolucao, desde que o timer foi criado ou resetado.
    double Seconds();

    //retorna o valor em millisegundos
    double Milliseconds();
};

inline void ACTimer::Stop() 
{
	mIsRunning = FALSE;

	unsigned __int64 val;
	::QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	double pauseInit = ((val - mBaseTime) * mFreq); 

	//pega o tempo do inicio do pause
	mInitPause = pauseInit;
};

inline void ACTimer::Start() 
{
	unsigned __int64 val;
	::QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	double pauseFim = ((val - mBaseTime) * mFreq); 

	//tempo ate dar o start
	mPausedTime += pauseFim - mInitPause;
	mIsRunning = TRUE;
};

// comeca do segundo 0.
inline void ACTimer::Reset() 
{
	unsigned __int64 pf;
	::QueryPerformanceFrequency( (LARGE_INTEGER *)&pf );
	mFreq = 1.0 / (double)pf;
	::QueryPerformanceCounter( (LARGE_INTEGER *)&mBaseTime );
};

// retorna o numero de seguntos em uma alta resolucao, desde que o timer foi criado ou resetado.
inline double ACTimer::Seconds()
{
	unsigned __int64 val;
	::QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	return ((val - mBaseTime) * mFreq) - mPausedTime;
};

//retorna o valor em millisegundos
inline double ACTimer::Milliseconds()
{
	return Seconds() * 1000.0;
};

#endif
