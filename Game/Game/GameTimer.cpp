#include <windows.h>

#include "GameTimer.h"

GameTimer::GameTimer()
{
	__int64 countPerSec;
	
	// cpu�� Ƣ��µ� �󸶳� �ɸ�����
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

	// cpu�� ��� Ƣ�����? 
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCounter);

	// ���� ī��Ʈ���� ����ī��Ʈ�� �����Ŀ� ��� Ƣ�� ī���� ���� ���Ѵ�.
	_deltaTime = (currentCounter - _prevCounter) * _secondsPerCount;
	_prevCounter = currentCounter;
}
double GameTimer::GetDeltaTime()
{
	return _deltaTime;
}