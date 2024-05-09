#include <Math/Vector.h>
#include "Math/Rotator.h"
#include "Math/Vector4.h"
#include "Math/Matrix.h"
#include "Math/IntPoint.h"


template<> const FRotator3f FRotator3f::ZeroRotator(0, 0, 0);
template<> const FRotator3d FRotator3d::ZeroRotator(0, 0, 0);

template<> const FVector3f FVector3f::ZeroVector(0, 0, 0);
template<> const FVector3f FVector3f::OneVector(1, 1, 1);
template<> const FVector3f FVector3f::UpVector(0, 1, 0);
template<> const FVector3f FVector3f::DownVector(0, -1, 0);
template<> const FVector3f FVector3f::ForwardVector(0, 0, 1);
template<> const FVector3f FVector3f::BackwardVector(0, 0, -1);
template<> const FVector3f FVector3f::RightVector(1, 0, 0);
template<> const FVector3f FVector3f::LeftVector(-1, 0, 0);
template<> const FVector3f FVector3f::XAxisVector(1, 0, 0);
template<> const FVector3f FVector3f::YAxisVector(0, 1, 0);
template<> const FVector3f FVector3f::ZAxisVector(0, 0, 1);
template<> const FVector3d FVector3d::ZeroVector(0, 0, 0);
template<> const FVector3d FVector3d::OneVector(1, 1, 1);
template<> const FVector3d FVector3d::UpVector(0, 1, 0);
template<> const FVector3d FVector3d::DownVector(0, -1, 0);
template<> const FVector3d FVector3d::ForwardVector(0, 0, 1);
template<> const FVector3d FVector3d::BackwardVector(0, 0, -1);
template<> const FVector3d FVector3d::RightVector(1, 0, 0);
template<> const FVector3d FVector3d::LeftVector(-1, 0, 0);
template<> const FVector3d FVector3d::XAxisVector(1, 0, 0);
template<> const FVector3d FVector3d::YAxisVectorS(0, 1, 0);
template<> const FVector3d FVector3d::ZAxisVector(0, 0, 1);
template<> const FVector4f FVector4f::ZeroVectorOneW(0, 0, 0, 1);

template<> const FIntPoint FIntPoint::ZeroValue(0, 0);


template<> const FMatrix FMatrix::Identity(1, 0, 0, 0,
										   0, 1, 0, 0,
										   0, 0, 1, 0,
										   0, 0, 0, 1);








template struct DE::Math::TVector<float>;
template struct DE::Math::TVector<double>;
