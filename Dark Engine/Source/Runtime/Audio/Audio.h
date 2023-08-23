#pragma once
#include <xaudio2.h>
#include <assert.h>
#include <Core/Logging/Logger.hpp>




class FAudioCore
{
public:
	FAudioCore()
	{
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		Log("Testfdssdfgdfgdf");

		if (FAILED(hr))
		{
			assert(false);
		
			return;
		}


		if (FAILED(hr = XAudio2Create(&m_XAudio, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		{
			assert(false);
			return;
		}
	}



private:
	IXAudio2* m_XAudio = nullptr;












};