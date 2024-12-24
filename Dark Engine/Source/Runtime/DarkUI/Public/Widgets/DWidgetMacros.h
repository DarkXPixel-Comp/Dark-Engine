#pragma once

#include "Misc/Attribute.h"
#include "Math/Vector2.h"


#define DUI_PRIVATE_ATTRIBUTE_VARIABLE(AttributeType, AttributeName) \
	TAttribute<AttributeType> _##AttributeName

#define DUI_PRIVATE_ARGUMENT_VARIABLE(ArgumentType, ArgumentName) \
	ArgumentType _##ArgumentName


#define DUI_PRIVATE_ATTRIBUTE_FUNCTION(AttributeType, AttributeName) \
	WidgetArgsType& AttributeName(TAttribute<AttributeType> InAttribute) \
	{  \
		_##AttributeName = std::move(InAttribute);\
		return static_cast<WidgetArgsType*>(this)->Me();\
	}	

#define DUI_PRIVATE_ARGUMENT_FUNCTION(ArgumentType, ArgumentName) \
	WidgetArgsType& ArgumentName(ArgumentType InArgument) \
	{	 \
		_##ArgumentName = InArgument;  \
		return static_cast<WidgetArgsType*>(this)->Me();\
	}


#define DarkUI_ARGUMENT(ArgumentType, ArgumentName) \
	DUI_PRIVATE_ARGUMENT_VARIABLE(ArgumentType, ArgumentName); \
	DUI_PRIVATE_ARGUMENT_FUNCTION(ArgumentType, ArgumentName);
	
#define DarkUI_ATTRIBUTE(AttributeType, AttributeName) \
	DUI_PRIVATE_ATTRIBUTE_VARIABLE(AttributeType, AttributeName); \
	DUI_PRIVATE_ATTRIBUTE_FUNCTION(AttributeType, AttributeName);

struct FDarkUIBaseNamedArgs
{
	FDarkUIBaseNamedArgs() = default;

	DUI_PRIVATE_ATTRIBUTE_VARIABLE(bool, IsEnabled) = true;
	DUI_PRIVATE_ARGUMENT_VARIABLE(float, RenderOpacity) = 1.f;
	DUI_PRIVATE_ARGUMENT_VARIABLE(bool, EnabledAttributesUpdate) = true;
	DUI_PRIVATE_ATTRIBUTE_VARIABLE(FVector2d, RenderTransformPivot) = FVector2d::ZeroVector;
};


template<typename InWidgetType>
struct TDarkUIBaseNamedArgs : public FDarkUIBaseNamedArgs
{
	typedef InWidgetType WidgetType;
	typedef typename WidgetType::FArguments WidgetArgsType;

	DUI_PRIVATE_ATTRIBUTE_FUNCTION(bool, IsEnabled);
	DUI_PRIVATE_ARGUMENT_FUNCTION(float, RenderOpacity);
	DUI_PRIVATE_ARGUMENT_FUNCTION(bool, EnabledAttributesUpdate);
	DUI_PRIVATE_ATTRIBUTE_FUNCTION(FVector2d, RenderTransformPivot);


	WidgetArgsType& Me()
	{
		return *(static_cast<WidgetArgsType*>(this));
	}

};


#define DarkUI_BEGIN_ARGS(InWidgetType) \
	public: \
	struct FArguments : public TDarkUIBaseNamedArgs<InWidgetType> \
	{ \
		typedef FArguments WidgetArgsType; \
		typedef InWidgetType WidgetType; \
		FORCENOINLINE FArguments()


#define DarkUI_END_ARGS() \
	};


namespace TRequiredArgs
{
	struct T0RequiredArgs
	{
		template<class WidgetType>
		void CallConstruct(WidgetType* Widget, const typename WidgetType::FArguments& WithNamedArgs) const
		{
			Widget->Construct(WithNamedArgs);
		}
	};

	template<typename Arg0Type>
	struct T1RequiredArgs
	{
		T1RequiredArgs(Arg0Type&& InArg0) :
			Arg0(InArg0)
		{}

		template<class WidgetType>
		void CallConstruct(WidgetType* Widget, const typename WidgetType::FArguments& WithNamedArgs) const
		{
			Widget->Construct(WithNamedArgs, std::forward<Arg0Type>(Arg0));
		}

		Arg0Type& Arg0;
	};

	template<typename Arg0Type, typename Arg1Type>
	struct T2RequiredArgs
	{
		T2RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1)
			: Arg0(InArg0)
			, Arg1(InArg1)
		{
		}

		template<class WidgetType>
		void CallConstruct(WidgetType* Widget, const typename WidgetType::FArguments& WithNamedArgs) const
		{
			// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
			Widget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1));
		}

		Arg0Type& Arg0;
		Arg1Type& Arg1;
	};

	template<typename Arg0Type, typename Arg1Type, typename Arg2Type>
	struct T3RequiredArgs
	{
		T3RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2)
			: Arg0(InArg0)
			, Arg1(InArg1)
			, Arg2(InArg2)
		{
		}

		template<class WidgetType>
		void CallConstruct(WidgetType* Widget, const typename WidgetType::FArguments& WithNamedArgs) const
		{
			// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
			Widget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1), std::forward<Arg2Type>(Arg2));
		}

		Arg0Type& Arg0;
		Arg1Type& Arg1;
		Arg2Type& Arg2;
	};

	template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type>
	struct T4RequiredArgs
	{
		T4RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3)
			: Arg0(InArg0)
			, Arg1(InArg1)
			, Arg2(InArg2)
			, Arg3(InArg3)
		{
		}

		template<class WidgetType>
		void CallConstruct(WidgetType* Widget, const typename WidgetType::FArguments& WithNamedArgs) const
		{
			// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
			Widget->Construct(WithNamedArgs, std::forward<Arg0Type>(Arg0), std::forward<Arg1Type>(Arg1), std::forward<Arg2Type>(Arg2), std::forward<Arg3Type>(Arg3));
		}

		Arg0Type& Arg0;
		Arg1Type& Arg1;
		Arg2Type& Arg2;
		Arg3Type& Arg3;
	};

	template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	struct T5RequiredArgs
	{
		T5RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3, Arg4Type&& InArg4)
			: Arg0(InArg0)
			, Arg1(InArg1)
			, Arg2(InArg2)
			, Arg3(InArg3)
			, Arg4(InArg4)
		{
		}

		template<class WidgetType>
		void CallConstruct(WidgetType* Widget, const typename WidgetType::FArguments& WithNamedArgs) const
		{
			// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
			Widget->Construct(WithNamedArgs, std::forward<Arg0Type>(Arg0), std::forward<Arg1Type>(Arg1), std::forward<Arg2Type>(Arg2), std::forward<Arg3Type>(Arg3), std::forward<Arg4Type>(Arg4));
		}

		Arg0Type& Arg0;
		Arg1Type& Arg1;
		Arg2Type& Arg2;
		Arg3Type& Arg3;
		Arg4Type& Arg4;
	};


	FORCEINLINE T0RequiredArgs MakeRequiredArgs()
	{
		return T0RequiredArgs();
	}

	template<typename Arg0Type>
	T1RequiredArgs<Arg0Type&&> MakeRequiredArgs(Arg0Type&& InArg0)
	{
		return T1RequiredArgs<Arg0Type&&>(std::forward<Arg0Type>(InArg0));
	}

	template<typename Arg0Type, typename Arg1Type>
	T2RequiredArgs<Arg0Type&&, Arg1Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1)
	{
		return T2RequiredArgs<Arg0Type&&, Arg1Type&&>(std::forward<Arg0Type>(InArg0), std::forward<Arg1Type>(InArg1));
	}

	template<typename Arg0Type, typename Arg1Type, typename Arg2Type>
	T3RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2)
	{
		return T3RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&>(std::forward<Arg0Type>(InArg0), std::forward<Arg1Type>(InArg1), std::forward<Arg2Type>(InArg2));
	}

	template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type>
	T4RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3)
	{
		return T4RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&>(std::forward<Arg0Type>(InArg0), std::forward<Arg1Type>(InArg1), std::forward<Arg2Type>(InArg2), std::forward<Arg3Type>(InArg3));
	}

	template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	T5RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&, Arg4Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3, Arg4Type&& InArg4)
	{
		return T5RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&, Arg4Type&&>(std::forward<Arg0Type>(InArg0), std::forward<Arg1Type>(InArg1), std::forward<Arg2Type>(InArg2), std::forward<Arg3Type>(InArg3), std::forward<Arg4Type>(InArg4));
	}


}

template<class WidgetType, typename RequiredArgsPayloadType>
struct TSlateDecl
{
	TSlateDecl(const ANSICHAR* InType, const ANSICHAR* InFile, int32 Line, RequiredArgsPayloadType&& InRequiredArgs) :
		RequiredArgs(InRequiredArgs)
	{
		Widget = MakeShareble(new WidgetType());
	}

	TSharedPtr<WidgetType> operator <<=(const typename WidgetType::FArguments& InArgs)&&
	{
		Widget->DUIWidgetConstruct(InArgs);
		RequiredArgs.CallConstruct(Widget.get(), InArgs);

		return std::move(Widget);
	}


	TSharedPtr<WidgetType> Widget;
	RequiredArgsPayloadType& RequiredArgs;
};

template<typename WidgetType, typename RequiredArgsPayloadType>
TSlateDecl<WidgetType, RequiredArgsPayloadType> MakeTDecl(const ANSICHAR* Type, const ANSICHAR* File, int32 Line, RequiredArgsPayloadType&& RequiredArgs)
{
	return TSlateDecl<WidgetType, RequiredArgsPayloadType>(Type, File, Line, std::forward<RequiredArgsPayloadType>(RequiredArgs));
}


#define DUINew(WidgetType, ...) \
	MakeTDecl<WidgetType>(#WidgetType, __FILE__, __LINE__, TRequiredArgs::MakeRequiredArgs( __VA_ARGS__ )) <<= WidgetType::FArguments()
