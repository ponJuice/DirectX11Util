#pragma once
#include "Terminal.h"
class FpsController
{
public:
	static int fps;
	static int setFps;
	static int fpsCounter;
	static DWORD diffTime;
	static DWORD startTime;
	static DWORD endTime;
	static DWORD frameTime;
	static DWORD d_diffTime;
	static bool skipFlag;
	static DWORD time;
	static bool stopFlag;
public:
	int getFps();
	bool updataFps();
	void wait(DWORD* waitTime);
	FpsController(int Fps);
	~FpsController();
};
