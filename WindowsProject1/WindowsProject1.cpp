// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "WindowsProject1.h"
#include "CommCtrl.h"
#include "Winuser.h"
#include <Commdlg.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;  
HWND hWndMain;
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];    
HWND hwndArray[7];
OPENFILENAME ofn;
TCHAR PATH[MAX_LOADSTRING];
TCHAR Buff[MAX_LOADSTRING];
DWORD BSize = MAX_LOADSTRING;
const TCHAR TITLE[] = L"My Notepad";
TCHAR titleWND[MAX_LOADSTRING];
HANDLE handlebank = NULL;
HKEY hKey;
int wl, wh;
HWND hPathText;
HWND hPath;
BOOL OPEN = false;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Toolbar(HWND, UINT, WPARAM, LPARAM);
HWND				CreateToolbar(HWND);
HWND CreateEditControl(HWND);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

HWND CreateEditControl(HWND hWndParent) {
	HWND editControl = CreateWindowEx(0, L"EDIT",0,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER | WS_CLIPSIBLINGS,
		0, 0, 0, 0,  
		hWndParent, (HMENU)ID_EDITCHILD,
		(HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE), NULL);
	return editControl;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   InitCommonControls();
   hInst = hInstance; 
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)TITLE);
   hWndMain = hWnd;

   DWORD t1 = 0, t2 = sizeof(wl), t3 = 0, t4 = sizeof(wh);
   RegOpenKeyExW(HKEY_CURRENT_USER, L"txted", 0, KEY_ALL_ACCESS, &hKey);
   RegQueryValueExA(hKey, (LPCSTR)L"l", 0, (LPDWORD)& t1, (BYTE*)& wl, &t2);
   RegQueryValueExA(hKey, (LPCSTR)L"h", 0, (LPDWORD)& t3, (BYTE*)& wh, &t4);

   SetWindowPos(hWnd, 0, 0, 0, wl, wh, SWP_NOMOVE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND ButtonOpen , ButtonSave , ButtonSaveAs;
	static HWND TOOLBAR , EDITCONTROL;
	TCHAR szFileTitle[MAX_LOADSTRING];

    switch (message) {
	case WM_CREATE: {
		ButtonOpen = CreateWindowEx(0, L"BUTTON", L"Open", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_CLIPSIBLINGS, 0, 0, 0, 0, hWnd, (HMENU)IDM_OPEN, hInst, NULL);
		ButtonSave = CreateWindowEx(0, L"BUTTON", L"Save", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_CLIPSIBLINGS,0, 0, 0, 0,hWnd, (HMENU)IDM_SAVE, hInst, NULL);
		ButtonSaveAs = CreateWindowExW(0, L"BUTTON", L"Save as", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_CLIPSIBLINGS, 0, 0, 0, 0, hWnd, (HMENU)IDM_SAVEAS, hInst, NULL);
		hPathText = CreateWindowExW(0, L"STATIC", L"Path to file: ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | SS_SIMPLE | WS_CLIPSIBLINGS,0, 0, 0, 0, hWnd, 0, hInst, NULL);
		hPath = CreateWindowExW(0, L"STATIC", PATH,WS_TABSTOP | WS_VISIBLE | WS_CHILD | SS_SIMPLE | WS_CLIPSIBLINGS,0, 0, 0, 0, hWnd, 0, hInst, NULL);
		TOOLBAR = CreateToolbar(hWnd);
		EDITCONTROL = CreateEditControl(hWnd);
		hwndArray[4] = ButtonSave;
		hwndArray[5] = ButtonSaveAs;
		hwndArray[6] = ButtonOpen;
		return 0;
	}

	case WM_SIZE: {
		SetWindowPos(TOOLBAR, 0, 0, 0, 0, 0, 0);
		SetWindowPos(EDITCONTROL, 0, 10, 45, LOWORD(lParam) - 10, HIWORD(lParam) - 100, 0);
		SetWindowPos(hPathText, 0, 10, HIWORD(lParam) - 40, LOWORD(lParam) - 220, 20, 0);
		SetWindowPos(hPath, 0, 85, HIWORD(lParam) - 40, LOWORD(lParam) - 180, 20, 0);
		SetWindowPos(hwndArray[4], 0, LOWORD(lParam) - 132, HIWORD(lParam) - 40, 60, 30, 0);
		SetWindowPos(hwndArray[5], 0, LOWORD(lParam) - 70, HIWORD(lParam) - 40, 60, 30, 0);
		SetWindowPos(hwndArray[6], 0, LOWORD(lParam) - 194, HIWORD(lParam) - 40, 60, 30, 0);
	}
        break;

	case WM_GETMINMAXINFO: {
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 600;
		lpMMI->ptMinTrackSize.y = 400;
	}
	    break;

    case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {

			case IDM_OPEN: {
				OPEN = true;
				HANDLE hl;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = PATH;
				ofn.nMaxFile = sizeof(PATH);
				ofn.lpstrFileTitle = titleWND;
				ofn.nMaxFileTitle = sizeof(titleWND);
				ofn.lpstrFilter = L"Текстовые файлы (*.txt)\0*.txt\0";
				ofn.nFilterIndex = 2;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetOpenFileName(&ofn) == TRUE) {
					hl = CreateFile(ofn.lpstrFile,
						GENERIC_READ | GENERIC_WRITE,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
					handlebank = hl;
				}
				ReadFile(handlebank, &Buff, sizeof(Buff), &BSize, NULL);
				SendMessage(EDITCONTROL, WM_SETTEXT, 0, (LPARAM)Buff);
				SendMessage(hPath, WM_SETTEXT, 0, (LPARAM)PATH);
			}
			break;

			case IDM_SAVE: {
				if (!OPEN) SendMessage(hwndArray[5], BM_CLICK, 0, 0);
				else {
					CloseHandle(handlebank);
					SendMessage(EDITCONTROL, WM_GETTEXT, MAX_LOADSTRING, (LPARAM)Buff);
					handlebank = CreateFile(PATH,
						GENERIC_READ | GENERIC_WRITE,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
					WriteFile(handlebank, &Buff, sizeof(Buff), &BSize, NULL);
				}
			}
				break;

			case IDM_SAVEAS: {
				HANDLE hl;
				OPENFILENAME sfn;
				TCHAR szSaveFileName[MAX_PATH];
				ZeroMemory(&sfn, sizeof(sfn));
				sfn.lStructSize = sizeof(sfn);
				sfn.hwndOwner = hWnd;
				sfn.lpstrFilter = L"Текстовые файлы (*.txt)\0*.txt\0";
				sfn.lpstrFile = PATH;
				sfn.lpstrFileTitle = titleWND;
				sfn.nMaxFileTitle = sizeof(titleWND);
				sfn.nMaxFile = MAX_PATH;
				sfn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
				sfn.lpstrDefExt = L".txt";
				if (GetSaveFileName(&sfn)) {
					hl = CreateFile(sfn.lpstrFile,
						GENERIC_READ | GENERIC_WRITE,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
					handlebank = hl;
					SendMessage(EDITCONTROL, WM_GETTEXT, MAX_LOADSTRING, (LPARAM)Buff);
					WriteFile(handlebank, &Buff, sizeof(Buff), &BSize, NULL);
					CloseHandle(handlebank);
				}

			}
				break;

			case IDM_EDUNDO:
				if (SendMessage(EDITCONTROL, EM_CANUNDO, 0, 0))
					SendMessage(EDITCONTROL, WM_UNDO, 0, 0);
				else {
					MessageBox(EDITCONTROL,
						L"Nothing to undo.",
						L"Undo notification",
						MB_OK);
				}
				break;

			case IDM_EDCUT:
				SendMessage(EDITCONTROL, WM_CUT, 0, 0);
				break;

			case IDM_EDCOPY:
				SendMessage(EDITCONTROL, WM_COPY, 0, 0);
				break;

			case IDM_EDPASTE:
				SendMessage(EDITCONTROL, WM_PASTE, 0, 0);
				break;

			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;

			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_DESTROY: {
		RegSetValueExA(hKey, (LPCSTR)L"l", 0, REG_DWORD, (const BYTE*)& wl, sizeof(wl));
		RegSetValueExA(hKey, (LPCSTR)L"h", 0, REG_DWORD, (const BYTE*)& wh, sizeof(wh));
		RegCloseKey(hKey);
		PostQuitMessage(0);
	}
    break;

	case WM_CLOSE:
		if (SendMessage(EDITCONTROL, WM_GETTEXT, MAX_LOADSTRING, (LPARAM)Buff)) {
			Buff;
			switch (int a = MessageBox(hWnd, L"Хотите сохранить изменения?", L"Выход", MB_YESNOCANCEL | MB_ICONQUESTION)) {
			case IDYES:
				SendMessage(hwndArray[4], BM_CLICK, 0, 0);
				DestroyWindow(hWnd);
				PostQuitMessage(0);
				break;
			case IDNO:
				DestroyWindow(hWnd);
				PostQuitMessage(0);
				break;
			}
		}
		else {
			DestroyWindow(hWnd);
			PostQuitMessage(0);
		}
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HWND CreateToolbar(HWND hwndParent) {
	HWND h_toolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwndParent, NULL, NULL, NULL);

	if (h_toolbar == NULL) return NULL;

	SendMessage(h_toolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	TBBUTTON tbb[7];
	TBADDBITMAP tbab;

	tbab.hInst = HINST_COMMCTRL;
	tbab.nID = IDB_STD_LARGE_COLOR;

	SendMessage(h_toolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);
	ZeroMemory(tbb, sizeof(tbb));

	tbb[0].iBitmap = STD_FILEOPEN;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].idCommand = IDM_OPEN;
	tbb[0].iString = (INT_PTR)L"Open";

	tbb[1].iBitmap = STD_FILESAVE;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].idCommand = IDM_SAVE;
	tbb[1].iString = (INT_PTR)L"Save";

	tbb[2].iBitmap = STD_FILESAVE;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;
	tbb[2].idCommand = IDM_SAVEAS;
	tbb[2].iString = (INT_PTR)L"SaveAS";

	tbb[3].iBitmap = STD_UNDO;
	tbb[3].fsState = TBSTATE_ENABLED;
	tbb[3].fsStyle = TBSTYLE_BUTTON;
	tbb[3].idCommand = IDM_EDUNDO;
	tbb[3].iString = (INT_PTR)L"Undo";

	tbb[4].iBitmap = STD_COPY;
	tbb[4].fsState = TBSTATE_ENABLED;
	tbb[4].fsStyle = TBSTYLE_BUTTON;
	tbb[4].idCommand = IDM_EDCOPY;
	tbb[4].iString = (INT_PTR)L"Copy";

	tbb[5].iBitmap = STD_CUT;
	tbb[5].fsState = TBSTATE_ENABLED;
	tbb[5].fsStyle = TBSTYLE_BUTTON;
	tbb[5].idCommand = IDM_EDCUT;
	tbb[5].iString = (INT_PTR)L"Cut";

	tbb[6].iBitmap = STD_PASTE;
	tbb[6].fsState = TBSTATE_ENABLED;
	tbb[6].fsStyle = TBSTYLE_BUTTON;
	tbb[6].idCommand = IDM_EDPASTE;
	tbb[6].iString = (INT_PTR)L"Paste";

	SendMessage(h_toolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(h_toolbar, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);

	SetWindowPos(h_toolbar, 0, 0, 0, 0, 0, 0);
	SendMessage(h_toolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(h_toolbar, TRUE);

	return h_toolbar;
}
