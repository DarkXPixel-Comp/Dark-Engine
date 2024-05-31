#pragma once
#include "HAL/Platform.h"
#include <utility>


class GObject;
class GClass;



struct FObjectPtr
{
	using FObjectHandle = uint64;
	FObjectPtr() : Handle(reinterpret_cast<FObjectHandle>(nullptr))
	{

	}

	explicit FObjectPtr(ENoInit) {}

	FObjectPtr(const FObjectPtr& Object) : Handle(FObjectHandle(Object.Handle)) {}

	explicit FObjectPtr(GObject* Object): Handle(reinterpret_cast<FObjectHandle>(Object))
	{

	}

	FORCEINLINE bool IsValid() const
	{
		return Handle != 0;
	}

	FORCEINLINE bool operator==(const FObjectPtr& Other) const
	{
		return Handle == Other.Handle;
	}

	FORCEINLINE FObjectPtr& operator=(FObjectPtr&& InOther)
	{
		Handle = std::move(InOther.Handle);
		return *this;
	}
	FORCEINLINE FObjectPtr& operator=(GObject* Other)
	{
		Handle = reinterpret_cast<FObjectHandle>(Other);
		return *this;
	}

	FORCEINLINE FObjectPtr& operator=(decltype(__nullptr))
	{
		Handle = reinterpret_cast<FObjectHandle>(nullptr);
		return *this;
	}

	FORCEINLINE FObjectPtr& operator=(const FObjectPtr& InOther)
	{
		Handle = InOther.Handle;
		return *this;
	}

	FORCEINLINE GObject* operator->() const { return Get(); }
	FORCEINLINE GObject& operator*() const { return *Get(); }


	GObject* Get() const
	{
		return reinterpret_cast<GObject*>(Handle);
	}
	
	GClass* GetClass() const;


private:
	mutable FObjectHandle Handle;


};




template<typename T>
struct TObjectPtr
{
public:
	using ElementType = T;

	TObjectPtr() : ObjectPtr()
	{}

	TObjectPtr(ENoInit) : ObjectPtr(NoInit)
	{}

	TObjectPtr(TObjectPtr<ElementType>&& Other):
		ObjectPtr(std::move(Other.ObjectPtr))
	{}

	TObjectPtr(TObjectPtr<ElementType>& Other):
		ObjectPtr(Other.ObjectPtr)
	{}

	TObjectPtr(FObjectPtr ObjPtr) : ObjectPtr(ObjPtr)
	{}
	
	TObjectPtr(T& Object): 
		ObjectPtr(const_cast<std::remove_const_t<T>*>(&Object))
	{}

	template <typename U>
	TObjectPtr(U&& Other)
	{
		//ObjectPtr = const_cast<std::remove_const_t<T>*>(Other);
		ObjectPtr = static_cast<T*>(Other);
	}

	template <typename U>
	FORCEINLINE TObjectPtr<T>& operator=(U&& Other)
	{
		ObjectPtr = const_cast<std::remove_const_t<T>*>(Other);
		return *this;
	}


	template <typename U>
	FORCEINLINE TObjectPtr<T>& operator=(const TObjectPtr<U>& Other)
	{
		ObjectPtr = Other.ObjectPtr;
	}

	FORCEINLINE ElementType* Get() const
	{
		return (ElementType*)(ObjectPtr.Get());
	}


	



	FORCEINLINE operator T*() const { return Get(); }

	template <typename U>
	explicit FORCEINLINE operator U* () const { return (U*)Get(); }
	explicit FORCEINLINE operator uint64() const { return (uint64)Get(); }

	FORCEINLINE T* operator->() const { return Get(); }
	FORCEINLINE T& operator*() const { return *Get(); }

private:
	union
	{
		FObjectPtr ObjectPtr;

		ElementType* DebugPtr;
	};



};