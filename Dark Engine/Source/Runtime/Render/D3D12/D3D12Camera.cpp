#include "D3D12Camera.h"
#include <Source/Runtime/Engine/public/DEngine.h>


D3D12Camera::D3D12Camera(XMFLOAT3 position, XMFLOAT3 rotation)
{
	FWindowsWindow* window = GEngine.GetWindowManager()->GetWindow(0);
	m_position = position;




}
