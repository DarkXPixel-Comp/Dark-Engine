#pragma once
#include "CoreTypes.h"
#include "Misc/AssertionMacros.h"

class FRefCountedObject
{
public:
	FRefCountedObject() : NumRefs(0) {}
	virtual ~FRefCountedObject() { check(!NumRefs); }
	FRefCountedObject(const FRefCountedObject& Rhs) = delete;
	FRefCountedObject& operator=(const FRefCountedObject& Rhs) = delete;
	uint32 AddRef() const
	{
		return uint32(++NumRefs);
	}
	uint32 Release() const
	{
		uint32 Refs = uint32(--NumRefs);
		if (Refs == 0)
		{
			delete this;
		}
		return Refs;
	}
	uint32 GetRefCount() const
	{
		return uint32(NumRefs);
	}
private:
	mutable int32 NumRefs;
};
