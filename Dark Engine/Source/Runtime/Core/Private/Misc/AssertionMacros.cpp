#include "Misc/AssertionMacros.h"
#include "Platform/PlatformString.h"
#include "Platform/PlatformMisc.h"
#include "Misc/Paths.h"
#include <fstream>


static bool GIsCriticalError = false;

bool FDebug::CheckFailed(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format, ...)
{
	va_list Args;
	va_start(Args, Format);
	TCHAR Buffer[256];
	FPlatformString::StringPrintf(Buffer, 256, Format, Args);
	OpenHTMLErrorV(Expression, File, Line, Buffer, Args);
	va_end(Args);

	if (!FPlatformMisc::IsDebuggerPresent())
	{
		FPlatformMisc::PromptForRemoteDebugging();

		return false;
	}

	return true;
}


std::wstring GetCurrentDateTimeString()
{
	// Получаем текущее время с высокой точностью
	auto now = std::chrono::system_clock::now();
	// Преобразуем в системное время (time_t)
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	// Для потоковой записи в строку
	std::wstringstream ss;
	// Конвертируем время в локальное представление
	std::tm local_tm;
#ifdef _WIN32
	localtime_s(&local_tm, &now_time);  // Windows
#else
	localtime_r(&now_time, &local_tm);  // Linux/macOS
#endif

	// Форматируем дату и время
	ss << std::put_time(&local_tm, TEXT("%Y-%m-%d %H:%M:%S"));
	return ss.str();
}

void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
{
	size_t pos = 0;
	while ((pos = str.find(from, pos)) != std::string::npos)
	{
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
}


std::wstring EscapeHtml(const std::wstring& data) {
	std::wstring buffer;
	buffer.reserve(data.size());
	for (TCHAR c : data) {
		switch (c) {
		case '&': buffer.append(TEXT("&amp;")); break;
		case '<': buffer.append(TEXT("&lt;")); break;
		case '>': buffer.append(TEXT("&gt;")); break;
		case '"': buffer.append(TEXT("&quot;")); break;
		case '\'': buffer.append(TEXT("&#39;")); break;
		case '\n': buffer.append(TEXT("<br>")); break; // переносы строк в детали
		default: buffer.push_back(c); break;
		}
	}
	return buffer;
}

std::vector<std::wstring> SplitLines(const std::wstring& str) {
	std::vector<std::wstring> lines;
	size_t pos = 0, prev = 0;
	while ((pos = str.find(L'\n', prev)) != std::wstring::npos) {
		lines.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	if (prev < str.size())
		lines.push_back(str.substr(prev));
	return lines;
}

std::wstring GenerateStackTraceHtml(const std::wstring& stackText) {
	std::vector<std::wstring> lines = SplitLines(stackText);
	std::wstring html;

	for (const auto& line : lines) {
		std::wstring escapedLine = EscapeHtml(line);

		// Разделяем по " at "
		size_t pos = escapedLine.find(L" at ");
		std::wstring funcPart = pos == std::wstring::npos ? escapedLine : escapedLine.substr(0, pos);
		std::wstring addrPart = pos == std::wstring::npos ? L"" : escapedLine.substr(pos + 4);

		html += L"<div class=\"stack-entry\">\n";
		html += L"  <div class=\"stack-func\">" + funcPart + L"</div>\n";
		if (!addrPart.empty())
			html += L"  <div class=\"stack-details\">" + addrPart + L"</div>\n";
		else
			html += L"  <div class=\"stack-details\"></div>\n";
		html += L"</div>\n";
	}

	return html;
}

void FDebug::OpenHTMLErrorV(const ANSICHAR* Expression, const ANSICHAR* File, int32 Line, const TCHAR* Format, va_list Args)
{
	std::filesystem::path FilePath(File);
	FilePath = std::filesystem::relative(FilePath, *FPaths::RootDir());
	if (FilePath.empty())
	{
		FilePath = File;
	}

	TCHAR Buffer[512];
	FPlatformString::StringPrintf(Buffer, 512, Format, Args);

	std::wifstream fin(*(FPaths::SourceDir() + TEXT("Templates/AssertError.html")));
	if (!fin.is_open())
	{
		FPlatformMisc::DebugPrint(TEXT("Failed to open template file for error report."));
		return;
	}

	std::wstringstream buffer;
	buffer << fin.rdbuf();
	std::wstring html = buffer.str();
	fin.close();

	FString StackTrace;
	FPlatformMisc::GetStackTrace(StackTrace);
	//GenerateStackTraceHtml(StackTrace);
	
	ReplaceAll(html, L"{{ERROR_MESSAGE}}", *FString(Expression));
	ReplaceAll(html, L"{{FILE_PATH}}", *FString(FilePath));
	ReplaceAll(html, L"{{LINE_NUMBER}}", std::to_wstring(Line));
	ReplaceAll(html, L"{{ERROR_DATETIME}}", GetCurrentDateTimeString());
	ReplaceAll(html, L"{{STACK_TRACE_INTERACTIVE}}", GenerateStackTraceHtml(StackTrace));



	std::wofstream fout(*(FPaths::EngineLogDir() + TEXT("AssertError.html")));
	if (!fout.is_open())
	{
		FPlatformMisc::DebugPrint(TEXT("Failed to open output file for error report."));
		return;
	}

	fout << html;
	fout.close();

	FPlatformMisc::OpenHTML(*(FPaths::EngineLogDir() + TEXT("AssertError.html")));	
}

void FDebug::OpenHTMLCallStack(const FString& ExceptionCode, const FString& ExceptionAddress, const FString& StackTrace)
{
	std::wifstream fin(*(FPaths::SourceDir() + TEXT("Templates/UnhandledError.html")));
	if (!fin.is_open())
	{
		FPlatformMisc::DebugPrint(TEXT("Failed to open template file for error report."));
		return;
	}

	std::wstringstream buffer;
	buffer << fin.rdbuf();
	std::wstring html = buffer.str();
	fin.close();

	ReplaceAll(html, L"{{EXCEPTION_CODE}}", *ExceptionCode);
	ReplaceAll(html, L"{{EXCEPTION_ADDRESS}}", *ExceptionAddress);
	ReplaceAll(html, L"{{STACK_TRACE}}", *StackTrace);
	ReplaceAll(html, L"{{ERROR_DATETIME}}", GetCurrentDateTimeString());
	ReplaceAll(html, TEXT("{{ERROR_MESSAGE}}"), *ExceptionCode);
	ReplaceAll(html, TEXT("{{FILE_PATH}}"), TEXT("UNDEFINED"));

	//ReplaceAll(html, L"{{STACK_TRACE_INTERACTIVE}}", GenerateStackTraceHtml(StackTrace));



	std::wofstream fout(*(FPaths::EngineLogDir() + TEXT("UnhandledError.html")));
	if (!fout.is_open())
	{
		FPlatformMisc::DebugPrint(TEXT("Failed to open output file for error report."));
		return;
	}

	fout << html;
	fout.close();

	FPlatformMisc::OpenHTML(*(FPaths::EngineLogDir() + TEXT("UnhandledError.html")));


}