#include "Containers/DarkString.h" 

#include "Containers/Array.h"



FString::FString(const ANSICHAR* Str)
{
	int32 Lenght = strlen(Str) + 1;
	_string.resize(Lenght);

	size_t ConvertedChars = 0;

	mbstowcs_s(NULL, _string.data(), Lenght, Str, Lenght - 1);
}

FString::FString(const std::wstring& Str)
{
	_string = Str;
}

std::string FString::ToString() const
{
	std::string Result;
	int32 Lenght = wcslen(_string.c_str()) + 1;
	Result.resize(Lenght);

	wcstombs_s(NULL, Result.data(), Lenght, _string.data(), Lenght - 1);

	return Result;
}


FString::FString(const std::string& Str)
	: FString(Str.data())
{


}

FString FString::PrintFInternal(const TCHAR* Fmt, ...)
{
	//int32 BufferSize = 512;
	//TCHAR StartingBuffer[512];
	//TArray<TCHAR> Buffer(StartingBuffer);
	//int32 Result = -1;


	//va_list Args;
	//va_start(Args, Fmt);
	//Result = _vsnwprintf(Buffer.GetData(), BufferSize, Fmt, Args);


	//va_end(Args);



	//return FString();



	int32 BufferSize = 512;
	TCHAR StartingBuffer[512];
	//TArray<TCHAR> TBuffer = StartingBuffer;
	TArray<TCHAR> Buffer(StartingBuffer);
	int32 Result = -1;


	GET_VARARGS_RESULT(Buffer.GetData(), BufferSize, BufferSize - 1, Fmt, Fmt, Result);

	//va_list args;
	//va_start(args, Fmt);

	//Result = _vsnwprintf(Buffer.GetData(), BufferSize, Fmt, args);
	//va_end(args);

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

FString::FString(const WIDECHAR* Str)
{
	_string = Str;
}


int32 FString::Len() const
{
	return _string.size();
}