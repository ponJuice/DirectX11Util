#pragma once
#include "Terminal.h"
class FpsController
{
public:
	int fps;
	int setFps;
	int fpsCounter;
	DWORD diffTime;
	DWORD startTime;
	DWORD endTime;
	DWORD frameTime;
	DWORD d_diffTime;
	bool skipFlag;
	DWORD time;
	bool stopFlag;
public:
	int getFps();
	bool updataFps();
	void wait(DWORD* waitTime);
	FpsController(int Fps);
	~FpsController();
};
