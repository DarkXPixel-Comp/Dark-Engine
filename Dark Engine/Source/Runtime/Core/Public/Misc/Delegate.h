#pragma once
#include "Containers/Array.h"
#include <functional>
#include "Memory/MemoryCore.h"
		   
#define FUNC_DECLARE_MULTICAST_DELEGATE(DelegateName, ReturnType, ...) typedef TMultiCastDelegate<ReturnType, __VA_ARGS__> DelegateName
#define FUNC_DECLARE_DELEGATE(DelegateName, ReturnType, ...) typedef TDelegate<ReturnType, __VA_ARGS__> DelegateName


#define DECLARE_DELEGATE( DelegateName ) FUNC_DECLARE_DELEGATE( DelegateName, void )
#define DECLARE_MULTICAST_DELEGATE( DelegateName ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void )

#define DECLARE_DELEGATE_OneParam( DelegateName, Param1Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type )
#define DECLARE_MULTICAST_DELEGATE_OneParam( DelegateName, Param1Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type )

#define DECLARE_DELEGATE_TwoParams( DelegateName, Param1Type, Param2Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type )
#define DECLARE_MULTICAST_DELEGATE_TwoParams( DelegateName, Param1Type, Param2Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type )

#define DECLARE_DELEGATE_ThreeParams( DelegateName, Param1Type, Param2Type, Param3Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type )
#define DECLARE_MULTICAST_DELEGATE_ThreeParams( DelegateName, Param1Type, Param2Type, Param3Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type )

#define DECLARE_DELEGATE_FourParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type )
#define DECLARE_MULTICAST_DELEGATE_FourParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type )

#define DECLARE_DELEGATE_FiveParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type )
#define DECLARE_MULTICAST_DELEGATE_FiveParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type )

#define DECLARE_DELEGATE_SixParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type )
#define DECLARE_MULTICAST_DELEGATE_SixParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type )

#define DECLARE_DELEGATE_SevenParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type )
#define DECLARE_MULTICAST_DELEGATE_SevenParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type )

#define DECLARE_DELEGATE_EightParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type )
#define DECLARE_MULTICAST_DELEGATE_EightParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type )

#define DECLARE_DELEGATE_NineParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type )
#define DECLARE_MULTICAST_DELEGATE_NineParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type )

#define DECLARE_DELEGATE_RetVal(ReturnValue, DelegateName) FUNC_DECLARE_DELEGATE(DelegateName, ReturnValue)



namespace Core::Delegate
{
	uint64 GenerateContainerId();
}


template<typename InRetValType, typename ...ParamTypes>
class IIContainer
{
	//IIContainer()

public:
	IIContainer()
	{
		Id = Core::Delegate::GenerateContainerId();
	}

	IIContainer(const IIContainer& Other) : Id(Other.Id)
	{}

	virtual ~IIContainer() {}

	virtual InRetValType Call(ParamTypes...) = 0;
	virtual IIContainer<InRetValType, ParamTypes...>* Copy() const = 0;
	uint64 GetId() const
	{
		return Id;
	}

private:
	uint64 Id;
};

template <typename UserClass, typename InRetValType, typename... ParamTypes>
class TMethodContainer : public IIContainer<InRetValType, ParamTypes...>
{
	using FFuncPtr = InRetValType(UserClass::*)(ParamTypes...);
	using FuncPtr = std::function<InRetValType(ParamTypes...)>;
	
	TMethodContainer() = default;
public:
	TMethodContainer(UserClass* cl, FFuncPtr func) : uClass(cl), pFunc(func)
	{

	}

	TMethodContainer(UserClass* cl, FuncPtr func) : uClass(cl), pFunction(func)
	{

	}

	TMethodContainer(const TMethodContainer& Other)
		: uClass(Other.uClass), pFunc(Other.pFunc), IIContainer<InRetValType, ParamTypes...>(Other)
	{}

	TMethodContainer(TMethodContainer&& Other) : IIContainer<InRetValType, ParamTypes...>(std::move(Other))
	{
		uClass = Other.uClass;
		pFunc = Other.pFunc;
		Other.uClass = nullptr;
		Other.pFunc = nullptr;
	}

	bool operator==(const TMethodContainer& Other)
	{
		return pFunc == Other.pFunc && uClass == Other.uClass;
	}

	TMethodContainer<UserClass, InRetValType, ParamTypes...>* Copy() const override
	{
		//TMethodContainer<UserClass, InRetValType, ParamTypes...>* Result = new TMethodContainer<UserClass, InRetValType, ParamTypes...>();
		return new TMethodContainer<UserClass, InRetValType, ParamTypes...>(*this);
	}

	virtual FFuncPtr GetHandle()
	{
		return pFunc;
	}

	virtual bool IsMethod() const
	{
		return true;
	}
	virtual bool IsFunction() const
	{
		return false;
	}

	InRetValType Call(ParamTypes... params)
	{
		return (uClass->*pFunc)(params...);
	}
private:
	FuncPtr pFunction;
	FFuncPtr pFunc;
	UserClass* uClass;
};

template<typename InRetValType, typename... ParamTypes>
class TFuncContainer : public IIContainer<InRetValType, ParamTypes...>
{
	using FuncPtr = std::function<InRetValType(ParamTypes...)>;

	TFuncContainer() = default;

public:
	TFuncContainer(FuncPtr func) : ppFunc(func) {}

	TFuncContainer(const TFuncContainer& Other) : ppFunc(Other.ppFunc), IIContainer<InRetValType, ParamTypes...>(Other)
	{
	}

	TFuncContainer(TFuncContainer&& Other) : ppFunc(std::move(Other.ppFunc)), IIContainer<InRetValType, ParamTypes...>(std::move(Other))
	{

	}

	
	TFuncContainer<InRetValType, ParamTypes...>* Copy() const override
	{
		return new TFuncContainer<InRetValType, ParamTypes...>(*this);
	}

	InRetValType Call(ParamTypes... params)
	{
		return ppFunc(params...);
	}

	virtual bool IsMethod() const
	{
		return false;
	}
	virtual bool IsFunction() const
	{
		return true;
	}

private:
	FuncPtr ppFunc;
};

template <typename InRetValType, typename... ParamTypes>
class TDelegate
{
public:
	TDelegate() = default;
	TDelegate(const TDelegate<InRetValType, ParamTypes...>& Other)
	{
		*this = Other;
	}
	TDelegate& operator=(const TDelegate<InRetValType, ParamTypes...>& Other)
	{
		if (Other.container)
		{
			container.reset(Other.container->Copy());
		}
		return *this;
	}
	TDelegate(TDelegate<InRetValType, ParamTypes...>&&) = default;
	TDelegate& operator=(TDelegate<InRetValType, ParamTypes...>&& Other) = default;

	bool IsBound() const
	{
		return container != nullptr;
	}

	int64 GetHandle() const
	{
		return container ? container->GetId() : 0;
	}

	void Bind(std::function<InRetValType(ParamTypes...)> func)
	{
		//container = new TFuncContainer<InRetValType, ParamTypes...>(std::forward<InRetValType(*)(ParamTypes...)>(Lambda));
		container = MakeUnique(new TFuncContainer<InRetValType, ParamTypes...>(func));
	}

	template<typename UserClass>
	void Bind(UserClass* inUserObject, InRetValType(UserClass::* func)(ParamTypes...))
	{
		container = MakeUnique(new TMethodContainer<UserClass, InRetValType, ParamTypes...>(inUserObject, func));

	}

	bool operator==(const TDelegate& Other) const
	{
		return container && Other.container ? container->GetID() == container->GetID() : container == Other.container;
	}

	void Unbind()
	{
		container.reset();
	}

	InRetValType BroadCast(ParamTypes... params) const
	{
		if (container)
		{
			return container->Call(params...);
		}
	}


	template<typename UserClass>
	static TDelegate<InRetValType, ParamTypes...> Create(UserClass* InUserObject, InRetValType(UserClass::* func)(ParamTypes...))
	{
		TDelegate<InRetValType, ParamTypes...> Delegate;
		Delegate.Bind(InUserObject, func);
		return Delegate;
	}

private:
	TUniquePtr<IIContainer<InRetValType, ParamTypes...>> container;
};

template <typename InRetValType, typename... ParamTypes>
class TMultiCastDelegate
{
	using FFuncPtr = InRetValType(*)(ParamTypes...);

public:
	TMultiCastDelegate() { containers = new TArray<IIContainer<InRetValType, ParamTypes...>*>(); }
	~TMultiCastDelegate() { delete containers; }
	//const void Bind(InRetValType(*func)(ParamTypes...))
	//{
	//	IIContainer<InRetValType, ParamTypes...>* temp = new TFuncContainer<InRetValType, ParamTypes...>(func);

	//	containers->Push(temp);

	//	pFunc = func;
	//}

	const void Bind(std::function<InRetValType(ParamTypes...)> func)
	{
		IIContainer<InRetValType, ParamTypes...>* temp = new TFuncContainer<InRetValType, ParamTypes...>(func);
		containers->Push(temp);
	}

	template<typename UserClass>
	const void Bind(UserClass* inUserObject, InRetValType(UserClass::* func)(ParamTypes...))
	{
		IIContainer<InRetValType, ParamTypes...>* temp = new TMethodContainer<UserClass, InRetValType, ParamTypes...>(inUserObject, func);

		containers->Push(temp);
	}




	void BroadCast(ParamTypes... params)
	{
		for (auto& i : *containers)
		{
			i->Call(params...);
		}

	}

	TArray<IIContainer<InRetValType, ParamTypes...>*>* containers = nullptr;

	//IIContainer<InRetValType, ParamTypes...>* container;
	FFuncPtr pFunc = nullptr;
};
