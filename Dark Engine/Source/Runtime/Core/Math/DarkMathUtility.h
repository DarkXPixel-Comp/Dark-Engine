#pragma once

#include <DirectXMath.h>


using FVector = DirectX::XMFLOAT4;


static FVector operator+(const FVector& v1, const FVector& v2)
{
	FVector result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	result.w = v1.w;

	return result;
}

//
//
//class FVector
//{
//public:
//	FVector(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f);
//	FVector(XMFLOAT2 vec4);
//	FVector(XMFLOAT3 vec4);
//	FVector(XMFLOAT4 vec4);
//	FVector(const FVector& vec);
//
//
//	FVector& operator=(const FVector& vec);
//
//
//
//public:
//	FVector operator-() { return GetNegativeVector(); }
//	FVector operator-(const FVector& V);
//	FVector operator*(float Scale);
//
//	FVector GetNegativeVector();
//
//	FVector Normalize4();
//	FVector GetNormalize4Vector();
//
//	FVector Normalize();
//	FVector GetNormalizeVector();
//
//	float Length();
//
//
//	DirectX::XMVECTOR ToXMVector();
//
//	DirectX::XMVECTOR ToXMVector(FVector vec);
//	static FVector FromXMVector(DirectX::XMVECTOR vec);
//
//
//
//public:
//	float X;
//	float Y;
//	float Z;
//	float W;
//
//private:
//
//
//};