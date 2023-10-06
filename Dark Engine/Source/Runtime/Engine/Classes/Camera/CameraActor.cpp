#include "CameraActor.h"
#include <Core/CommandLine/CommandLine.h>
#include <Core/CoreDefines.h>
#include <Engine/public/DEngine.h>


UCameraActor::UCameraActor()
{
	auto window = GEngine.GetWindowManager()->GetWindow(0);
	window->onResizeWindow.Bind(this, &UCameraActor::OnResize);

}

void UCameraActor::BeginPlay()
{
	Super::BeginPlay();
}



void UCameraActor::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	m_camera.m_position = m_position;
	m_camera.m_rotation = m_rotation;

	sensevity = DeltaTime * 100;
}

void UCameraActor::Destroy()
{
	Super::Destroy();
}

void UCameraActor::SetupPlayerController(FInputCore* controller)
{
	Super::SetupPlayerController(controller);
	controller->MouseDelegate.Bind(this, &UCameraActor::MouseLook);
	controller->WDelegate.Bind(this, &UCameraActor::W);
	controller->ADelegate.Bind(this, &UCameraActor::A);
	controller->SDelegate.Bind(this, &UCameraActor::S);
	controller->DDelegate.Bind(this, &UCameraActor::D);
	controller->SpaceDelegate.Bind(this, &UCameraActor::Space);
	controller->ActionDelegate.Bind(this, &UCameraActor::Action);
}

void UCameraActor::OnResize(long x, long y)
{
	m_camera.m_aspectRatio = static_cast<float>(x) / static_cast<float>(y);
}

void UCameraActor::MouseLook(float x, float y)
{
	float yaw = x * sensevity * mouseSensevity;
	float pitch = y * sensevity * mouseSensevity;

	m_rotation.z += yaw;
	m_rotation.y += pitch;

	if (m_rotation.z > 360)
		m_rotation.z -= 360;
	if (m_rotation.z < 0)
		m_rotation.z += 360;

	if (m_rotation.y > 89.f)
		m_rotation.y = 89.f;

	if (m_rotation.y < -89.f)
		m_rotation.y = -89.f;

	/*PrintLine(icstr(x), "\t");
	PrintLine(icstr(y), "\n");*/


	/*m_rotation.z +=  (-x * sensevity);
	m_rotation.y += (y * sensevity);*/
	x = 0;
	//PrintLine(icstr(y), "\n");
	
}

void UCameraActor::W()
{
	XMFLOAT3 Front = GetFrontVector();
	Front.x -= m_position.x;
	Front.y -= m_position.y;
	Front.z -= m_position.z;


	m_position.z += sensevity * Front.z * Speed;
	m_position.x += sensevity * Front.x * Speed;
	m_position.y += sensevity * Front.y * Speed;
}

void UCameraActor::A()
{
	XMFLOAT3 Right = GetRightVector();
	Right.x -= m_position.x;
	Right.y -= m_position.y;
	Right.z -= m_position.z;


	m_position.z -= sensevity * Right.z * Speed;
	m_position.x -= sensevity * Right.x * Speed;
	m_position.y -= sensevity * Right.y * Speed;
}
void UCameraActor::S()
{
	XMFLOAT3 Front = GetFrontVector();
	Front.x -= m_position.x;
	Front.y -= m_position.y;
	Front.z -= m_position.z;


	m_position.z -= sensevity * Front.z * Speed;
	m_position.x -= sensevity * Front.x * Speed;
	m_position.y -= sensevity * Front.y * Speed;

}
void UCameraActor::D()
{
	XMFLOAT3 Right = GetRightVector();
	Right.x -= m_position.x;
	Right.y -= m_position.y;
	Right.z -= m_position.z;


	m_position.z += sensevity * Right.z * Speed;
	m_position.x += sensevity * Right.x * Speed;
	m_position.y += sensevity * Right.y * Speed;
}

void UCameraActor::Space()
{
	m_position.y += sensevity * Speed;
}

void UCameraActor::Action(eInputActionState action)
{
	switch (action)
	{
	case F1:
		break;
	case LShift:
		m_position.y -= sensevity * Speed;
		break;
	}
}


