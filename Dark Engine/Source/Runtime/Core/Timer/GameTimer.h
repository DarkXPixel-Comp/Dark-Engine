#pragma once
//#include <Core/CoreMinimal.h>



class FGameTimer
{
	static FGameTimer* _inst_;
	static FGameTimer __timer;

private:
	FGameTimer();
public:

	static float GameTime();
	static float DeltaTime();
	static float TotalTime();

	static void Reset();
	static void Start();
	static void Stop();
	static void Tick();

private:
	double m_secondsPerCount;
	double m_deltaTime;


	__int64 m_baseTime;
	__int64 m_pausedTime;
	__int64 m_stopTime;
	__int64 m_prevTime;
	__int64 m_currTime;


	bool m_stopped;






};


