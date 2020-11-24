#pragma once

void loadUserInfo(HWND &hWnd);
void toggleUserInfo(HWND &hWnd, bool& toggleInfo);
void applyUserInfoChanges(HWND &hWnd);

INT_PTR CALLBACK UserProfile(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    HBITMAP bmp2;
    static bool toggleInfo = false; //false = Not editing information
    OPENFILENAME ofn;

    TCHAR szFileName[MAX_PATH] = L"";
    ZeroMemory(&(ofn), sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFileName;
    ofn.hwndOwner = hWnd;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        loadUserInfo(hWnd);
        toggleInfo = true;
        toggleUserInfo(hWnd, toggleInfo);

        return (INT_PTR)TRUE;

    }break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
        case IDC_BTN_EDIT: 
        {
            if(toggleInfo) //If this happens, you are canceling the edit, thus change back to the unedited info.
                loadUserInfo(hWnd);

            toggleUserInfo(hWnd, toggleInfo);
        }
        break;

        case IDC_BTN_APPLY:
        {
            applyUserInfoChanges(hWnd);
            toggleUserInfo(hWnd, toggleInfo);
        }
        break;

        case IDC_BTN_IMG:
        {
            /*TODO: change image*/
            //SendDlgItemMessage(hWnd, IDC_IMGPATH, WM_SETTEXT, NULL, (LPARAM)L"I need to do the pick img plzzzzz");

            ofn.lpstrFilter = L"BITMAP (*.bmp)\0*.bmp\0";

            ofn.nMaxFile = MAX_PATH;

            ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

            TCHAR tempChar[MAX_PATH] = L"";

            if (GetOpenFileName(&ofn)) {
                wcscpy_s(tempChar, ofn.lpstrFile);
                bmp2 = (HBITMAP)LoadImage(NULL, tempChar, IMAGE_BITMAP, 300, 300, LR_LOADFROMFILE);
                SendDlgItemMessage(hWnd, IDC_IMG_USER, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);

                SendDlgItemMessage(hWnd, IDC_IMGPATH, WM_SETTEXT, NULL, (LPARAM)tempChar);
            }
            else { MessageBox(hWnd, L"No se a seleccionado un archivo", L"Error", MB_ICONERROR); }

        }
        break;

		case IDM_EXIT:
		{
			EndDialog(hWnd, true);
            return (INT_PTR)TRUE;
		}
		break;
		}
	}
	break;


    case WM_DESTROY:
        EndDialog(hWnd, true);
        return (INT_PTR)TRUE;
        break;

    }
    return (INT_PTR)FALSE;
}

void loadUserInfo(HWND &hWnd) {
    HBITMAP bmp2;

    SendDlgItemMessage(hWnd, IDC_USERNAME, WM_SETTEXT, NULL, (LPARAM)LoggedUser->Nombre);
    SendDlgItemMessage(hWnd, IDC_PASSWORD, WM_SETTEXT, NULL, (LPARAM)LoggedUser->Pass);
    SendDlgItemMessage(hWnd, IDC_COMPANY, WM_SETTEXT, NULL, (LPARAM)LoggedUser->Alias);
    SendDlgItemMessage(hWnd, IDC_IMGPATH, WM_SETTEXT, NULL, (LPARAM)LoggedUser->BMPRaiz);

    bmp2 = (HBITMAP)LoadImage(NULL, LoggedUser->BMPRaiz, IMAGE_BITMAP, 300, 300, LR_LOADFROMFILE);
    SendDlgItemMessage(hWnd, IDC_IMG_USER, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
}

void toggleUserInfo(HWND &hWnd, bool& toggleInfo) {
    toggleInfo = !toggleInfo;

    EnableWindow(GetDlgItem(hWnd, IDC_PASSWORD), toggleInfo);
    EnableWindow(GetDlgItem(hWnd, IDC_COMPANY), toggleInfo);
    EnableWindow(GetDlgItem(hWnd, IDC_BTN_IMG), toggleInfo);
    ShowWindow(GetDlgItem(hWnd, IDC_BTN_APPLY), (toggleInfo) ? SW_SHOW : SW_HIDE);
    SendDlgItemMessage(hWnd, IDC_BTN_EDIT, WM_SETTEXT, NULL, (LPARAM)((toggleInfo)? L"Cancel" : L"Edit..."));
    SendDlgItemMessage(hWnd, IDC_COMPANY, WM_KILLFOCUS, NULL, NULL);

    UpdateWindow(hWnd);
}

void applyUserInfoChanges(HWND &hWnd) {
    //TODO: Here we go from the edit controls to logging users

    getTextFromIDEditBox(hWnd, IDC_PASSWORD, LoggedUser->Pass);
    getTextFromIDEditBox(hWnd, IDC_COMPANY, LoggedUser->Alias);
    getTextFromIDEditBox(hWnd, IDC_IMGPATH, LoggedUser->BMPRaiz);

}