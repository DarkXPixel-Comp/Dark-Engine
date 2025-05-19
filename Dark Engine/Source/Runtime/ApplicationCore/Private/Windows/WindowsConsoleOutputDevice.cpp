#define NOMINMAX
#include "Windows/WindowsConsoleOutputDevice.h"
#include <chrono>
#include <Uxtheme.h>
#include <Containers/DarkString.h>
#include "Platform/PlatformString.h"
#include <Logging/LogCategory.h>
#include "Containers/Array.h"


struct NewLogEntry { FString String; ELogVerbosity::Type Verbosity; const class FString& Category; double Time; uint16 TextAttribute; };
struct Activity { HWND NameHwnd = 0; HWND StatusHwnd = 0; FString Name; FString Status; int32 Light = 0; int32 SortValue; bool bStatusDirty = false; bool bAlignLeft = false; };
struct LogEntry
{
	FString String;
	ELogVerbosity::Type Verbosity;
	const FString& Category;
	double Time;
	uint16 TextAttribute;
	uint16 LineCount;
};

class FWindowsConsoleOutputDevice::FConsoleWindow
{
public:
	enum ENotificationId : int32
	{
		ID_LOG = 0x8801,
		ID_LOG_INCLUDE_FILTER,
		ID_LOG_EXCLUDE_FILTER,
		ID_COMMAND,
		ID_COMMANDBUTTON,
		ID_CLEARLOGBUTTON,
		ID_ADDCHECKPOINTBUTTON,
		ID_TRACKEDACTIVITY,
	};
	enum : int32
	{
		HTEXPAND = 1000, // Just some value that is not any of the existing HT-ones
	};

	FConsoleWindow(FWindowsConsoleOutputDevice* InOwner) : Owner(InOwner)
	{
		StartDateTime = std::chrono::system_clock::now();
	//StartDateTime = 
		ConsoleHandle = CreateEvent(NULL, false, false, NULL);



	}

	~FConsoleWindow()
	{
		PostMessageW(MainHwnd, WM_CLOSE, 0, 0);
		WaitForSingleObject(Thread, 1000);
		Thread = NULL;
		CloseHandle(ConsoleHandle);

	}

	void CreateColors()
	{
		COLORREF BorderColor = RGB(0, 0, 0);
		COLORREF LogBackgroundColor = RGB(0, 0, 0);


		BackgroundColor = RGB(32, 32, 32);
		TextColor = RGB(220, 220, 220);
		EditBackgroundColor = RGB(64, 64, 64);
		StatusBackgroundColor[0] = RGB(42, 42, 42);
		StatusBackgroundColor[1] = RGB(49, 49, 49);
		ButtonColor = RGB(48, 48, 48);
		ButtonHighlightColor = RGB(100, 100, 100);
		ButtonPressedColor = RGB(130, 130, 100);
		ThumbColor = RGB(77, 77, 77);
		ScrollBackgroundColor = RGB(48, 48, 48);
		BorderColor = RGB(90, 90, 90);


		TextBrush = CreateSolidBrush(TextColor);
		BackgroundBrush = CreateSolidBrush(BackgroundColor);
		LogBackgroundBrush = CreateSolidBrush(LogBackgroundColor);
		EditBackgroundBrush = CreateSolidBrush(EditBackgroundColor);
		StatusBackgroundBrush[0] = CreateSolidBrush(StatusBackgroundColor[0]);
		StatusBackgroundBrush[1] = CreateSolidBrush(StatusBackgroundColor[1]);
		ButtonBrush = CreateSolidBrush(ButtonColor);
		ButtonHighlightBrush = CreateSolidBrush(ButtonHighlightColor);
		ButtonPressedBrush = CreateSolidBrush(ButtonPressedColor);
		ThumbBrush = CreateSolidBrush(ThumbColor);
		ScrollBackgroundBrush = CreateSolidBrush(ScrollBackgroundColor);

		NoPen = CreatePen(PS_SOLID, 0, TextColor);
		BorderPen = CreatePen(PS_SOLID, 1, BorderColor);
		WindowBorderPen = CreatePen(PS_INSIDEFRAME, 1, BorderColor);
		TextPen = CreatePen(PS_SOLID, 1, TextColor);


	}

	void UpdateSize(int32 Width, int32 Height, bool bRedraw)
	{
		if (bRedraw)
			SendMessageW(LogHwnd, WM_SETREDRAW, false, 0);
		int32 Flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;
		if (!bRedraw)
			Flags |= SWP_NOREDRAW;

		ActivitiesTotalHeight = 150;

		int32 LogTop = 1;
		LogTop += 35;

		int32 ButtonWidth = 90;
		int32 CommandFlags = Flags | SWP_SHOWWINDOW;
		int32 FilterFlags = 0;

		int32 CommandY = Height - ActivitiesTotalHeight - 20 - 5;
		int32 CommandHeight = CommandY - LogTop - 5;


		CommandFlags |= SWP_SHOWWINDOW;
		FilterFlags |= SWP_SHOWWINDOW;

		int32 Y = 10;
		int32 ButtonStart = Width - 235;

		SetWindowPos(ClearLogButtonHwnd, 0, ButtonStart, Y - 2, 80, 22, FilterFlags);
		SetWindowPos(CheckpointButtonHwnd, 0, ButtonStart + 94, Y - 2, 135, 22, FilterFlags);

		int32 WidthForFilters = ButtonStart - 45;
		if(WidthForFilters < 100)
			FilterFlags |= SWP_HIDEWINDOW;
		int32 X = 8;
		int32 WidthForEditBoxes = WidthForFilters / 2;
		SetWindowPos(IncludeFilterHwnd, 0, X, Y - 1, WidthForEditBoxes, 20, FilterFlags);
		X += WidthForEditBoxes + 20;
		SetWindowPos(ExcludeFilterHwnd, 0, X, Y - 1, WidthForEditBoxes, 20, FilterFlags);

		SetWindowPos(LogHwnd, 0, 7, LogTop, Width - 33, CommandHeight, Flags);
		SetWindowPos(LogScrollHwnd, 0, Width - 26, LogTop, 19, CommandHeight, Flags);
		SetWindowPos(CommandHwnd, 0, 8, CommandY, Width - ButtonWidth - 20, 20, CommandFlags);
		SetWindowPos(RunCommandHwnd, 0, Width - ButtonWidth - 7, CommandY - 1, ButtonWidth, 22, CommandFlags);


		if (bRedraw)
			SendMessageW(LogHwnd, WM_SETREDRAW, true, 0);
	}

	int32 GetSelectedItems(TArray<int>& OutSelectedItems)
	{
		int32 SelectionCount = static_cast<int32>(SendMessage(LogHwnd, LB_GETSELCOUNT, 0, 0));
		OutSelectedItems.Resize(SelectionCount);
		SendMessage(LogHwnd, LB_GETSELITEMS, SelectionCount, (LPARAM)OutSelectedItems.GetData());
		return SelectionCount;
	}

	int32 HitTestNCB(int32 X, int32 Y)
	{
		RECT WindowRect;
		GetWindowRect(MainHwnd, &WindowRect);
		if (Y < WindowRect.top + 2 || Y > WindowRect.top + 27 || X > WindowRect.right - 2)
			return HTNOWHERE;
		int32 ButtonWidth = 27;
		if (X <= WindowRect.right - ButtonWidth * 4)
			return HTCAPTION;
		int32 ButtonIndex = (WindowRect.right - X) / ButtonWidth;
		int32 Buttons[] = { HTCLOSE, HTMAXBUTTON, HTMINBUTTON, HTEXPAND };
		return Buttons[ButtonIndex];
	}

	int32 HitTestNCB(LPARAM lParam)
	{
		return HitTestNCB((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
	}

	LRESULT HitTestNCA(HWND hWnd, LPARAM lParam)
	{
		POINT MousePt = { ((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)) };
		RECT WindowRect;
		GetWindowRect(hWnd, &WindowRect);
		RECT FrameRect = { 0 };
		AdjustWindowRectEx(&FrameRect, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, false, NULL);
		int32 Row = 1;
		int32 Col = 1;
		bool bOnResizeBorder = false;

		int BorderWidth = 2;

		if (MousePt.y >= WindowRect.top && MousePt.y < WindowRect.top + 27)
		{
			bOnResizeBorder = (MousePt.y < (WindowRect.top - FrameRect.top));
			Row = 0;
		}
		else if (MousePt.y < WindowRect.bottom && MousePt.y >= WindowRect.bottom - BorderWidth)
			Row = 2;

		if (MousePt.x >= WindowRect.left && MousePt.x < WindowRect.left + BorderWidth)
			Col = 0;
		else if (MousePt.x < WindowRect.right && MousePt.x >= WindowRect.right - BorderWidth)
			Col = 2;

		LRESULT hitTests[3][3] =
		{
			{ HTTOPLEFT,    bOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
			{ HTLEFT,       HTCLIENT,     HTRIGHT },
			{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
		};

		return hitTests[Row][Col];
	}

	void RedrawNC()
	{
		RedrawWindow(MainHwnd, NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
	}


	void SetTopVisible(int32 TopVisible, bool ShouldPost)
	{
		SetWindowPos(TipHwnd, MainHwnd, 1, 1, 1, 1, SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOOWNERZORDER);
		if (ShouldPost)
			PostMessageW(LogHwnd, LB_SETTOPINDEX, TopVisible, 0);
		else
			SendMessageW(LogHwnd, LB_SETTOPINDEX, TopVisible, 0);
	}


	void MoveTopVisible(const RECT& Rect, int32 Offset)
	{
		bAutoScrollLog = false;
		int32 TopVisible = static_cast<int32>(SendMessage(LogHwnd, LB_GETTOPINDEX, 0, 0));
		int32 PageSize = Rect.bottom / LogFontHeight;
		TopVisible = std::max(0, TopVisible + Offset);
		SetTopVisible(TopVisible, false);
		RedrawWindow(LogScrollHwnd, NULL, NULL, RDW_INVALIDATE);
	}

	int32 HandleMouseWheel(WPARAM wParam)
	{
		RECT Rect;
		GetClientRect(LogHwnd, &Rect);
		MoveTopVisible(Rect, -GET_WHEEL_DELTA_WPARAM(wParam) * 3 / WHEEL_DELTA);
		return 0;
	}

	static LRESULT StaticMainWndWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		FConsoleWindow* ThisWindow = (FConsoleWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!ThisWindow && Msg == WM_CREATE)
		{
			#pragma comment( lib, "UxTheme")
			SetWindowTheme(hWnd, TEXT(""), TEXT(""));
			ThisWindow = (FConsoleWindow*)lParam;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ThisWindow);
		}
		if (ThisWindow && hWnd == ThisWindow->MainHwnd)
		{
			return ThisWindow->MainWndProc(hWnd, Msg, wParam, lParam);
		}
		else
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}


	template<class Lambda>
	int32 TraverseActivityPositions(int32 Width, int32 Height, const Lambda& InLamda)
	{
		Width -= 6; // Margins

		TArray<Activity*> LeftActivities;
		TArray<Activity*> RightActivities;
		uint32 ActivityCount = 0;
		for (int32 i = 0, e = Activities.Num(); i != e; ++i)
		{
			Activity& A = Activities[i];
			if (A.Name == TEXT(""))
				continue;
			++ActivityCount;
			if (A.bAlignLeft)
				LeftActivities.Add(&A);
			else
				RightActivities.Add(&A);
		}

		auto SortFunc = [](const Activity* A, const Activity* B)
			{
				if (A->SortValue != B->SortValue)
					return A->SortValue < B->SortValue;
				return uintptr_t(A) < uintptr_t(B);
			};

		std::sort(LeftActivities.begin(), LeftActivities.end(), SortFunc);
		std::sort(RightActivities.begin(), RightActivities.end(), SortFunc);

		int32 LeftCount = LeftActivities.Num();
		int32 RightCount = ActivityCount - LeftCount;

		int32 LeftColumnMinWidth = 600;
		int32 RightColumnWidth = 200;
		int32 LeftColumnCount = 1;
		int32 RightColumnCount = 1;

		int32 RowCount = std::max(LeftCount, RightCount);

		if (RightCount > LeftCount)
		{
			if (Width - LeftColumnMinWidth >= RightColumnWidth * 2)
			{
				RightColumnCount = 2;
				RowCount = std::max(LeftCount, (RightCount + 1) / 2);
			}
		}

		int32 RowHeight = 18;
		int32 TotalHeight = RowCount * RowHeight + 8;


		int32 StartY = Height - TotalHeight + 2;

		int32 X;
		int32 Y;
		int32 RowIndex;
		int32 ColWidth;
		int32 ColOffset;

		auto IterateActivities = [&](TArray<Activity*>& SortedActivities)
			{
				for (int32 i = 0, e = SortedActivities.Num(); i != e; ++i)
				{
					Activity& A = *SortedActivities[i];
					if (A.Name == TEXT(""))
						continue;

					InLamda(A, X, Y, ColWidth, RowIndex);
					Y += RowHeight;
					++RowIndex;
					if (RowIndex < RowCount)
						continue;
					RowIndex = 0;
					Y = StartY;
					X += ColOffset;
				}
			};

		X = 7;
		Y = StartY;
		RowIndex = 0;
		int TotalLeftWidth = Width - RightColumnCount * RightColumnWidth - X;
		ColWidth = TotalLeftWidth / LeftColumnCount;
		ColOffset = ColWidth - 2;
		IterateActivities(LeftActivities);

		X = Width - RightColumnWidth + 8;
		Y = StartY;
		RowIndex = 0;
		ColWidth = RightColumnWidth - 8;
		ColOffset = -RightColumnWidth;

		IterateActivities(RightActivities);
		return TotalHeight;
	}

	LRESULT MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_HELP:
			{
				break;
			}
		case WM_SIZE:
		{
			switch (wParam)
			{
			case SIZE_MAXIMIZED:
			case SIZE_RESTORED:
				UpdateSize(LOWORD(lParam), HIWORD(lParam), true);
				RECT WindowRect;
				GetWindowRect(MainHwnd, &WindowRect);
				ConsoleHeight = WindowRect.bottom - WindowRect.top;
				ConsoleWidth = WindowRect.right - WindowRect.left;
			default:
				break;
			}
			return 0;
		}
		case WM_MOVE:
		{
			RECT WindowRect;
			GetWindowRect(MainHwnd, &WindowRect);
			ConsoleX = WindowRect.left;
			ConsoleY = WindowRect.top;
			break;
		}
		case WM_SETFOCUS:
			SetFocus(CommandHwnd);
			SendMessageW(CommandHwnd, EM_SETSEL, 0, -1);
			break;
		case WM_MOUSEHWHEEL:
			return HandleMouseWheel(wParam);
		case WM_ERASEBKGND:
		{
			HDC Hdc = (HDC)wParam;
			RECT Rect;
			GetClientRect(hWnd, &Rect);
			FillRect(Hdc, &Rect, BackgroundBrush);

			SelectObject(Hdc, BorderPen);
			SelectObject(Hdc, GetStockObject(NULL_BRUSH));

			for (int32 dlgId : {ID_LOG_INCLUDE_FILTER, ID_LOG_EXCLUDE_FILTER, ID_COMMAND})
			{
				HWND EditHwnd = GetDlgItem(hWnd, dlgId);
				if (!IsWindowVisible(EditHwnd))
					continue;
				RECT EditRect;
				GetClientRect(EditHwnd, &EditRect);
				MapWindowPoints(EditHwnd, hWnd, (LPPOINT)&EditRect, 2);
				Rectangle(Hdc, EditRect.left - 1, EditRect.top - 1, EditRect.right + 1, EditRect.bottom + 1);
			}
			return true;
		}
		case WM_CTLCOLORSTATIC:
		{ 
			HDC Hdc = (HDC)wParam;
			SetTextColor(Hdc, TextColor);
			SetBkColor(Hdc, BackgroundColor);
			return (INT_PTR)BackgroundBrush;
		}
		case WM_CTLCOLORLISTBOX:
		{
			if ((HWND)lParam == LogHwnd)
			{
				int32 ItemCount = (int32)SendMessage(LogHwnd, LB_GETCOUNT, 0, 0);
				RECT Rect;
				GetClientRect(LogHwnd, &Rect);
				int32 VisibleCount = (int32)ceil((float)Rect.bottom / (float)LogFontHeight);
				int32 TopIndex = (int32)SendMessage(LogHwnd, LB_GETTOPINDEX, 0, 0);
				if (ItemCount - TopIndex < VisibleCount)
				{
					Rect.top = (ItemCount - TopIndex) * LogFontHeight;
					FillRect((HDC)wParam, &Rect, LogBackgroundBrush);
				}
				return (INT_PTR)GetStockObject(NULL_BRUSH);
			}
			break;
		}
		case WM_MEASUREITEM:
		{
			PMEASUREITEMSTRUCT pmis = (PMEASUREITEMSTRUCT)lParam;
			pmis->itemHeight = LogFontHeight;
			return true;
		}


		case WM_NOTIFY:
		{
			LPNMHDR hdr = (LPNMHDR)lParam;
			if (hdr->code != NM_CUSTOMDRAW)
				break;
			switch (hdr->idFrom)
			{
			case ID_COMMANDBUTTON:
			case ID_CLEARLOGBUTTON:
			case ID_ADDCHECKPOINTBUTTON:
			{
				NMCUSTOMDRAW& nmcd = *(NMCUSTOMDRAW*)hdr;
				uint32 CtlId = (uint32)hdr->idFrom;
				int32 ItemState = nmcd.uItemState;
				TCHAR Str[32];
				int32 StrLen = GetDlgItemText(hWnd, CtlId, Str, 32);
				HBRUSH Brush = ButtonBrush;
				COLORREF Color = ButtonColor;
				if (ItemState & CDIS_SELECTED)
				{
					Brush = ButtonPressedBrush;
					Color = ButtonPressedColor;
				}
				else if (ItemState & CDIS_HOT)
				{
					Brush = ButtonHighlightBrush;
					Color = ButtonHighlightColor;
				}
				SelectObject(nmcd.hdc, GetStockObject(NULL_BRUSH));
				FillRect(nmcd.hdc, &nmcd.rc, Brush);
				SetBkColor(nmcd.hdc, Color);
				SetTextColor(nmcd.hdc, TextColor);
				DrawTextW(nmcd.hdc, Str, StrLen, &nmcd.rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP);
				SelectObject(nmcd.hdc, BorderPen);
				Rectangle(nmcd.hdc, nmcd.rc.left, nmcd.rc.top, nmcd.rc.right, nmcd.rc.bottom);
				return CDRF_SKIPDEFAULT;
			}
			}
			break;
		}
		case WM_DRAWITEM:
		{
			PDRAWITEMSTRUCT pdis = (PDRAWITEMSTRUCT)lParam;
			HDC Hdc = pdis->hDC;

			if (pdis->CtlType == ODT_STATIC)
			{
				TCHAR Str[512];
				int32 StrLen = GetWindowText(pdis->hwndItem, Str, std::size(Str));

				if (pdis->hwndItem == TipHwnd)
				{
					if (pdis->rcItem.bottom != 1)
					{
						FillRect(Hdc, &pdis->rcItem, BackgroundBrush);
						SetTextColor(Hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
						SelectObject(Hdc, LogFont);
						DrawText(Hdc, Str, StrLen, &pdis->rcItem, DT_SINGLELINE | DT_VCENTER);
					}
					break;
				}
				HBRUSH Brush = BackgroundBrush;

				int32 RowIndex = GetWindowLong(pdis->hwndItem, GWLP_USERDATA);
				if (RowIndex != -1)
				{
					Brush = StatusBackgroundBrush[(RowIndex & 1)];
					SetBkColor(Hdc, StatusBackgroundColor[(RowIndex & 1)]);
				}

				SelectObject(Hdc, Brush);

				// All this stuff just to prevent flickering when text changes fast
				int32 TextOffset = 1;
				RECT ModifiedRect = pdis->rcItem;
				DrawTextW(Hdc, Str, StrLen, &ModifiedRect, DT_SINGLELINE | DT_END_ELLIPSIS | DT_CALCRECT | DT_MODIFYSTRING);
				RECT TempRect = pdis->rcItem;
				TempRect.bottom = TempRect.top + TextOffset;
				TempRect.right = ModifiedRect.right;
				FillRect(Hdc, &TempRect, Brush);

				TempRect = pdis->rcItem;
				TempRect.left = ModifiedRect.right;
				FillRect(Hdc, &TempRect, Brush);
				if (ModifiedRect.bottom < pdis->rcItem.bottom)
				{
					TempRect = pdis->rcItem;
					TempRect.right = ModifiedRect.right;
					TempRect.top = ModifiedRect.bottom;
					FillRect(Hdc, &TempRect, Brush);
				}

				StrLen = FPlatformString::StrLen(Str);
				ExtTextOut(Hdc, pdis->rcItem.left, pdis->rcItem.top + TextOffset, 0, &pdis->rcItem, Str, StrLen, 0);
				break;
			}
			if (pdis->CtlID != ID_LOG || pdis->itemID == -1)
				break;

			switch (pdis->itemAction)
			{
			case ODA_SELECT:
			case ODA_DRAWENTIRE:
			{
				uint32 LogVirtualIndex = (uint32)(pdis->itemData);
				LogEntry& Entry = *(LogEntry*)(nullptr);

				int Middle = (pdis->rcItem.top + pdis->rcItem.bottom) / 2;
				int YPos = Middle - LogFontHeight / 2 - (1 - 1) * LogFontHeight / 2;


				if (pdis->itemState & ODS_SELECTED)
				{
					FillRect(Hdc, &pdis->rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
					SetTextColor(Hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
				}
				else
				{
					FillRect(Hdc, &pdis->rcItem, LogBackgroundBrush);
					if (Entry.TextAttribute == (FOREGROUND_INTENSITY | FOREGROUND_RED))
						SetTextColor(Hdc, RGB(220, 0, 0));
					else if (Entry.TextAttribute == (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN))
						SetTextColor(Hdc, RGB(220, 220, 0));
					else
						SetTextColor(Hdc, RGB(204, 204, 204));
				}
				int32 XPos = 0;
				TextOut(Hdc, XPos, YPos, TEXT("HELLO"), 5);
				return true;
			}
			case ODA_FOCUS:
				return true;
			}
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC Hdc = BeginPaint(hWnd, &ps);
			RECT Rect;
			GetClientRect(hWnd, &Rect);	

			TraverseActivityPositions(Rect.right, Rect.bottom, [&](Activity& A, int32 X, int32 Y, int32 Width, int32 RowIndex)
				{	
					if (A.Light == 0)
						return;
					SelectObject(Hdc, StatusLightBrush[A.Light - 1]);
					int32 XOffset = 12;
					int32 YOffset = 1;
					int32 FontHeight = 16;
					int32 CircleSize = 11;
					int32 HalfDiff = (FontHeight - CircleSize) / 2;
					RoundRect(Hdc, X - CircleSize + XOffset, Y + HalfDiff + YOffset, X + XOffset, Y + FontHeight - HalfDiff + YOffset, CircleSize, CircleSize);
				});

			EndPaint(hWnd, &ps);
			return 0;
		}
		break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_VKEYTOITEM:
		{
			if ((HWND)lParam == LogHwnd)
			{
				if ((LOWORD(wParam) == 0x43 || LOWORD(wParam) == VK_INSERT) && (::GetKeyState(VK_CONTROL) >> 15)) // Copy-paste
				{
					TArray<int> SelectedItems;
					int32 SelectionCount = GetSelectedItems(SelectedItems);
					if (SelectionCount > 0)
					{
						TArray<TCHAR> Buffer;
						FString String;
						for (int32 i = 0; i != SelectionCount; ++i)
						{
							int32 Index = SelectedItems[i];
							int32 LogIndex = static_cast<int32>(SendMessage(LogHwnd, LB_GETITEMDATA, Index, 0) - LogIndexOffset);
							String.Append(TEXT("CHANGE: [%i]%s"), __LINE__, *FString(__FILE__));
							String.Append(TEXT("\r\n"));
						}
					}
				}
				else if (LOWORD(wParam) == VK_END && !(::GetKeyState(VK_SHIFT) >> 15)) // Enable auto scrolling and remove selection
				{
					return -2;
				}
			}
			break;
		}

		case DM_GETDEFID:
			return MAKEWPARAM(ID_COMMANDBUTTON, DC_HASDEFID);

		case WM_NCHITTEST: // This actually draw things
		{
			int32 Res = HitTestNCB(lParam);
			if (Res != HTNOWHERE)
				return Res;
			return HitTestNCA(hWnd, lParam);
		}

		default:					
			break;
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	static LRESULT CALLBACK StaticScrollBarWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	uint32 ThreadProc()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX WndClass;
		ZeroMemory(&WndClass, sizeof(WNDCLASSEX));
		WndClass.cbSize = sizeof(WNDCLASSEX);
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc = &StaticMainWndWndProc;
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hInstance = hInstance;
		WndClass.hbrBackground = NULL;
		WndClass.lpszClassName = TEXT("ConsoleWindow");
		ATOM WndClassAtom = RegisterClassEx(&WndClass);

		WndClass.lpszClassName = TEXT("ConsoleScrollBar");
		WndClass.lpfnWndProc = &StaticScrollBarWndProc;
		ATOM ScrollBarClassAtom = RegisterClassEx(&WndClass);

		NONCLIENTMETRICS NonClientMetrics;
		NonClientMetrics.cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &NonClientMetrics, 0);
		Font = (HFONT)CreateFontIndirect(&NonClientMetrics.lfMessageFont);

		const TCHAR* FontName = TEXT("Cascadia Mono");
		int32 FontHeight = -MulDiv(8, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
		LogFont = (HFONT)CreateFont(FontHeight, 0, 0, 0, FW_NORMAL,false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_MODERN, FontName);

		DWORD WindowStyle = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN;
		const TCHAR* WindowClassName = MAKEINTATOM(WndClassAtom);
		MainHwnd = CreateWindow(WindowClassName, TEXT("Console"), WindowStyle, ConsoleX, ConsoleY, ConsoleWidth, ConsoleHeight, NULL, NULL, hInstance, this);
		SetWindowLongPtr(MainHwnd, GWLP_USERDATA, (LONG_PTR)this);
		RECT MainRect;
		GetWindowRect(MainHwnd, &MainRect);
		ConsoleX = MainRect.left;
		ConsoleY = MainRect.top;
		ConsoleWidth = MainRect.right - MainRect.left;
		ConsoleHeight = MainRect.bottom - MainRect.top;

		SIZE FontSize;
		HDC Hdc = GetDC(MainHwnd);
		SelectObject(Hdc, LogFont);
		GetTextExtentPoint32(Hdc, TEXT("A"), 1, &FontSize);
		LogFontWidth = FontSize.cx;
		LogFontHeight = FontSize.cy;
		ReleaseDC(MainHwnd, Hdc);

		IncludeFilterHwnd = _CreateWindow(WC_EDIT, NULL, ES_AUTOHSCROLL, Font, 0, 0, 0, 0, 0, ID_LOG_INCLUDE_FILTER);
		ExcludeFilterHwnd = _CreateWindow(WC_EDIT, NULL, ES_AUTOHSCROLL, Font, 0, 0, 0, 0, 0, ID_LOG_EXCLUDE_FILTER);
		ClearLogButtonHwnd = _CreateWindow(WC_BUTTON, TEXT("Clear"), 0, Font, 0, 0, 0, 0, 0, ID_CLEARLOGBUTTON);
		CheckpointButtonHwnd = _CreateWindow(WC_BUTTON, TEXT("Log CHECKPOITN0"), 0, Font, 0, 0, 0, 0, 0, ID_ADDCHECKPOINTBUTTON);
	
		DWORD LogHwndStyle = LBS_NOINTEGRALHEIGHT | LBS_EXTENDEDSEL | LBS_WANTKEYBOARDINPUT | LBS_NOTIFY | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED;
		LogHwnd = _CreateWindow(WC_LISTBOX, NULL, LogHwndStyle, LogFont, 0, 0, 1, 1, ID_LOG);
		SetWindowLongPtr(LogHwnd, GWLP_USERDATA, (LONG_PTR)this);
		LogHwndWndProcPtr = (WNDPROC)SetWindowLongPtr(LogHwnd, GWLP_WNDPROC, (LONG_PTR)&StaticLogHwndWndProc);

		const TCHAR* ScrollBarClassName = MAKEINTATOM(ScrollBarClassAtom);
		LogScrollHwnd = _CreateWindow(ScrollBarClassName, NULL, SBS_VERT, NULL);
		SetWindowLongPtr(LogScrollHwnd, GWLP_USERDATA, (LONG_PTR)this);

		CommandHwnd = _CreateWindow(WC_EDIT, NULL, ES_AUTOHSCROLL, Font, 0, 0, 1, 1, 0, ID_COMMAND);
		RunCommandHwnd = _CreateWindow(WC_BUTTON, TEXT("Run"), 0, Font, 0, 0, 1, 1, 0, ID_COMMANDBUTTON);

		SetFocus(CommandHwnd);
		RECT Rect;
		GetClientRect(MainHwnd, &Rect);
		UpdateSize(Rect.right, Rect.bottom, false);


		UpdateWindow(MainHwnd);
		SetForegroundWindow(MainHwnd);

		TipHwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE, WC_STATIC, TEXT(""), SS_OWNERDRAW | WS_VISIBLE | WS_POPUP | WS_DISABLED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 1, 1, 1, 1, MainHwnd, NULL, hInstance, NULL);
		SendMessage(TipHwnd, WM_SETFONT, (WPARAM)Font, 0);


		bool bLoop = true;
		bool bWasVisible = false;
		while (bLoop)
		{
			if (bWasVisible != bIsVisible)
			{
				MoveWindow(MainHwnd, ConsoleX, ConsoleY, ConsoleWidth, ConsoleHeight, TRUE);
			}

			ShowWindow(MainHwnd, bIsVisible ? SW_SHOW : SW_HIDE);
			bWasVisible = bIsVisible;

			if (bIsVisible)
			{
				UpdateIncludeFilter(*ConsoleIncludeFilterStr);
				UpdateExcludeFilter(*ConsoleExcludeFilterStr);
			}

			DWORD WaitResult = MsgWaitForMultipleObjects(1, &ConsoleHandle, false, 200, QS_ALLINPUT);
			if (WaitResult == WAIT_TIMEOUT)
				continue;

			MSG Msg;
			while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				if (!IsDialogMessage(MainHwnd, &Msg))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
				if (Msg.message == WM_QUIT)
				{
					DestroyWindow(MainHwnd);
					DeleteObject(LogFont);
					DeleteObject(Font);
					UnregisterClass(WindowClassName, hInstance);
					MainHwnd = NULL;
					bLoop = false;
					break;
				}
			}

		}

		return 0;

	}

	void UpdateIncludeFilter(const TCHAR* Str)
	{
		ConsoleIncludeFilterStr = Str;
		SetDlgItemText(MainHwnd, ID_LOG_INCLUDE_FILTER, *ConsoleIncludeFilterStr);
		//ConsoleIncludeFilterStr.ParseIntoArray(IncludeFilter, TEXT(" "));
	}

	void UpdateExcludeFilter(const TCHAR* Str)
	{
		ConsoleExcludeFilterStr = Str;
		SetDlgItemText(MainHwnd, ID_LOG_EXCLUDE_FILTER, *ConsoleExcludeFilterStr);
		
		//ConsoleExcludeFilterStr.ParseIntoArray(ExcludeFilter, TEXT(" "));
	}

	HWND _CreateWindow(const TCHAR* ClassName, const TCHAR* Str, DWORD Style, HFONT hFont, int32 X = 0, int32 Y = 0, int32 Width = 1, int32 Height = 1, DWORD ExStyle = 0, ENotificationId InId = (ENotificationId)0)
	{
		HWND Hwnd = CreateWindowEx(ExStyle, ClassName, Str, Style | WS_VISIBLE | WS_CHILD, X, Y, Width, Height, MainHwnd, (HMENU)InId, NULL, NULL);
		if (hFont)
			SendMessageW(Hwnd, WM_SETFONT, (WPARAM)hFont, 0);
		return Hwnd;
	}

	static LRESULT CALLBACK StaticLogHwndWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		//FConsoleWindow* ThisPtr = (FConsoleWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		//return ThisPtr->LogHwndWndProc(hWnd, Msg, wParam, lParam);
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}


	static DWORD WINAPI StaticThreadProc(LPVOID lParam)
	{
		return ((FConsoleWindow*)lParam)->ThreadProc();
	}

	void Start()
	{
		Thread = CreateThread(NULL, 0, StaticThreadProc, this, 0, NULL);
	}



	FWindowsConsoleOutputDevice* Owner;
	std::chrono::system_clock::time_point StartDateTime;
	double StartTime;
	HANDLE ConsoleHandle;
	HANDLE Thread;

	int32 ActivitiesTotalHeight = 0;
	TArray<Activity> Activities;

	bool bIsVisible = false;

	HWND MainHwnd;
	HWND LogHwnd;
	HWND LogScrollHwnd;
	HWND CommandHwnd;
	HWND RunCommandHwnd;
	HWND IncludeFilterHwnd;
	HWND ExcludeFilterHwnd;
	HWND ClearLogButtonHwnd;
	HWND CheckpointButtonHwnd;
	HWND TipHwnd;

	FString ConsoleIncludeFilterStr;
	FString ConsoleExcludeFilterStr;

	TArray<FString> IncludeFilter;
	TArray<FString> ExcludeFilter;

	int32 ConsoleWidth = 160;
	int32 ConsoleHeight = 40;

	int32 ConsoleX = 0;
	int32 ConsoleY = 0;

	HFONT Font;
	HFONT LogFont;


	WNDPROC LogHwndWndProcPtr;

	int32 LogIndexOffset = 0;
	int32 LogFontWidth = 0;
	int32 LogFontHeight = 0;
	int32 LogScrollGrabPos = -1;
	int32 NcButtonHot = -1;
	int32 NcButtonDown = -1;
	int32 RightClickedItem = -1;
	FString SelectedWord;
	FString SelectedCategory;

	HBRUSH TextBrush;
	HBRUSH BackgroundBrush;
	HBRUSH LogBackgroundBrush;
	HBRUSH EditBackgroundBrush;
	HBRUSH StatusBackgroundBrush[2];
	HBRUSH ButtonBrush;
	HBRUSH ButtonHighlightBrush;
	HBRUSH ButtonPressedBrush;
	HBRUSH ThumbBrush;
	HBRUSH ScrollBackgroundBrush;
	HBRUSH StatusLightBrush[3];
	HPEN BorderPen;
	HPEN WindowBorderPen;
	HPEN TextPen;
	HPEN NoPen;

	COLORREF TextColor;
	COLORREF BackgroundColor;
	COLORREF EditBackgroundColor;
	COLORREF StatusBackgroundColor[2];
	COLORREF ButtonColor;
	COLORREF ButtonHighlightColor;
	COLORREF ButtonPressedColor;
	COLORREF ThumbColor;
	COLORREF ScrollBackgroundColor;

	bool bAutoScrollLog = true;

};

void FWindowsConsoleOutputDevice::Show(bool bShowWindow)
{
	if (bShowWindow)
	{
		int32 ConsoleWidth = 1000;
		int32 ConsoleHeight = 700;
		int32 ConsoleX = 0;
		int32 ConsoleY = 0;
		int32 ConsoleColor = 0;


		bool bFirstCall = !Window;
		if (bFirstCall)
			Window = new FConsoleWindow(this);

		Window->ConsoleWidth = ConsoleWidth;
		Window->ConsoleHeight = ConsoleHeight;
		Window->ConsoleX = ConsoleX;
		Window->ConsoleY = ConsoleY;
		Window->bIsVisible = true;
		//Window->ConsoleColor = ConsoleColor;
		
		if (bFirstCall)
			Window->Start();




	}


}
