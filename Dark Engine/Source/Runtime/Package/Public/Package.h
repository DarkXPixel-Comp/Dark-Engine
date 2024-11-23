#pragma once
#include "Containers/DarkString.h"



class FPackage
{
public:
	FString GetName() const;
	void SetName(const FString& InName);

	template<typename T>
	void GetPackage(T* OutPackage);
	void GetPackage(void* OutPackage, uint64 InSize);

	template<typename T>
	void WritePackage(T* const InPackage);
	void WritePackage(void* const InPackage, uint64 InSize);

	uint64 GetSize() const;
	




private:
	FString Name;
};