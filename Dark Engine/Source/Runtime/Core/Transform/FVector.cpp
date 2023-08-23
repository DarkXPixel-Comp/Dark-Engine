#include "FVector.h"



FVector::FVector(XMFLOAT4 vec4):
    X(vec4.x),
    Y(vec4.y),
    Z(vec4.z),
    W(vec4.w)
{
    
}

FVector::FVector(const FVector& vec):
    X(vec.X),
    Y(vec.Y),
    Z(vec.Z),
    W(vec.W)
{
    

}

FVector::FVector(float x, float y, float z, float w) :
    X(x),
    Y(y),
    Z(z),
    W(w)
{

}


FVector& FVector::operator=(const FVector& vec)
{
    X = vec.X;
    Y = vec.Y;
    Z = vec.Z;
    W = vec.W;

    return *this;
}

FVector FVector::operator-(const FVector& V)
{
    FVector vector;

    vector.X = X - V.X;
    vector.Y = Y - V.Y;
    vector.Z = Z - V.Z;

    return vector;
}

FVector FVector::operator*(float Scale)
{
    FVector vector;

    vector.X = X * Scale;
    vector.Y = Y * Scale;
    vector.Z = Z * Scale;

    return vector;
}

FVector FVector::GetNegativeVector()
{
    FVector vector;
    
    vector.X = -X;
    vector.Y = -Y;
    vector.Z = -Z;

    return vector;
}

FVector FVector::Normalize4()
{
    auto vector = ToXMVector();
    vector = XMVector4Normalize(vector);

    *this = FromXMVector(vector);
    



    return *this;
}


FVector FVector::GetNormalize4Vector()
{
    auto vector = ToXMVector();
    vector = XMVector4Normalize(vector);


    return FromXMVector(vector);
}

FVector FVector::Normalize()
{
    float tW = W;
    W = 0;
    auto vector = ToXMVector();
    vector = XMVector3Normalize(vector);

    *this = FromXMVector(vector);

    W = tW;


    return *this;
}

FVector FVector::GetNormalizeVector()
{
    auto vector = ToXMVector();
    vector = XMVector3Normalize(vector);


    return FromXMVector(vector);
}

float FVector::Length()
{
  /*  m_xmVector = XMVectorSet(X, Y, Z, W);

    XMVector3Normalize


    return XMVectorGetX(XMVector4Length(m_xmVector));*/


    return 0.f;

}

DirectX::XMVECTOR FVector::ToXMVector()
{
    return XMVectorSet(X, Y, Z, W);
}

DirectX::XMVECTOR FVector::ToXMVector(FVector vec)
{
    return XMVectorSet(vec.X, vec.Y, vec.Z, vec.W);
}

FVector FVector::FromXMVector(DirectX::XMVECTOR vec)
{
    XMFLOAT4 vec4 = {};
    XMStoreFloat4(&vec4, vec);
    

    return FVector(vec4);
}


