#pragma once
#include "Logger.h"
#include <Application/Application.h>




class Logger;

Logger* Logger::inst;
Logger l;


Logger::Logger()
{
	inst = this;
}





void Logger::Initialize(size_t s)
{
	inst->severity = (s);
	inst->isWork = true;
	inst->Logs.resize(256);

	std::thread th(logging, inst); th.detach();

	inst->log("The system started successfully", LOGGER_ENUM::LOGGER_INFO);

	//inst << FString("s")


}


void Logger::Exit()
{
	inst->isWork = false;
}

void record(log_& obj)
{
	FString folder = "logs/";

	tm ltm;
	localtime_s(&ltm, &obj.time);

	

	std::ofstream fout(folder.ToString() + std::to_string(ltm.tm_year - 100 + 2000) + "-" + std::to_string(ltm.tm_mday) + "-" + std::to_string(ltm.tm_mon + 1) + ".log", std::ios::app);

	FString text = "[" + std::to_string(ltm.tm_hour) + ":" + std::to_string(ltm.tm_min) + ":" + std::to_string(ltm.tm_sec) + "] ";

	//fout << "[" + std::to_string(ltm.tm_hour) + ":" + std::to_string(ltm.tm_min) + ":" + std::to_string(ltm.tm_sec) + "] ";


	//fout << text;


	switch (obj.severenty)
	{
	case LOGGER_ERROR:
		//fout << "ERROR ";
		text += "ERROR ";
		break;
	case LOGGER_WARNING:
		//fout << "WARNIMG";
		text += "WARNIMG ";
		break;
	case LOGGER_INFO:
		//fout << "INFO ";
		text += "INFO ";
		break;
	default:
		//fout << "NULL ";
		text += "NULL ";
		break;
	}

	fout << text.ToString() << " - " << obj.txt.ToString() << "\n";

	if (obj.isConsole)
	{
		/*CommandConsole::Print(text.c_str());
		CommandConsole::Print(" - ");
		CommandConsole::Print(obj.txt.c_str());
		CommandConsole::Print("\n");*/

		PrintLine(text.ToString().c_str(), " - ", obj.txt.ToString().c_str(), "\n");
	}

	obj.Result = text + " - " + obj.txt;

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
		while (!obj->logs.empty())
		{
			auto it = obj->logs.begin();

			if (it == obj->logs.end())
				continue;

			if (it->ModernLog)
			{
				ModernLog(*it);
			}
			else
			{
				record(*it);
			}
			//std::cout << it->txt;

			
			if (obj->CountCurrentLogs >= 256)
			{
				obj->CountCurrentLogs = 0;
			}
			obj->Logs[obj->CountCurrentLogs++] = *it;
			obj->MaxCountLogs = FPlatformMath::Max(obj->MaxCountLogs, obj->CountCurrentLogs);

			obj->logs.erase(it);
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}


}

void Logger::log(FString logTxt, LOGGER_ENUM severenty)
{
	if (((inst->severity) | (severenty)) != inst->severity)
	{
		return;
	}

	log_ temp;

	temp.severenty = severenty;
	temp.time = time(0);
	temp.txt = logTxt;
	temp.isConsole = true;

	inst->logs.push_back(temp);

}

void Logger::logF(const char* arg, ...)
{
	va_list arguments;
	string ret;
	for (va_start(arguments, arg); arg != nullptr; arg = va_arg(arguments, const char*))
	{
		ret += arg;
	}

	Logger::log(ret);
	va_end(arguments);


}

void Logger::logF(FString *arg, ...)
{
	va_list arguments;
	FString ret;


	for (va_start(arguments, arg); arg != nullptr; arg = va_arg(arguments, FString*))
	{
		ret += *arg;
	}

	Logger::log(ret);
	va_end(arguments);
}



void Logger::log(std::wstring str, LOGGER_ENUM severenty)
{
	if (((inst->severity) | (severenty)) != inst->severity)
	{
		return;
	}

	log_ temp;

	temp.severenty = severenty;
	temp.time = time(0);
	temp.txt = str;
	temp.isConsole = true;

	inst->logs.push_back(temp);


}


void Logger::urgLog(FString logTxt, LOGGER_ENUM severinty)
{
	std::list<log_> lTemp = this->logs;

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

	return;

}

void Logger::exLog(FString logTxt, LOGGER_ENUM severenty)
{
	log_ temp;

	temp.severenty = severenty;
	temp.time = time(0);
	temp.txt = logTxt;
	temp.isConsole = true;

	inst->logs.push_back(temp);
}

void Logger::log(log_ InLog)
{
	InLog.time = time(0);
	inst->logs.push_back(InLog);
}




void Logger::wait()
{
	while (!inst->logs.empty())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}


