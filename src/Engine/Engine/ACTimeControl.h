//**************************
// Controla o tempo do sistema, trabalha com millisegundos
// creator: Daniel Gadens
// date: 18/05/2011
//****************************


#ifndef __ACTIMECONTROL_H
#define __ACTIMECONTROL_H

#include <windows.h>
#include "EngineCommon.h"

class ACTimer;

class ACTimeControl	
{

private:
	static FLOAT mfElapsedTime;
	static DWORD mElapsedTime;
    static UINT mFrameCount;
    static UINT mFps;
	static UINT mAVGFps;
	static DWORD mCurrentTime;
    static UINT mLastTime;
    static UINT mFrameTime;
	static DWORD mTotalTime;
	static ACTimer mTimer;

public:

	static BOOL IsRunning;
	static BOOL FixedFPS;

	//METODOS
    static void Update();
    static INT GetFPS();
    static FLOAT UnitPerSec(FLOAT speed);
	static UINT GetElapsedTime();
	static FLOAT GetFElapsedTime();
	static void Stop();
	static void Start();

	static double GetTotalMilliseconds();
	static double GetTotalSeconds();
	static double GetTotalMinutes();
	static double GetTotalHours();

	static DWORD GetMilliseconds();
	static DWORD GetSeconds();
	static DWORD GetMinutes();
	static DWORD GetHours();

};

#endif