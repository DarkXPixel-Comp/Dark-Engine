#pragma once

#include <Windows.h>
#include <thread>


class Thread
{
	HANDLE thread;
	DWORD threadID;

public:
	template<class _Fn, class _Args>
	Thread(_Fn&& lpFunc, _Args&& args)
	{
		thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&lpFunc, &args, CREATE_SUSPENDED, &threadID);

		ResumeThread(thread);

	}



};