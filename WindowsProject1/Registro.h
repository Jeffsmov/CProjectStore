#pragma once

INT_PTR CALLBACK registroCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//Set Dialog Icon
		//HICON hicon = (HICON)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LAFOTOTIENDA));
		//SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
		return (INT_PTR)TRUE;

	}break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		
		case IDC_REGISTRARTE:
		{
			TCHAR userName[MAX_LOADSTRING];
			TCHAR pass[MAX_LOADSTRING];

			getTextFromIDEditBox(hWnd, IDC_USERNAME, userName);
			getTextFromIDEditBox(hWnd, IDC_USERPASS, pass);

			/*MessageBox(hWnd, userName, L"Username", 0);
			MessageBox(hWnd, pass, L"Password", 0);*/

			User** xd = getAllUsersArray();
			int num = BusquedaUserName(xd, amountOfUsers, userName);

			if (num == -1) {
				//TODO: Usuario Disponible
				LoggedUser = addUser(userName, pass);
				MessageBox(hWnd, L"Usuario creado con exito", L"Exito", 0);
				EndDialog(hWnd, true);
				return true;
			}
			else {
				//TODO: Usuario No Disponible
				MessageBox(hWnd, L"Usuario no disponible", L"Error", 0);
			}

			//int num = BusquedaUserName(xd, amountOfUsers, userName);
			//MessageBox(hWnd, xd[num].Nombre, L"Usuario", 0);
		}
		break;

		case IDM_EXIT:
		case IDCANCEL:
		{
			EndDialog(hWnd, true);
			return true;
		}
		break;
		}
	}
	break;
	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		BITMAP      bitmap01;
		HGDIOBJ		oldBitmap01;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC         hdcMem01;
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		EndDialog(hWnd, true);
		return true;
		break;
	}
	return (INT_PTR)FALSE;
}