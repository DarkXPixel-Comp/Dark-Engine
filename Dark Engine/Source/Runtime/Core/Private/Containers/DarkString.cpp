#include "Containers/DarkString.h" 

#include "Containers/Array.h"
#include <Windows.h>
#include <xlocale>



FString::FString(const ANSICHAR* Str)
{
	int32 Lenght = (int32)strlen(Str);
	int sz = MultiByteToWideChar(CP_UTF8, 0, Str, Lenght, 0, 0);
	if (sz < 1)
	{
		_string = TEXT("");
	}
	else
	{
		_string.resize(sz);
		MultiByteToWideChar(CP_UTF8, 0, Str, Lenght,
			_string.data(), sz);
	}
}

FString::FString(const std::wstring& Str)
{
	_string = Str;
}

std::string FString::ToString() const
{
	std::string Result;
	int32 Lenght = (int32)wcslen(_string.c_str()) + 1;
	Result.resize(Lenght);

	wcstombs_s(NULL, Result.data(), Lenght, _string.data(), Lenght - 1);

	return Result;
}

const char* FString::GetUTF8() const		 // Change on Multiplatform or FPlatformString
{
	if (bChangedString)
	{
		std::string& Result = _tempString;

		int sz = WideCharToMultiByte(CP_UTF8, 0, _string.data(), -1, 0, 0, 0, 0);
		Result.resize(sz);
		WideCharToMultiByte(CP_UTF8, 0, _string.data(), -1, Result.data(), sz, 0, 0);

		//_tempString = std::move(Result);
		bChangedString = false;
	}
	return _tempString.c_str();
}


const char8_t* FString::GetUTF() const
{
	return reinterpret_cast<const char8_t*>(GetUTF8());
}

FString::FString(const std::string& Str)
	: FString(Str.data())
{


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

FString::FString(const WIDECHAR* Str)
{
	_string = Str;
}


int32 FString::Len() const
{
	return (int32)_string.size();
}