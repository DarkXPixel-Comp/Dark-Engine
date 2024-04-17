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
			bChangedString = true;
		}
		return *this;
	}

	void PopBack()
	{
		_string.erase(_string.begin() + _string.size() - 1);
		bChangedString = true;
	}

	void PopFirst()
	{
		_string.erase(_string.begin());
		bChangedString = true;
	}

	TCHAR Back() const
	{
		return _string.size() != 0 ? _string.back() : TEXT('\0');
	}

	TCHAR First() const
	{
		return _string.size() != 0 ? _string.front() : TEXT('\0');
	}

	bool Contains(const FString& Other)
	{
		return _string.find(Other._string) != std::wstring::npos;
	}

	static int32 FromString(int32& Value, const TCHAR* Buffer)
	{
		//Value = std::stoi(Buffer);
		Value = wcstol(Buffer, nullptr, 10);
		return Value;
	}

	static float FromString(float& Value, const TCHAR* Buffer)
	{
		//Value = std::stof(Buffer);
		Value = wcstof(Buffer, nullptr);
		return Value;
	}
	static bool FromString(bool& Value, const TCHAR* Buffer)
	{
		//Value = std::stoi(Buffer);
		Value = wcstol(Buffer, nullptr, 10);
		return Value;
	}
	static FString FromString(FString& Value, const TCHAR* Buffer)
	{
		Value = Buffer;
		return Value;
	}





	template <typename ...TypeArgs>
	static FString PrintF(const TCHAR* Fmt, TypeArgs... Args)
	{
		return PrintFInternal(Fmt, Args...);
	}
	static FString PrintFInternal(const TCHAR* Fmt, ...);


	FString& operator=(const TCHAR* Str) { _string = Str; return *this; }
	FString& operator=(const FString& Str) { _string = Str._string; return *this; }

	const char* operator-() const
	{
		return GetStr();
	}

	const char* operator!() const
	{
		return GetUTF8();
	}

	FString operator/(const FString& Other)
	{
		return FString::PrintF(TEXT("%s/%s"), _string.data(), *Other);
	}


	TCHAR& operator[](int32 Index);


	FString& Replace(TCHAR Str1, TCHAR Str2)
	{
		std::replace(_string.begin(), _string.end(), Str1, Str2);
		return *this;
	}


	void Resize(uint32 NewSize)
	{
		_string.resize(NewSize);
	}

	void Reserve(uint32 NewReserve)
	{
		_string.reserve(NewReserve);
	}

	std::wstring& GetNativeString()
	{
		return _string;
	}


	int32 Len() const;
	decltype(auto) Data() const { return _string.c_str(); }
	TCHAR* Data() { return _string.data(); }

	std::string ToString() const;


	template<typename T>
	static FString NumToString(T Val)
	{
		return std::to_wstring(Val);
	}

	template<>
	static FString NumToString(int32 Val)
	{
		return std::to_wstring(Val);
	}
	template<>
	static FString NumToString(float Val)
	{
		return std::to_wstring(Val);
	}
	template<>
	static FString NumToString(bool Val)
	{
		return std::to_wstring(Val);
	}
	template<>
	static FString NumToString(FString Val)
	{
		return Val;
	}


	

	const char* GetStr() const
	{
		_tempString = ToString();
		return _tempString.c_str();
	}

	const char* GetUTF8() const;




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
		return _string.compare(R._string) == 0;
		//return _string == R._string;
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

	mutable std::string _tempString;
	mutable bool bChangedString = true;

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