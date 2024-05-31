#pragma once
#include "Containers/Array.h"
class GClass;

template <typename T>
struct TRegistrationInfo
{
	using TType = T;

	TType* InnerSingleton = nullptr;
	TType* OuterSingleton = nullptr;
};

using FClassRegistrationInfo = TRegistrationInfo<GClass>;


template <typename T>
class TDeferredRegistry
{
public:
	using TInfo = T;
	using TType = typename T::TType;
	struct FRegistrant
	{
		TType* (*OuterRegisterFn)();
		TType* (*InnerRegisterFn)();
		TInfo* Info;
	};

	void AddRegistration(TType* (*InOuterRegister)(), TType* (*InInnerRegister)(), const TCHAR* InName, TInfo& InInfo)
	{
		//FRegistrant Registrant;
		//Registrant.Info = InInfo;
		//Registrant.InnerRegisterFn = InInnerRegister;
		//Registrant.OuterRegisterFn = InOuterRegister;
		Registrations.Add(FRegistrant{InOuterRegister, InInnerRegister, &InInfo});
	}

	void DoPendingInnerRegistrations()
	{
		for (auto& i : Registrations)
		{
			InnerRegister(i);
		}
	}

	void DoPendingOuterRegistrations()
	{
		for (auto& i : Registrations)
		{
			OuterRegister(i);
		}
	}

	static TType* InnerRegister(const FRegistrant& Registrant)
	{
		return Registrant.InnerRegisterFn();
	}

	static TType* OuterRegister(const FRegistrant& Registrant)
	{
		return Registrant.OuterRegisterFn();
	}



	TArray<FRegistrant>& GetRegistrations()
	{
		return Registrations;
	}


	static TDeferredRegistry& Get()
	{
		static TDeferredRegistry Registry;
		return Registry;
	}


private:
	TArray<FRegistrant>	Registrations;
	int32 NumRegistrations;


};


using FClassDeferredRegistry = TDeferredRegistry<FClassRegistrationInfo>;


FORCEINLINE void RegisterCompiledInInfo(class GClass* (*InOuterRegister)(), class GClass* (*InInnerRegister)(), const TCHAR* InName, FClassRegistrationInfo& InInfo)
{
	FClassDeferredRegistry::Get().AddRegistration(InOuterRegister, InInnerRegister, InName, InInfo);
}



struct FRegisterCompiledInfo
{
	template<typename...Args>
	FRegisterCompiledInfo(Args&& ...args)
	{
		RegisterCompiledInInfo(std::forward<Args>(args)...);
	}
};