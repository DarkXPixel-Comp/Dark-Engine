#include "CameraActor.h"
#include <Core/CommandLine/CommandLine.h>
#include <Core/CoreDefines.h>
#include <Engine/public/DEngine.h>

void UCameraActor::BeginPlay()
{
	auto window = GEngine.GetWindowManager()->GetWindow(0);
	window->onResizeWindow.Bind(this, &UCameraActor::OnResize);

}



void UCameraActor::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	m_camera.m_position = m_position;
	m_camera.m_rotation = m_rotation;
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
	m_rotation.z +=  (-x * sensevity);
	m_rotation.y += (y * sensevity);
	x = 0;
	PrintLine(icstr(y));
}

void UCameraActor::W()
{
	m_position.z += sensevity;
}

void UCameraActor::A()
{
	m_position.x += -sensevity;
}
void UCameraActor::S()
{
	m_position.z += -sensevity;

}
void UCameraActor::D()
{
	m_position.x += sensevity;
}

void UCameraActor::Space()
{
	m_position.y += sensevity;
}

void UCameraActor::Action(eInputActionState action)
{
	switch (action)
	{
	case F1:
		break;
	case LShift:
		m_position.y -= sensevity;
		break;
	}
}


