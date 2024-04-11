#pragma once


#include <string>
#include <algorithm>
#include <HAL/Platform.h>
#include "Misc/VarArgs.h"








class FString
{
public:
	FString() : _string(L"") {}
	FString(const ANSICHAR* Str);
	FString(const WIDECHAR* Str);
	FString(uint32 Size) : _string(Size ,L'\0') {}

	FString(const std::wstring& Str);
	FString(const std::string& Str);

	FString(TCHAR* Str) : _string(Str) {}

	FString(const FString& Str)
	{
		_string = Str._string;
	}

	FString& AppendV(const TCHAR* Fmt, va_list Args)
	{
		TCHAR Buf[512];
		int32 Res = _vsnwprintf(Buf, 512, Fmt, Args);
		if (Res >= 0 && Res < sizeof(Buf))
		{
			*this += Buf;
		}
		return *this;
	}


	template <typename ...TypeArgs>
	static FString PrintF(const TCHAR* Fmt, TypeArgs... Args)
	{
		return PrintFInternal(Fmt, Args...);
	}
	static FString PrintFInternal(const TCHAR* Fmt, ...);


	FString& operator=(const TCHAR* Str) { _string = Str; return *this; }
	FString& operator=(const FString& Str) { _string = Str._string; return *this; }

	const char* operator-()
	{
		return GetStr();
	}

	const char* operator!()
	{
		return GetUTF8();
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

	//operator const TCHAR*() const
	//{
	//	return _string.data();
	//}
	//
	//operator TCHAR*()
	//{
	//	return _string.data();
	//}

	//operator const ANSICHAR* ()
	//{
	//	return GetStr();
	//}
	//operator ANSICHAR* ()
	//{
	//	if (bChangedString)
	//	{
	//		_tempString = ToString();
	//		bChangedString = false;
	//	}
	//	return _tempString.data();
	//}


	template<typename T>
	static FString NumToString(T Val)
	{
		return std::to_wstring(Val);
	}
	

	const char* GetStr()
	{
		if (bChangedString)
		{
			_tempString = ToString();
			bChangedString = false;
		}
		return _tempString.c_str();
	}

	const char* GetUTF8();




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

	bool operator==(const FString& R) const
	{
		return _string == R._string;
	}

	bool operator<(const FString& R) const
	{
		return _string < R._string;
	}
	bool operator>(const FString& R) const
	{
		return _string > R._string;
	}


	decltype(auto) begin() { return _string.begin(); }
	decltype(auto) end() { return _string.end(); }









private:
	std::wstring _string;
	std::string _tempString;

	bool bChangedString = true;

private:
	friend struct std::hash<FString>;

};


template<>
struct std::hash<FString>
{
	std::size_t operator()(const FString& Key) const
	{
		using std::size_t;
		using std::hash;
		using std::wstring;

		return hash<wstring>()(Key._string);
	}
};