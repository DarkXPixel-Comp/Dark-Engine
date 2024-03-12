#include "Application/UIApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Math/Vector2D.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "d3d12.h"
#include "DescriptorHeap.h"
#include <dxgi.h>
#include <Runtime/D3D12RHI/public/d3dx12.h>
#include "Timer/GameTimer.h"
#include "Misc/Paths.h"



TSharedPtr<UIApplication> UIApplication::CurrentApplication = nullptr;
TSharedPtr<FGenericApplication> UIApplication::PlatformApplication = nullptr;

TRefCountPtr<ID3D12Device> device;
TSharedPtr<DirectX::DescriptorHeap>	srHeap;
TSharedPtr<DirectX::DescriptorHeap>	rtHeap;
TRefCountPtr<ID3D12CommandAllocator> CommandAllocator;
TRefCountPtr<ID3D12CommandQueue> queue;
TRefCountPtr<ID3D12GraphicsCommandList>	CommandList; 
TRefCountPtr<IDXGISwapChain> swapchain;
TRefCountPtr<ID3D12Fence> fence;
TRefCountPtr<IDXGIFactory> factory;
TRefCountPtr<ID3D12Resource> backBuffers[2];
uint64 FenceValue;
HANDLE FenceEvent;
int32 CurrentIndex = 0;


void TransitionResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,
	Microsoft::WRL::ComPtr<ID3D12Resource> resource,
	D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		resource.Get(),
		beforeState, afterState);

	commandList->ResourceBarrier(1, &barrier);




}


TSharedPtr<UIApplication> UIApplication::Create(const TSharedPtr<FGenericApplication>& InPlatformApplictation)
{
	PlatformApplication = InPlatformApplictation;
	CurrentApplication = MakeShareble(new UIApplication());

	PlatformApplication->SetMessageHandler(CurrentApplication);


	FGameTimer::Start();


	return CurrentApplication;
}						  
						
TSharedPtr<UIApplication> UIApplication::Create()
{
	return Create(MakeShareble(FPlatformApplicationMisc::CreateApplication()));
}


void UIApplication::InitializeRenderer(TSharedPtr<FUIRenderer> InRenderer)
{
	check(InRenderer);
	Renderer = InRenderer;
	Renderer->Initialize();


}

void UIApplication::Tick()
{
	float DeltaTime = FGameTimer::DeltaTime();

	TickPlatform(DeltaTime);

	FGameTimer::Tick();

	TickAndDrawWidgets(DeltaTime);



}

void UIApplication::TickPlatform(float DeltaTime)
{
	if (UIWindows.Num() > 0)
	{
		PlatformApplication->PumpMessages();
	}
	PlatformApplication->Tick(DeltaTime);

}





void UIApplication::TickAndDrawWidgets(float DeltaTime)
{
	DrawWindows();
	for (auto& Window : UIWindows)
	{
		Window->Update(DeltaTime);
	}
}

void UIApplication::DrawWindows()
{	
	check(Renderer);

	
	Renderer->DrawWindows(UIWindows);

}


TSharedPtr<UIWindow> UIApplication::AddWindow(TSharedPtr<UIWindow> InWindow)
{
	TSharedPtr<FGenericWindow> NewWindow = MakeWindow(InWindow);
	UIWindows.Add(InWindow);
	return InWindow;

}



void UIApplication::RegisterGameViewport(TSharedPtr<UIViewport> InViewport)
{
	GameViewport = InViewport;
	AllGameViewports.Add(InViewport);
}




FIntPoint UIApplication::GetMousePosition() const
{

	return PlatformApplication->GetMousePos();
}

TSharedPtr<FGenericWindow> UIApplication::MakeWindow(TSharedPtr<UIWindow> InUIWindow)
{
	TSharedPtr<FGenericWindowDefinition> Definition = MakeShareble(new FGenericWindowDefinition());

	const FVector2f Size = InUIWindow->GetInitSizeInScreen();
	Definition->WidthOnScreen = Size.X;
	Definition->HeightOnScreen = Size.Y;



	const FVector2f Position = InUIWindow->GetInitPositionInScreen();
	Definition->XPositionOnScreen = Position.X;
	Definition->YPositionOnScreen = Position.Y;
	Definition->bHasWindowBorder = InUIWindow->HasWindowBorder();

	Definition->Title = InUIWindow->GetTitle();

	TSharedPtr<FGenericWindow> Window = PlatformApplication->MakeWindow();

	InUIWindow->SetNativeWindow(Window);
	
#ifdef IMGUI
	InUIWindow->SetImGuiContext(ImGui::CreateContext());

	//SET IMGUI STYLE
	{
		ImGui::StyleColorsSpectrum();

	}

	//Window->InitImGui();
#endif

	PlatformApplication->InitializeWindow(Window, Definition);


	return Window;
}