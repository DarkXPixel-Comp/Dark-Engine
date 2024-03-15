#include "Widgets/UISideWidget.h"
#include "imgui_internal.h"


ImGuiDir UIDirToImGuiDir(EUISide dir)
{
    switch (dir)
    {
        case EUISide::Left: return ImGuiDir_Left; break; 
        case EUISide::Right: return ImGuiDir_Right; break;
        case EUISide::Up: return ImGuiDir_Up; break;
        case EUISide::Down: return ImGuiDir_Down; break;
    }
    return 0;
}





UISideWidget::UISideWidget(EUISide InSide)
{
    Side = InSide;
}
void UISideWidget::DrawImGui()
{
    ImGui::BeginViewportSideBar(Name.GetStr(), ImGui::GetMainViewport(), UIDirToImGuiDir(Side), Size.X, 0);
    // if(ImG)

    // ImGui::Dock
    


}