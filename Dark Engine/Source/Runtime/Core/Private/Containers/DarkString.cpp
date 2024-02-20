#include "Containers/DarkString.h"



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

FString::FString(const WIDECHAR* Str)
{
	_string = Str;
}


int32 FString::Len() const
{
	return _string.size();
}