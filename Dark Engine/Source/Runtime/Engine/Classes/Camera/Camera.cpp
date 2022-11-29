#include "Camera.h"

#include <Engine/public/DEngine.h>

void ACameraActor::Update()
{
	Super::Update();

	Front.x = cos(XMConvertToRadians(Rotation.z)) * cos(XMConvertToRadians(Rotation.y));
	Front.y = sin(XMConvertToRadians(Rotation.y));
	Front.z = sin(XMConvertToRadians(Rotation.z)) * cos(XMConvertToRadians(Rotation.y));







	Right.x = cos(XMConvertToRadians(Rotation.z + 90)) * cos(XMConvertToRadians(0));
	Right.y = sin(XMConvertToRadians(0));
	Right.z = sin(XMConvertToRadians(Rotation.z + 90)) * cos(XMConvertToRadians(0));


	Up.x = cos(XMConvertToRadians(0)) * cos(XMConvertToRadians(90));
	Up.y = sin(XMConvertToRadians(90));
	Up.z = sin(XMConvertToRadians(0)) * cos(XMConvertToRadians(90));




	XMVECTOR eyePosition = XMVectorSet(Position.x, Position.y, Position.z, 1);
	XMVECTOR focusPoint = -XMVectorSet(Front.x, Front.y, -Front.z, 1) + eyePosition;
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

	XMVECTOR dir = -XMVectorSet(Front.x, Front.y, -Front.z, 1);
	

	auto window = GEngine.GetWindow();

	XMMatrixOrthographicLH(window->GetWitdh(), window->GetHeight(), 0.1, 100);

	XMMATRIX ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), AspectRatio, 0.1, 100);

	//ProjectionMatrix = XMMatrixOrthographicLH(window->GetWitdh(), window->GetHeight(), 0.1, 100);

	//ViewMatrix = XMMatrixLookToLH(eyePosition, dir, upDirection);

	CameraMatrix = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);

	



}
