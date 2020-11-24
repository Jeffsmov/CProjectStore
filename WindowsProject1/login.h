#pragma once

INT_PTR CALLBACK loginCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//Set Dialog Icon
		//HICON hicon = (HICON)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LAFOTOTIENDA));
		//SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
		return (INT_PTR)TRUE;

	}break;

	//BUG:
	//Weird bug in login, can't really think of why it happens, but you can't tab to switch editboxes. Only happens on Logging so far, userprofile does allow tab. (Check regist later)
	//Checking an empty winapi proyect doesn't show any proof that I fucked up something so far...
	//Perhaps it is that the main window of the progam is actually a dialog and winapi doesn't really like that? Maybe a focus or something can fix this issue...
	//Check keyboard/mouse behavior in winapi32 later after I finish the project. This is a not important bug.

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, (DLGPROC)About); //Ventana y el callback
		}
		break;
		case IDC_LOGIN:
		{
			TCHAR userName[MAX_LOADSTRING];
			TCHAR pass[MAX_LOADSTRING];

			getTextFromIDEditBox(hWnd, IDC_USERNAME, userName);
			getTextFromIDEditBox(hWnd, IDC_USERPASS, pass);

			User** xd = getAllUsersArray();
			int num = BusquedaUserName(xd, amountOfUsers, userName);

			if (num == -1) {
				MessageBox(hWnd, L"Informacion incorrecta", L"Error", 0);
			}
			else {
				if (wcscmp(xd[num]->Pass, pass)==0) {
					MessageBox(hWnd, L"Buenos dias", L"Exito", 0);
					LoggedUser = xd[num];

					//ShowWindow(hWnd, SW_HIDE);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_USERPROFILE), hWnd, (DLGPROC)UserProfile); //Ventana y el callback
					//ShowWindow(hWnd, SW_SHOW);
					LoggedUser = NULL;
				}
				else {
					MessageBox(hWnd, L"Informacion incorrecta", L"Error", 0);
				}
			}

			deleteUserArrayAfterUse(xd);
		}
		break;
		case IDC_REGISTRO:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_REGISTRO), hWnd, (DLGPROC)registroCallback); //Ventana y el callback
		}
		break;

		case IDM_EXIT:
		case IDCANCEL:
		{
			DestroyWindow(hWnd);
			return (INT_PTR)TRUE;
		}
		break;
		}
	}
	break;
	case WM_DESTROY:
		writeFiles();
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}