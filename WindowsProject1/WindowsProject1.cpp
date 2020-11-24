// WindowsProject1.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <commdlg.h>
#include <string>
#include <string.h>
#include "resource.h"
#include <mmsystem.h>
#include <fstream>
#include <direct.h>
#include <ctime>
#include <minwinbase.h>

#pragma comment(lib, "winmm.lib")

#include "framework.h"
#include "WindowsProject1.h"

using namespace std;
#define MAX_LOADSTRING 100
#define MAX_TEXTSTRING 35
// Global Variables:
HINSTANCE hInst;	//current instance

// Forward declarations of functions included in this code module:
BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//Globales 2 XDDDDD
#include "classes.h"
User* UserList = NULL;
int amountOfUsers = 0;
User* LoggedUser = NULL;

//Todo la basura aqui
#include "funciones.h"
#include "registro.h"
#include "UserProfile.h"

#include "login.h"
//XDDD


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;
    //makeAllUsers(); //This is temp, here the correct thing to use would be...
    readFiles(); //Fuck you I want to die

    //I still need to find a good place to
    //writeFiles(); //Perhaps when I destroy the login dialog... //Remember to hide login dialog after successful login...

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateDialog(hInst, MAKEINTRESOURCEW(IDD_LOGIN), NULL, loginCallback);
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


INT_PTR CALLBACK About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, true);
            return (INT_PTR)TRUE;
        }
        break;

    case WM_DESTROY:
        EndDialog(hWnd, true);
        break;

    }
    return (INT_PTR)FALSE;
}
