#pragma once

#include <string>
#include <algorithm>
#include <HAL/Platform.h>


//using FString = std::string;






class FString
{
public:
	FString() : _string(L"") {}
	FString(const ANSICHAR* Str);
	FString(const WIDECHAR* Str);
	FString(uint32 Size) : _string(Size ,L'\0') {}

	FString(const std::wstring& Str);
	FString(const std::string& Str);

	FString(const FString& Str)
	{
		_string = Str._string;
	}




	FString& operator=(const TCHAR* Str);
	FString& operator=(const FString& Str) { _string = Str._string; return *this; }

	const char* operator-()
	{
		return GetStr();
	}

	TCHAR& operator[](int32 Index);


	FString& Replace(TCHAR Str1, TCHAR Str2)
	{
		std::replace(_string.begin(), _string.end(), Str1, Str2);
		return *this;
	}


	int32 Len() const;
	decltype(auto) Data() const { return _string.c_str(); }
	std::string ToString() const;

	static FString FloatToString(float Val)
	{
		return std::to_wstring(Val);
	}

	const char* GetStr()
	{
		_tempString = ToString();
		return _tempString.c_str();
	}




	FString operator+(FString R)
	{
		std::wstring Result;
		Result.resize(_string.size() + R._string.size());
		
		wcscat(Result.data(), _string.c_str());
		wcscat(Result.data(), R._string.c_str());
		return Result;

	}
	FString& operator+=(FString R)
	{
		_string += R._string;
		return *this;
	}

	const TCHAR* operator*() const { return _string.data(); }
	TCHAR* operator*() { return _string.data(); }

	bool operator==(FString R)
	{
		return _string == R._string;
	}

	decltype(auto) begin() { return _string.begin(); }
	decltype(auto) end() { return _string.end(); }









private:
	std::wstring _string;
	std::string _tempString;
};