#include "Widgets/UIWidgetTest.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include "DirectXMath.h"


using namespace DirectX;

void UIWidgetTest::DrawImGui()
{
	if(ImGui::Begin("Test"))
	{
		static FVector3f Translation, Rotation, Scale;
		
		ImGui::DragFloat3("Tr", Translation.XYZ);
		ImGui::DragFloat3("Rt", Rotation.XYZ);
		ImGui::DragFloat3("Sc", Scale.XYZ);

		FMatrix4x4f Mat;
		ImGuizmo::RecomposeMatrixFromComponents(Translation.XYZ, Rotation.XYZ, Scale.XYZ, *Mat.M);

		XMMATRIX View = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 1), 
			XMVectorSet(0, 0, 1, 1), XMVectorSet(0, 1, 0, 0));
		XMMATRIX Proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 16 / 9, 0.1f, 100.f);;




		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate((float*)&View, (float*)&Proj, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float*)Mat.M);
		
		ImGuizmo::DecomposeMatrixToComponents((float*)Mat.M, Translation.XYZ, Rotation.XYZ, Scale.XYZ);


		ImGui::End();
	}




}

void UIWidgetTest::Update(float DeltaTime)
{
}