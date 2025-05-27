#include "Windows/WindowsPlatformMisc.h"
#include "Misc/Paths.h"
#include <DbgHelp.h>

void FWindowsPlatformMisc::DebugPrint(const FString& Text)
{
	OutputDebugString(*Text);
}

bool FWindowsPlatformMisc::IsDebuggerPresent()
{
	return static_cast<bool>(::IsDebuggerPresent());
}

void FWindowsPlatformMisc::PromptForRemoteDebugging()
{
	FString Text = FString::PrintF(TEXT("Have a programmer remote debug this crash?\n"
		"Hit NO to exit and submit error report as normal.\n"
		"Otherwise, contact a programmer for remote debugging,\n"
		"giving them the changelist number below.\n"
		"Once they have confirmed they are connected to the machine,\n"
		"hit YES to allow them to debug the crash.\n"
		"[Changelist = %d]"));

	if (CreateMessageBoxError(*Text, TEXT("Crashed")))
	{
		//::DebugBreak();
	}
	::DebugBreak();
}

void FWindowsPlatformMisc::OpenHTML(const FString& Path)
{
	ShellExecuteW(NULL, L"open", *Path, NULL, NULL, SW_SHOWNORMAL);
}

void FWindowsPlatformMisc::GetStackTrace(FString& Out)
{
	const int32 MaxFrames = 32;
	void* StackFrames[MaxFrames];

	WORD Frames = CaptureStackBackTrace(0, MaxFrames, StackFrames, NULL);

	HANDLE Process = GetCurrentProcess();
	BOOL Result = SymInitialize(Process, NULL, TRUE);

	SYMBOL_INFO* Symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(TCHAR), 1);
	if (!Symbol)
		return;

	Symbol->MaxNameLen = 255;
	Symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	for (WORD i = 0; i < Frames; ++i)
	{
		DWORD64 Address = (DWORD64)(uintptr_t)StackFrames[i];
		DWORD DisplacementLine = 0;
		IMAGEHLP_LINE64 Line = { 0 };
		Line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		if (SymFromAddr(Process, Address, NULL, Symbol))
		{
			if (SymGetLineFromAddr64(Process, Address, &DisplacementLine, &Line))
			{
				FString Relative = std::filesystem::relative(Line.FileName, *FPaths::RootDir()).wstring();

				Out += FString::PrintF(TEXT("%s at %s:%d (0x%p)\n"),
					*FString(Symbol->Name),
					*Relative,
					Line.LineNumber,
					(void*)Address);
			}
			else
			{
				Out += FString::PrintF(TEXT("%s at 0x%p\n"), *FString(Symbol->Name), (void*)Address);
			}
		}
		else
		{
			Out += FString::PrintF(TEXT("Unknown function at 0x%08X\n"), Address);
		}
	}

	free(Symbol);
}

void FWindowsPlatformMisc::GetStackTrace(CONTEXT* context, FString& Out)
{
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	STACKFRAME64 stackFrame;
	ZeroMemory(&stackFrame, sizeof(STACKFRAME64));

	stackFrame.AddrPC.Offset = context->Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context->Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context->Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;

	for (int frameNum = 0; frameNum < 64; ++frameNum)
	{
		BOOL result = StackWalk64(
			IMAGE_FILE_MACHINE_AMD64,
			process,
			GetCurrentThread(),
			&stackFrame,
			context,
			NULL,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			NULL
		);

		if (!result || stackFrame.AddrPC.Offset == 0)
			break;

		char buffer[sizeof(SYMBOL_INFO) + 256];
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		DWORD64 displacement = 0;
		if (SymFromAddr(process, stackFrame.AddrPC.Offset, &displacement, symbol))
		{
			IMAGEHLP_LINE64 Line = { 0 };
			Line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD displacementLine = 0;

			if (SymGetLineFromAddr64(process, stackFrame.AddrPC.Offset, &displacementLine, &Line))
			{
				FString Relative = std::filesystem::relative(Line.FileName, *FPaths::RootDir()).wstring();

				Out += FString::PrintF(TEXT("%s at %s:%d (0x%p)\n"),
					*FString(symbol->Name),
					*Relative,
					Line.LineNumber,
					(void*)stackFrame.AddrPC.Offset);
			}
			else
			{
				Out += FString::PrintF(TEXT("%s at 0x%p\n"), *FString(symbol->Name), (void*)stackFrame.AddrPC.Offset);
			}
		}
		else
		{
			Out += FString::PrintF(TEXT("Unknown function at 0x%p\n"), (void*)stackFrame.AddrPC.Offset);
		}
	}

	SymCleanup(process);
}

CORE_API FString FWindowsPlatformMisc::GetExceptionCodeString(DWORD code)
{
	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION: return TEXT("EXCEPTION_ACCESS_VIOLATION");
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return TEXT("EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
	case EXCEPTION_BREAKPOINT: return TEXT("EXCEPTION_BREAKPOINT");
	case EXCEPTION_DATATYPE_MISALIGNMENT: return TEXT("EXCEPTION_DATATYPE_MISALIGNMENT");
	case EXCEPTION_FLT_DENORMAL_OPERAND: return TEXT("EXCEPTION_FLT_DENORMAL_OPERAND");
	default: return FString::PrintF(TEXT("Unknown exception 0x%08X"), code);
	}
}
