#pragma once
#include "imgui.h"
#include "imgui_internal.h"


namespace ImDark
{
	bool BeginMenubar(const ImRect& barRectangle);

	__forceinline void ShiftCursorX(float distance)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
	}

	__forceinline void ShiftCursorY(float distance)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
	}

	__forceinline void ShiftCursor(float x, float y)
	{
		const ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
	}

	__forceinline void DrawButtonImage(void* Image1, void* Image2, void* Image3,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImVec2 rectMin, ImVec2 rectMax)
	{
		auto* drawList = ImGui::GetForegroundDrawList();
		if (ImGui::IsItemActive())
			drawList->AddImage(Image1, rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintPressed);
		else if (ImGui::IsItemHovered())
			drawList->AddImage(Image2, rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else
			drawList->AddImage(Image3, rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintNormal);
	};

	__forceinline void DrawButtonImage(void* Image1, void* Image2, void* Image3,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImRect rectangle)
	{
		DrawButtonImage(Image1, Image2, Image3, tintNormal, tintHovered, tintPressed, rectangle.Min, rectangle.Max);
	};

	__forceinline void DrawButtonImage(void* Image1,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImVec2 rectMin, ImVec2 rectMax)
	{
		DrawButtonImage(Image1, Image1, Image1, tintNormal, tintHovered, tintPressed, rectMin, rectMax);
	};

	__forceinline void DrawButtonImage(void* Image1,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImRect rectangle)
	{
		DrawButtonImage(Image1, Image1, Image1, tintNormal, tintHovered, tintPressed, rectangle.Min, rectangle.Max);
	};


	__forceinline void DrawButtonImage(void* Image1, void* Image2, void* Image3,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
	{
		DrawButtonImage(Image1, Image2, Image3, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	};

	__forceinline void DrawButtonImage(void* Image1,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
	{
		DrawButtonImage(Image1, Image1, Image1, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	};


	__forceinline ImRect RectExpanded(const ImRect& rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x -= x;
		result.Min.y -= y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	__forceinline ImRect RectOffset(const ImRect& rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x += x;
		result.Min.y += y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	__forceinline ImRect GetItemRect()
	{
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}


	void EndMenubar();
}
