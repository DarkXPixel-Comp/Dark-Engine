#pragma once

#include "../Actor/Actor.h"



class ACameraActor : public AActor
{
	typedef AActor Super;

public:

	ACameraActor()
	{
		AspectRatio = 16.f / 9.f;
	}


	void BeginPlay() override { Super::BeginPlay();}
	void Update() override;
	void Destroy() override { Super::Destroy(); }

public:
	const float GetFov() const { return Fov; }
	const float GetAspectRation() const { return AspectRatio; }
	const XMMATRIX GetMatrixCamera() { return CameraMatrix; }


	const XMFLOAT3& GetFrontVector() const { return Front; }
	const XMFLOAT3& GetUpVector() const { return Up; }
	const XMFLOAT3& GetRightVector() const { return Right; }

private:
	//XMFLOAT4X4 CameraMatrix;
	float Fov = 90;
	float AspectRatio;

	XMMATRIX CameraMatrix;

	XMFLOAT3 Front;
	XMFLOAT3 Up;
	XMFLOAT3 Right;







};