#pragma once

#include <string>
#include <HAL/Platform.h>


using FString = std::string;




//const FString operator+(const FString& Str1, const FString& Str2)
//{
//	return Str1 + Str2;
//}





class FStringT : public std::string
{
private:
	
public:

	friend FString operator/(FString&& lhs, const FString& Rhs);












};