#include "FpsController.h"


FpsController::FpsController(int Fps)
{
	float floatFps = 1000.0 / (float)Fps;
	if (floatFps > 0.5f){
		time = floatFps;
	}
	else{
		time = floatFps;
	}
	setFps = Fps;
	startTime = timeGetTime();
	frameTime = startTime;
	fps = 0;
	fpsCounter = 0;
	d_diffTime = 0;
	stopFlag = false;
}


FpsController::~FpsController()
{
}

void FpsController::wait(DWORD* waitTime){
	Sleep(*waitTime);
}

bool FpsController::updataFps(){
	endTime = timeGetTime();
	if (fpsCounter >= setFps){
		d_diffTime = (endTime - frameTime);
		fps = (fpsCounter*1000) / d_diffTime;
		fpsCounter = 0;
		frameTime = endTime;
	}
	diffTime = endTime - startTime;
	if (diffTime < time){
		startTime = timeGetTime();
		Sleep(time - diffTime);
		endTime = timeGetTime();
		skipFlag = false;
	}
	else{
		//skipFlag = true;
	}
	startTime = timeGetTime();
	fpsCounter++;
	return skipFlag;
}

int FpsController::getFps(){
	return fps;
}

int FpsController::fps;
int FpsController::setFps;
int FpsController::fpsCounter;
DWORD FpsController::startTime;
DWORD FpsController::endTime;
DWORD FpsController::frameTime;
DWORD FpsController::d_diffTime;
DWORD FpsController::diffTime;
DWORD FpsController::time;
bool FpsController::skipFlag;
bool FpsController::stopFlag;