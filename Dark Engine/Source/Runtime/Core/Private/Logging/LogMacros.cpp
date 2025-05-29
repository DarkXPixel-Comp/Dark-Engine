#include "Logging/LogMacros.h"
#include <thread>
#include <mutex>
#include <fstream>
#include "Memory/MemoryCore.h"
#include "Misc/Paths.h"
#include "Logging/StructedLog.h"
#include "Misc/GlobalLogger.h"

namespace LOGGER
{
	enum LOGGER_ENUM
	{
		LOGGER_INFO = 0b00000001,
		LOGGER_WARNING = 0b00000010,
		LOGGER_ERROR = 0b00000100,
		LOGGER_DEBUG = 0b00001000,
		LOGGER_CONSOLE = 0b00010000
	};

	struct log_
	{
		size_t severenty = 0;
		FString txt = TEXT("");
		time_t time = time_t();
		bool isConsole = false;

		///////
		FString Result = TEXT("");
		int32 Line = 0;
		FString File = TEXT("");
	};

	class Logger;
	extern TUniquePtr<Logger> LoggerGlobal;

	void ModernLog(log_& Obj);

	void record(log_& obj);


	void logging();


	class Logger
	{
		friend void logging();
		friend void record(log_&);
	public:
		Logger() { Initialize(this); }
		~Logger() { Exit(); }

		static void Initialize(Logger* logger)
		{
			logger->LogThread = std::thread(logging);

			FString Path = FPaths::EngineLogDir() / TEXT("latest/latest.log"); 
			std::filesystem::create_directories(*(FPaths::EngineLogDir() / TEXT("latest")));

			std::wofstream fout(*Path, std::ios_base::trunc);
			if (fout.is_open())
			{
				fout.close();
			}

			logger->isWork = true;

		}
		static void Exit()
		{
			if (LoggerGlobal->isWork)
			{
				LoggerGlobal->wait();
				LoggerGlobal->LogThread.join();
				LoggerGlobal->logs.clear();
			}

		}

		static void log(log_ InLog)
		{
			if (LoggerGlobal->isWork)
			{
				LoggerGlobal->Mutex.lock();
				InLog.time = time(0);
				LoggerGlobal->logs.push_back(InLog);
				LoggerGlobal->Mutex.unlock();
			}
		}
		static void wait()
		{
			uint64 Counter = 0;
			while (!LoggerGlobal->logs.empty())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (Counter++ > 1000)
					break;
			}
		}



	private:
		static void ClearLogs()
		{
			if(!LoggerGlobal->logs.empty())
			{
				LoggerGlobal->Mutex.lock();
				LoggerGlobal->logs.clear();
				LoggerGlobal->Mutex.unlock();
			}
		}

	public:
		std::vector<log_> TempLogs;
	private:
		std::vector <log_> logs;
		bool isWork = false;
		std::mutex Mutex;
		std::thread LogThread;
	};


	TUniquePtr<Logger> LoggerGlobal = MakeUnique(new Logger());
	void ModernLog(log_& Obj)
	{
		tm ltm;
		localtime_s(&ltm, &Obj.time);

		FString text = TEXT("[") + std::to_wstring(ltm.tm_hour) + TEXT(":") + std::to_wstring(ltm.tm_min)
			+ TEXT(":") + std::to_wstring(ltm.tm_sec) + TEXT("]");

		Obj.Result = text + TEXT(" - ") + Obj.Result;

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


		FString out = folder / std::to_wstring(ltm.tm_year - 100 + 2000) +
			TEXT("-") + std::to_wstring(ltm.tm_mday) + TEXT("-") + std::to_wstring(ltm.tm_mon + 1) + TEXT(".log");

		out = FString::PrintF(TEXT("%s/%i-%i-%i.log"), *folder, (ltm.tm_year - 100 + 2000), (ltm.tm_mday), (ltm.tm_mon + 1));

		//fout.open(folder.ToString() + std::to_string(ltm.tm_year - 100 + 2000) + "-" + std::to_string(ltm.tm_mday) + "-" + std::to_string(ltm.tm_mon + 1) + ".log", std::ios::app);

		fout.open(*out, std::ios::app);

		if (!fout.is_open() || !Latest.is_open())
		{
			return;
		}


		fout << *obj.Result << TEXT("\n");
		Latest << *obj.Result << TEXT("\n");

		Latest.close();
		fout.close();

	}
	void logging()
	{
		while (LoggerGlobal->isWork)
		{
			std::vector<log_>& TempLogs = LoggerGlobal->TempLogs;
			if (LoggerGlobal->logs.size())
			{
				LoggerGlobal->Mutex.lock();
				TempLogs = LoggerGlobal->logs;
				LoggerGlobal->logs.clear();
				LoggerGlobal->Mutex.unlock();
			}
			else
			{
				LoggerGlobal->Mutex.lock();
				TempLogs.clear();
				LoggerGlobal->Mutex.unlock();
			}
			for (auto& i : TempLogs)
			{
				ModernLog(i);
				record(i);
				
			}
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}
	}
}




//////////
void FMsg::LogV(const ANSICHAR* File, int32 Line, const FString& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, va_list Args)
{
	FLogRecord Record;
	FString Message;
	Message.AppendV(Fmt, Args);

	Record.SetCategory(Category);
	Record.SetFile(File);
	Record.SetTime(std::chrono::system_clock::now());
	Record.SetLine(Line);
	Record.SetVerbosity(Verbosity);
	Record.SetFormat(*Message);

	FGlobalLogger::Get().LogRecord(Record);

	/*FString Message;
	Message.AppendV(Fmt, Args);

	FString Verbose;

	switch (Verbosity)
	{
	case ELogVerbosity::Fatal:
		Verbose = TEXT("FATAL");
		break;
	case ELogVerbosity::Error:
		Verbose = TEXT("Error");
		break;
	case ELogVerbosity::Warning:
		Verbose = TEXT("Warning");
		break;
	case ELogVerbosity::Log:
		Verbose = TEXT("Log");
		break;
	default:
		return;
	}

	FString Result = FString::PrintF(TEXT("[%s] [%s]: %s"), *Category, *Verbose, *Message);
	LOGGER::log_ CurrentLog;
	CurrentLog.Result = Result;
	LOGGER::Logger::log(CurrentLog);*/
}

void BasicLog(const FLogCategoryBase& Category, const FStaticBasicLogRecord* Log, ...)
{
	va_list	 Args;
	va_start(Args, Log);
	//FString test = Log->Format;
	FMsg::LogV(Log->File, Log->Line, Category.GetCategoryName(), Log->Verbosity,
		static_cast<const wchar_t*>(Log->Format), Args);
	va_end(Args);

}