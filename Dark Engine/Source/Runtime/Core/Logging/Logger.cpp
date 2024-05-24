#pragma once
#include "Logger.h"
#include <Application/Application.h>
#include "Misc/Paths.h"




class Logger;

Logger* Logger::inst;


Logger::Logger()
{
	inst = this;
}


const std::locale DEFAULT_LOCALE(".utf-8");


void Logger::Initialize(size_t s)
{
	inst = new Logger();

	inst->severity = (s);
	inst->isWork = true;
	inst->logs.reserve(256);
	inst->TempLogs.reserve(256);


	inst->LogThread = std::thread(logging, inst);

	inst->log("The system started successfully", LOGGER_ENUM::LOGGER_INFO);

	FString Path = FPaths::EngineLogDir() / TEXT("latest/latest.log"); //TEXT("logs/latest/latest.log");

	std::wofstream fout(*Path, std::ios_base::trunc);
	if (fout.is_open())
	{
		fout.close();
	}

}


void Logger::Exit()
{
	if (inst)
	{
		inst->wait();
		inst->isWork = false;
		inst->LogThread.join();
		inst->logs.clear();
		delete inst;
		inst = nullptr;
	}
}

void record(log_& obj)
{
	static const FString folder = FPaths::EngineLogDir();
	static const FString Path = FPaths::EngineLogDir() / TEXT("latest/latest.log");


	tm ltm;
	localtime_s(&ltm, &obj.time);


	

	//std::ofstream fout;	

	std::wofstream fout;
	std::wofstream Latest(*Path, std::ios_base::app);


	FString out = folder.ToString() + std::to_string(ltm.tm_year - 100 + 2000) +
		"-" + std::to_string(ltm.tm_mday) + "-" + std::to_string(ltm.tm_mon + 1) + ".log";

	out = FString::PrintF(TEXT("%s/%i-%i-%i.log"), *folder, (ltm.tm_year - 100 + 2000), (ltm.tm_mday), (ltm.tm_mon + 1));

	//fout.open(folder.ToString() + std::to_string(ltm.tm_year - 100 + 2000) + "-" + std::to_string(ltm.tm_mday) + "-" + std::to_string(ltm.tm_mon + 1) + ".log", std::ios::app);
	
	fout.open(*out, std::ios::app);

	fout.imbue(DEFAULT_LOCALE);
	Latest.imbue(DEFAULT_LOCALE);

	if (!fout.is_open() || !Latest.is_open())
	{
		return;
	}


	fout << *obj.Result << TEXT("\n");
	Latest << *obj.Result << TEXT("\n");

	Latest.close();
	fout.close();
}

void ModernLog(log_& Obj)
{
	tm ltm;
	localtime_s(&ltm, &Obj.time);

	FString text = "[" + std::to_string(ltm.tm_hour) + ":" + std::to_string(ltm.tm_min)
		+ ":" + std::to_string(ltm.tm_sec) + "]";

	Obj.Result = text + + TEXT(" - ") + Obj.Result;
}


void logging(Logger* obj)
{
	while (obj->isWork)
	{
		std::vector<log_>& TempLogs = obj->TempLogs;
		if (obj->logs.size())
		{
			obj->Mutex.lock();
			TempLogs = obj->logs;
			obj->logs.clear();
			obj->Mutex.unlock();
		}
		else
		{
			obj->Mutex.lock();
			TempLogs.clear();
			obj->Mutex.unlock();
		}
		for (auto& i : TempLogs)
		{
			if (i.ModernLog)
			{
				ModernLog(i);
				record(i);
			}
			else
			{
				record(i);
			}
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
}

void Logger::log(FString logTxt, LOGGER_ENUM severenty)
{
	//if (((inst->severity) | (severenty)) != inst->severity)
	//{
	//	return;
	//}

	//log_ temp;

	//temp.severenty = severenty;
	//temp.time = time(0);
	//temp.txt = logTxt;
	////temp.isConsole = true;
	//temp.ModernLog = false;

	//inst->logs.push_back(temp);

}

void Logger::logF(const char* arg, ...)
{
	/*va_list arguments;
	string ret;
	for (va_start(arguments, arg); arg != nullptr; arg = va_arg(arguments, const char*))
	{
		ret += arg;
	}

	Logger::log(ret);
	va_end(arguments);*/


}

void Logger::logF(FString *arg, ...)
{
	//va_list arguments;
	//FString ret;


	//for (va_start(arguments, arg); arg != nullptr; arg = va_arg(arguments, FString*))
	//{
	//	ret += *arg;
	//}

	//Logger::log(ret);
	//va_end(arguments);
}



void Logger::log(std::wstring str, LOGGER_ENUM severenty)
{
	/*if (((inst->severity) | (severenty)) != inst->severity)
	{
		return;
	}

	log_ temp;

	temp.severenty = severenty;
	temp.time = time(0);
	temp.txt = str;
	temp.isConsole = true;

	inst->logs.push_back(temp);*/


}


void Logger::urgLog(FString logTxt, LOGGER_ENUM severinty)
{
	/*std::list<log_> lTemp = this->logs;

	logs.clear();

	if (((this->severity) | (severinty)) != this->severity)
	{
		return;
	}

	log_ temp;
	temp.severenty = severinty;
	temp.time = time(0);
	temp.txt = logTxt;

	record(temp);

	logs = lTemp;

	lTemp.clear();

	return;*/

}

void Logger::exLog(FString logTxt, LOGGER_ENUM severenty)
{
	log_ temp;

	//temp.severenty = severenty;
	//temp.time = time(0);
	//temp.txt = logTxt;
	//temp.isConsole = true;

	//inst->logs.push_back(temp);
}

void Logger::log(log_ InLog)
{
	if (inst && inst->isWork)
	{
		inst->Mutex.lock();
		InLog.time = time(0);
		inst->logs.push_back(InLog);
		inst->Mutex.unlock();
	}
}

void Logger::ClearLogs()
{	 
	if(inst->logs.size() != 0)
	{
		inst->Mutex.lock();
		inst->logs.clear();
		inst->Mutex.unlock();
	}
}




void Logger::wait()
{
	while (!inst->logs.empty())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}


