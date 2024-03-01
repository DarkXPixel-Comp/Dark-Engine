#pragma once

#include <string>
#include <HAL/Platform.h>


//using FString = std::string;






class FString
{
public:
	FString() : _string(L"") {}
	FString(const ANSICHAR* Str);
	FString(const WIDECHAR* Str);

	FString(const std::wstring& Str);
	FString(const std::string& Str);

	FString(const FString& Str)
	{
		_string = Str._string;
	}


	FString& operator=(const TCHAR* Str);
	FString& operator=(const FString& Str) { _string = Str._string; return *this; }

	TCHAR& operator[](int32 Index);



	int32 Len() const;
	decltype(auto) Data() const { return _string.c_str(); }
	std::string ToString() const;




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
};