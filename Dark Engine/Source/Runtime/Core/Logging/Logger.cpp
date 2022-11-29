#pragma once
#include "Logger.hpp"



class Logger;

Logger* Logger::inst;

Logger l;


Logger::Logger()
{
	inst = this;

	//std::thread th(logging, this); th.detach();

	///*severenty = (s);
	//log("The system started successfully", LOGGER_ENUM::LOGGER_INFO);*/
}





void Logger::Initialize(size_t s)
{
	inst->severenty = (s);
	std::thread th(logging, inst); th.detach();

	inst->urgLog("The system started successfully", LOGGER_ENUM::LOGGER_INFO);
}




void record(log_& obj)
{
	std::string folder = "logs/";

	tm ltm;
	localtime_s(&ltm, &obj.time);

	

	std::ofstream fout(folder + std::to_string(ltm.tm_year - 100 + 2000) + "-" + std::to_string(ltm.tm_mday) + "-" + std::to_string(ltm.tm_mon + 1) + ".log", std::ios::app);

	std::string text = "[" + std::to_string(ltm.tm_hour) + ":" + std::to_string(ltm.tm_min) + ":" + std::to_string(ltm.tm_sec) + "] ";

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

	fout << text << " - " << obj.txt << "\n";

	if (obj.isConsole)
	{
		CommandConsole::Print(text.c_str());
		CommandConsole::Print(" - ");
		CommandConsole::Print(obj.txt.c_str());
		CommandConsole::Print("\n");
	}

	fout.close();

}

void logging(Logger* obj)
{
	while (true)
	{
		while (!obj->logs.empty())
		{
			auto it = obj->logs.begin();

			record(*it);

			//std::cout << it->txt;
			obj->logs.erase(it);
		}

		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}

void Logger::log(std::string logTxt, LOGGER_ENUM severenty)
{
	if (((inst->severenty) | (severenty)) != inst->severenty)
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

void Logger::log(std::wstring str, LOGGER_ENUM severenty)
{
	if (((inst->severenty) | (severenty)) != inst->severenty)
	{
		return;
	}

	log_ temp;

	temp.severenty = severenty;
	temp.time = time(0);
	temp.txt = std::string(str.begin(), str.end());
	temp.isConsole = true;

	inst->logs.push_back(temp);


}


void Logger::urgLog(std::string logTxt, LOGGER_ENUM severinty)
{
	std::list<log_> lTemp = this->logs;

	logs.clear();

	if (((this->severenty) | (severinty)) != this->severenty)
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

void Logger::exLog(std::string logTxt, LOGGER_ENUM severenty)
{
	log_ temp;

	temp.severenty = severenty;
	temp.time = time(0);
	temp.txt = logTxt;
	temp.isConsole = true;

	inst->logs.push_back(temp);
}




void Logger::wait()
{
	while (!inst->logs.empty())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

}

