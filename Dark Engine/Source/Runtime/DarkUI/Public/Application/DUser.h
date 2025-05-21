#pragma once
#include "Memory/MemoryCore.h"
#include "CoreTypes.h"
#include "Containers/Array.h"
#include "Math/Vector2.h"


class DUIWindow;
class DUIWidget;
class IToolTip;



class FDUIUser : std::enable_shared_from_this<FDUIUser>
{
public:
	DARKUI_API virtual ~FDUIUser();

	DARKUI_API bool IsWindowHousingInteractiveTooltip(const TSharedPtr<const DUIWindow>& WindowTest) const;

private:
	DARKUI_API FDUIUser(int32 InUserIndex);



	enum class ETooltipOffsetDirection : uint8
	{
		Undetermined,
		Down,
		Right
	};

	struct FActiveTooltipInfo
	{
		void Reset();
		TWeakPtr<IToolTip> Tooltip;

		TWeakPtr<DUIWidget> TooltipVisualizer;

		TWeakPtr<DUIWidget> SourceWidget;

		FVector2f DesiredLocation = FVector2f::ZeroVector;

		FVector2f DesiredSize = FVector2f::ZeroVector;

		double SummonTime = 0.0;

		ETooltipOffsetDirection OffsetDirection = ETooltipOffsetDirection::Undetermined;
	};

	FActiveTooltipInfo ActiveTooltipInfo;


private:
	int32 UserIndex = -1;

	TWeakPtr<DUIWindow> TooltipWindow;

};