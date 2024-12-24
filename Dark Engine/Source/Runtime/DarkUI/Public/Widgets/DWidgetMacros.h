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


namespace RequiredArgs
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



	template<class WidgetType, typename RequiredArgsPayloadType>
	struct TSlateDecl
	{
		TSlateDecl(const ANSICHAR* InType, const ANSICHAR* InFile, int32 Line, RequiredArgsPayloadType&& InRequiredArgs)
		{

		}
	};


}

