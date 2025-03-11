#pragma once
#include <string>
#include <algorithm>
#include "Platform/Platform.h"
#include "Platform/PlatformString.h"

class FString
{
	using StringType = std::basic_string<TCHAR>;

public:
	CORE_API FString() {}

	CORE_API FString(const FString& Other)
	{
		String = Other.String;
	}

	CORE_API FString(uint32 Size) : String(Size, L'\0')
	{}

	CORE_API FString(FString&& Other) noexcept
	{
		String = std::move(Other.String);
	}

	CORE_API FString(const ANSICHAR* Other);

	CORE_API FString(const TCHAR* Other) : String(Other) {}

	CORE_API FString(const StringType& Other) : String(Other) {}

	CORE_API FString& operator=(const TCHAR* Other)
	{
		String = Other;
		return *this;
	}

	CORE_API std::string ToString() const;

	CORE_API std::string operator!() const;

	CORE_API FString& operator=(const FString& Other)
	{
		String = Other.String;
		return *this;
	}

	CORE_API FString operator+(const FString& Other) const
	{
		StringType Result = String + Other.String;
		return Result;
	}

	bool operator==(const FString& Other) const
	{
		return String == Other.String;
	}

	CORE_API FString& operator+=(const FString& Other)
	{
		String += Other.String;
		return *this;
	}


	CORE_API TCHAR* operator*()
	{
		return String.data();
	}

	CORE_API const TCHAR* operator*() const
	{
		return String.data();
	}

	FString& Replace(TCHAR Str1, TCHAR Str2)
	{
		std::replace(String.begin(), String.end(), Str1, Str2);
		return *this;
	}

	CORE_API FString& AppendV(const TCHAR* Fmt, va_list Args)
	{
		TCHAR Buffer[512];
		int32 Result = FGenericString::StringPrintf(Buffer, 512, Fmt, Args);

		if (Result >= 0 && Result < sizeof(Buffer))
		{
			*this += Buffer;
		}
		return *this;
	}

	CORE_API void PopBack()
	{
		String.erase(String.begin() + String.size() - 1);
	}


	CORE_API void PopFirst()
	{
		String.erase(String.begin());
		//bChangedString = true;
	}

	CORE_API TCHAR Back() const
	{
		return String.size() != 0 ? String.back() : TEXT('\0');
	}

	CORE_API TCHAR First() const
	{
		return String.size() != 0 ? String.front() : TEXT('\0');
	}

	CORE_API size_t size() const
	{
		return String.size();
	}

	CORE_API auto Lenght() const
	{
		return String.size();
	}

	CORE_API bool Contains(const FString& Other) const
	{
		return String.find(Other.String) != StringType::npos;
	}

	CORE_API decltype(auto) begin()	const
	{
		return String.begin();
	}

	CORE_API decltype(auto) begin()
	{
		return String.begin();
	}

	CORE_API decltype(auto) end()
	{
		return String.end();
	}

	CORE_API decltype(auto) end() const
	{
		return String.end();
	}

	CORE_API bool ContainsWithoutCase(const FString& Other) const
	{
		StringType Temp0(Lenght(), TEXT('\0')); std::transform(begin(), end(), Temp0.begin(), ::towlower);
		StringType Temp1(Other.Lenght(), TEXT('\0')); std::transform(Other.begin(), Other.end(), Temp1.begin(), ::towlower);

		return Temp0.find(Temp1) != StringType::npos;
	}

	CORE_API static int32 FromString(int32& Value, const TCHAR* Buffer)
	{
		return Value = FPlatformString::StringToInt32(Buffer, nullptr, 10);
	}

	CORE_API static float FromString(float& Value, const TCHAR* Buffer)
	{
		//Value = std::stof(Buffer);
		//Value = wcstof(Buffer, nullptr);
		return Value = FPlatformString::StringToFloat(Buffer, nullptr);
	}
	CORE_API static bool FromString(bool& Value, const TCHAR* Buffer)
	{
		std::wstring Buf = Buffer;
		std::transform(Buf.begin(), Buf.end(), Buf.begin(), FPlatformString::ToLower);

		if (Buf == TEXT("true"))
		{
			Value = true;
		}
		else if (Buf == TEXT("false"))
		{
			Value = false;
		}
		else
		{
			Value = FPlatformString::StringToInt32(Buffer, nullptr, 10);
		}
		return Value;
	}
	CORE_API static FString FromString(FString& Value, const TCHAR* Buffer)
	{
		Value = Buffer;
		return Value;
	}




	template <typename ...TypeArgs>
	static FString PrintF(const TCHAR* Fmt, TypeArgs... Args)
	{
		return PrintFInternal(Fmt, Args...);
	}
	CORE_API static FString PrintFInternal(const TCHAR* Fmt, ...);

private:
	StringType String;

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

		return hash<wstring>()(Key.String);
	}
};