#include "Containers/DarkString.h"
#include "Misc/VarArgs.h"
#include "Containers/Array.h"

FString::FString(const ANSICHAR* Other)
{
	FPlatformString::MultiByteToWideChar(Other, *this);
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