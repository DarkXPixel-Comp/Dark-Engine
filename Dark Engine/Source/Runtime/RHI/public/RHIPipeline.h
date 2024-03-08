#pragma once



enum class ERHIPipeline
{
	Graphics = 1 << 0,
	AsyncCompute = 1 << 1,
	None = 0,
	All = Graphics | AsyncCompute
};