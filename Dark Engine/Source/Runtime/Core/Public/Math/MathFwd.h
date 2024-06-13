#pragma once
#include <HAL/Platform.h>
#include "Vector2D.h"
#include "Vector.h"
#include "Rotator.h"
#include "Matrix.h"
#include "Plane.h"
#include "IntPoint.h"
#include "IntRect.h"
#include "Vector4.h"
#include "PersperctiveMatrix.h"
#include "InverseRotationMatrix.h"
//#include "Transform.h"


namespace DE::Math
{
	template<typename T> struct TVector;
	template<typename T> struct TVector2;
	template<typename T> struct TRotator;
	template<typename T> struct TMatrix;
	template<typename T> struct TIntPoint;
	template<typename T> struct TIntRect;
	template<typename T> struct TVector4;
	template<typename T> struct TReversedZPerspectiveMatrix;
	template<typename T> struct TInverseRotationMatrix;
	template<typename T> struct TPlane;
	template<typename T> struct TTransform;
};


// ISPC forward declarations
namespace ispc
{
	struct FVector;
	struct FVector2D;
	struct FVector4;
	struct FQuat;
	struct FMatrix;
	struct FPlane;
	struct FTransform;
	struct FIntPoint;
	struct FIntRect;
	struct FReversedZPerspectiveMatrix;
	struct FInverseRotationMatrix;
	struct FTransform;
};


using FVector =							DE::Math::TVector<double>;
using FVector2D =						DE::Math::TVector2<double>;
using FRotator =						DE::Math::TRotator<double>;


using FVector3d =						DE::Math::TVector<double>;
using FVector2d =						DE::Math::TVector2<double>;
using FRotator3d =						DE::Math::TRotator<double>;

using FVector3f =						DE::Math::TVector<float>;
using FVector2f =						DE::Math::TVector2<float>;
using FRotator3f =						DE::Math::TRotator<float>;

using FVector4f =						DE::Math::TVector4<float>;

using FMatrix =							DE::Math::TMatrix<double>;
using FMatrix4x4f =						DE::Math::TMatrix<float>;
using FMatrix4x4d =						DE::Math::TMatrix<double>;

using FReversedZPerspectiveMatrix =		DE::Math::TReversedZPerspectiveMatrix<double>;
using FReversedZPerspectiveMatrix4x4d =	DE::Math::TReversedZPerspectiveMatrix<double>;
using FReversedZPerspectiveMatrix4x4f = DE::Math::TReversedZPerspectiveMatrix<float>;

using FInverseRotationMatrix =			DE::Math::TInverseRotationMatrix<double>;
using FInverseRotationMatrix4x4f =		DE::Math::TInverseRotationMatrix<float>;
using FInverseRotationMatrix4x4d =		DE::Math::TInverseRotationMatrix<double>;

using FIntPoint =						DE::Math::TIntPoint<int32>;
using FIntRect =						DE::Math::TIntRect<int32>;

using FPlane =							DE::Math::TPlane<double>;
using FTransform =						DE::Math::TTransform<double>;