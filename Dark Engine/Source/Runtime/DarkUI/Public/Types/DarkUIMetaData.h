#pragma once
#include "Platform/Platform.h"
#include "Containers/DarkString.h"

class IDarkUIMetaData
{
public:
	template<class T>
	bool IsOfType() const
	{
		return IsOfTypeImpl(T::GetTypeID());
	}

	virtual ~IDarkUIMetaData() {}
protected:
	virtual bool IsOfTypeImpl(const FString& Type) const
	{
		return false;
	}
};

#define DUI_METADATA_TYPE(TYPE, BASE) \
	static const FString& GetTypeID() {static FString Type(TEXT(#TYPE)); return Type;} \
	virtual bool IsOfTypeImpl(const FString& Type) const override {return GetTypeID() == Type || BASE::IsOfTypeImpl(Type);}