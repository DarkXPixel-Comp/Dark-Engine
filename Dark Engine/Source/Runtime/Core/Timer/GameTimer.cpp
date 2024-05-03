#include "GameTimer.h"
#include <Windows.h>


FGameTimer* FGameTimer::_inst_ = nullptr;
FGameTimer FGameTimer::__timer;


FGameTimer::FGameTimer() : m_secondsPerCount(0.), m_deltaTime(-1.), m_baseTime(0),
m_pausedTime(0), m_prevTime(0), m_currTime(0), m_stopped(false)
{
	_inst_ = this;

	__int64 countsPerSec;

	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));

	m_secondsPerCount = 1.0 / (double)countsPerSec;
	Tick();
}

void FGameTimer::Tick()
{
	if (_inst_->m_stopped)
	{
		_inst_->m_deltaTime = 0.0;
		return;
	}

	__int64 currTime;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));


	_inst_->m_currTime = currTime;

	_inst_->m_deltaTime = (_inst_->m_currTime - _inst_->m_prevTime) * _inst_->m_secondsPerCount;

	_inst_->m_prevTime = _inst_->m_currTime;

	if (_inst_->m_deltaTime < 0.0)
	{
		_inst_->m_deltaTime = 0.0;

	}


}


float FGameTimer::GameTime()
{
	__int64 currTime;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	return (currTime) * _inst_->m_secondsPerCount;
}

float FGameTimer::DeltaTime()
{
	return (float)_inst_->m_deltaTime;
}

float FGameTimer::TotalTime()
{
	return (float)(((_inst_->m_currTime - _inst_->m_pausedTime) - _inst_->m_baseTime) * _inst_->m_secondsPerCount);
}


void FGameTimer::Reset()
{
	__int64 currTime;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

	_inst_->m_baseTime = currTime;
	_inst_->m_prevTime = currTime;
	_inst_->m_stopTime = 0;
	_inst_->m_stopped = false;





}


void FGameTimer::Start()
{
	__int64 startTime;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

	if (_inst_->m_stopped)
	{
		_inst_->m_pausedTime += (startTime - _inst_->m_stopTime);
		_inst_->m_prevTime = startTime;
		_inst_->m_stopTime = 0;
		_inst_->m_stopped = false;
	}

}
