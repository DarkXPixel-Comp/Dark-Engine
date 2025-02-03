#include "Containers/DarkString.h"
#include "Misc/VarArgs.h"
#include "Containers/Array.h"
#include "Platform/Platform.h"
#include <format>

FString::FString(const ANSICHAR* Other)
{
#if	PLATFORM_UTF8CHAR
#else
	FPlatformString::MultiByteToWideChar(Other, *this);
#endif
}

CORE_API std::string FString::ToString() const
{
#if	PLATFORM_UTF8CHAR
#else
	std::string Result;
	FPlatformString::WideCharToMultiByte(String.c_str(), Result);
	return Result;
#endif
}

CORE_API std::string FString::operator!() const
{
	return ToString();
}

FString FString::PrintFInternal(const TCHAR* Fmt, ...)
{
	int32 BufferSize = 512;
	TCHAR StartingBuffer[512];
	TArray<TCHAR> Buffer(StartingBuffer);
	int32 Result = -1;


	GET_VARARGS_RESULT(Buffer.GetData(), BufferSize, BufferSize - 1, Fmt, Fmt, Result);

	if (Result == -1)
	{
		Buffer.Empty();
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer.Resize(BufferSize);
			GET_VARARGS_RESULT(Buffer.GetData(), BufferSize, BufferSize - 1, Fmt, Fmt, Result);
		}
	}

	Buffer[Result] = TEXT('\0');

	FString ResultString(Buffer.GetData());

	return ResultString;
}
