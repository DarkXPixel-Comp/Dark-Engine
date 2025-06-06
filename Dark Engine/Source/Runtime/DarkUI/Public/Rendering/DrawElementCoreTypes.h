#pragma once
#include "CoreMinimal.h"
#include <tuple>

template<typename DrawElementType>

using FDUIDrawElementArray = TArray<DrawElementType>;

using FDUIDrawElementMap = std::tuple
<
	FDUIDrawElementArray<int32>
>;