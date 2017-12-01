#include <windows.h>

#include "GameTimer.h"

GameTimer::GameTimer()
{
	__int64 countPerSec;
	
	// cpu가 튀기는데 얼마나 걸리는지
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec);
	_secondsPerCount = 1.0 / (double)countPerSec;
}
GameTimer::~GameTimer()
{

}
void GameTimer::Init()
{
	__int64 currentCounter;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentCounter);
	_prevCounter = currentCounter;
	_deltaTime = 0.0;
}
void GameTimer::Update()
{
	__int64 currentCounter;

	// cpu가 몇번 튀겻는지? 
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCounter);

	// 현재 카운트에서 이전카운트를 빼준후에 몇번 튀긴 카운터 수를 곱한다.
	_deltaTime = (currentCounter - _prevCounter) * _secondsPerCount;
	_prevCounter = currentCounter;
}
double GameTimer::GetDeltaTime()
{
	return _deltaTime;
}