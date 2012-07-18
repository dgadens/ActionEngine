#include "ACTimeControl.h"
#include "ACTimer.h"

//tem  q definir aqui pq sao estaticos
FLOAT			ACTimeControl::mfElapsedTime = 0.0f;
DWORD			ACTimeControl::mElapsedTime = 0;
UINT			ACTimeControl::mFrameCount = 0;
UINT			ACTimeControl::mFps = 0;
UINT            ACTimeControl::mAVGFps = 0;
DWORD			ACTimeControl::mCurrentTime = 0;
UINT			ACTimeControl::mLastTime = 0;
UINT			ACTimeControl::mFrameTime = 0;
DWORD           ACTimeControl::mTotalTime = 0;
BOOL            ACTimeControl::FixedFPS = TRUE;
ACTimer         ACTimeControl::mTimer;
BOOL            ACTimeControl::IsRunning = FALSE;

void ACTimeControl::Update()
{
	if (IsRunning)
	{
		//pega o timer desde quando o micro foi ligado
		mCurrentTime = mTimer.Milliseconds();

		//preve o problema de resetar o contador a cada 47 dias
		if (mCurrentTime < mLastTime)
			mLastTime = mCurrentTime;

		//tempo desde o ultimo ciclo
		mElapsedTime = mCurrentTime - mLastTime;

		//se o elapsed time for menor q 16 entao os fps foram maiores doq 60 entao ele espera o tempo restante
		if (FixedFPS)
		{
			if (mElapsedTime < 16)
			{
				DWORD dif = 16 - mElapsedTime;
				Sleep(dif);
				mElapsedTime = 16;
			}
		}

		//armazena o tempo total
		mFrameTime += mElapsedTime;
		//soma 1 no contador de frames
		mFrameCount++;
		if (mFrameTime > 1000)
		{
			mFps = mFrameCount;
			mFrameCount = 0;
			mFrameTime = 0;
		}

		mfElapsedTime = (FLOAT)mElapsedTime * 0.001f;

		mLastTime = mCurrentTime;
	}
};

void ACTimeControl::Stop()
{
	IsRunning = FALSE;
	mTimer.Stop();
};

void ACTimeControl::Start()
{
	IsRunning = TRUE;
	mTimer.Start();
};

INT ACTimeControl::GetFPS()
{
	return mFps;
};

FLOAT ACTimeControl::UnitPerSec(FLOAT speed)
{
	if (mFps != 1)
		return (speed / mFps);
	else
		return (speed / 60);
};

UINT ACTimeControl::GetElapsedTime()
{
	return mElapsedTime;
};

FLOAT ACTimeControl::GetFElapsedTime()
{
	return mfElapsedTime;
};

double ACTimeControl::GetTotalMilliseconds()
{
	return mCurrentTime;
};

double ACTimeControl::GetTotalSeconds()	
{
	return (double)mCurrentTime / 1000.0f;
};

double ACTimeControl::GetTotalMinutes()
{
	return mCurrentTime / 60000.0f;
};

double ACTimeControl::GetTotalHours()
{
	return mCurrentTime / 3600000.0f;
};

DWORD ACTimeControl::GetMilliseconds()
{
	return mCurrentTime % 1000;
};

DWORD ACTimeControl::GetSeconds()
{
	return (mCurrentTime / 1000) % 60;
};

DWORD ACTimeControl::GetMinutes()
{
	return (mCurrentTime / 60000) % 60;
};

DWORD ACTimeControl::GetHours()
{
	return (mCurrentTime / 3600000) % 24;
};

