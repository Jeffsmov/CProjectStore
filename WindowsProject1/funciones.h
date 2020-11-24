#pragma once

void getTextFromIDEditBox(HWND hWnd, int id, TCHAR* texto) {
	HWND filedit = GetDlgItem(hWnd, id);
	GetWindowText(filedit, texto, MAX_TEXTSTRING);
}

void makeAllUsers() {
	//Old stuff, only use in case of needing to redo the files.
	//Does not include sells/products
	User* Aux = NULL;

	amountOfUsers = 3;

	UserList = new User;
	wcsncpy_s(UserList->Nombre, L"Buenos", MAX_TEXTSTRING);
	wcsncpy_s(UserList->Alias, L"DIAS", MAX_TEXTSTRING);
	wcsncpy_s(UserList->Pass, L"xDD", MAX_TEXTSTRING);
	wcsncpy_s(UserList->BMPRaiz, L"C:\\Users\\jeffm\\Pictures\\remjau.bmp", MAX_PATH); //Replace this with better images

	Aux = new User;
	wcsncpy_s(Aux->Nombre, L"Ayuda", MAX_TEXTSTRING);
	wcsncpy_s(Aux->Alias, L"AAAAAAAA", MAX_TEXTSTRING);
	wcsncpy_s(Aux->Pass, L"no", MAX_TEXTSTRING);
	wcsncpy_s(Aux->BMPRaiz, L"C:\\Users\\jeffm\\Pictures\\32287228.bmp", MAX_PATH);
	UserList->Sig = Aux;
	Aux->Ante = UserList;

	Aux = new User;
	wcsncpy_s(Aux->Nombre, L"Lal", MAX_TEXTSTRING);
	wcsncpy_s(Aux->Alias, L"LMAO", MAX_TEXTSTRING);
	wcsncpy_s(Aux->Pass, L"jaja", MAX_TEXTSTRING);
	wcsncpy_s(Aux->BMPRaiz, L"C:\\Users\\jeffm\\Pictures\\EAMUYQAU4AUrQyX.bmp", MAX_PATH);
	UserList->Sig->Sig = Aux;
	Aux->Ante = UserList->Sig;
}

bool writeFiles() {
	bool Exito = true;

	if (!CreateDirectory(L"C:\\Kirby", NULL) &&
		!ERROR_ALREADY_EXISTS == GetLastError())
	{
		MessageBox(0, L"Hubo un error al guardar los archivos. \n Vuelva a intentarlo", L"Error", MB_OK);
		return !Exito;
		// Failed to create directory.
	}

	if (!CreateDirectory(L"C:\\Kirby\\Archivos", NULL) &&
		!ERROR_ALREADY_EXISTS == GetLastError())
	{
		MessageBox(0, L"Hubo un error al guardar los archivos. \n Vuelva a intentarlo", L"Error", MB_OK);
		return !Exito;
	}


	User* Aux = UserList;

	ofstream nop(L"C:\\Kirby\\Archivos\\Users.kirby", ios::binary, ios::trunc);
	nop.close();

	ofstream KirbyUsuarios(L"C:\\Kirby\\Archivos\\Users.kirby", ios::binary, ios::trunc);

	if (!KirbyUsuarios.is_open()) {
		MessageBox(0, L"Hubo un error al guardar los archivos. \n Vuelva a intentarlo", L"Error", MB_OK);
		return !Exito;
	}

	KirbyUsuarios.write((char*)&amountOfUsers, sizeof(amountOfUsers));

	//Este loop le da a cada usuario su propia carpeta mientras que escribe la informacion de cada usuario en un solo archivo
	//En cada loop se crea un archivo de los productos y las ventas que cada usuario tiene y se escribe su informacion ahi, todo en la carpeta del usuario

	//Si, todo el programa esta en memoria. Si, si no se sale de forma adecuada se pierde la informacion. No, no es necesario hacer mas que esto.
	//Porque crees que a huevo quieren que solo puedas cerrar el programa con un boton en un menu?

	while (Aux != NULL) {
		TCHAR directory[MAX_PATH] = L"";
		TCHAR auxChar[MAX_PATH] = L"";

		wcsncpy_s(directory, L"C:\\Kirby\\Archivos", MAX_PATH); //directory ready

		wcsncpy_s(auxChar, L"\\", MAX_PATH);
		wcscat_s(auxChar, Aux->Nombre);
		wcscat_s(auxChar, L"\\"); //auxChar Ready

		wcscat_s(directory, auxChar); //User Folder Ready
		if (!CreateDirectory(directory, NULL) &&
			!ERROR_ALREADY_EXISTS == GetLastError())
		{
			MessageBox(0, L"Hubo un error al guardar los archivos. \n Vuelva a intentarlo", L"Error", MB_OK);
			return !Exito;
		}

		TCHAR dirPro[MAX_PATH] = L"";
		TCHAR dirSel[MAX_PATH] = L"";

		wcsncpy_s(dirPro, directory, MAX_PATH);
		wcsncpy_s(dirSel, directory, MAX_PATH);

		wcscat_s(dirPro, L"products.kirby");
		wcscat_s(dirSel, L"sells.kirby");

		ofstream sos(dirPro, ios::binary, ios::trunc);
		sos.close();
		ofstream non(dirSel, ios::binary, ios::trunc);
		non.close();

		ofstream KirbyProducts(dirPro, ios::binary, ios::trunc);
		ofstream KirbySells(dirSel, ios::binary, ios::trunc);

		if (!KirbyProducts.is_open() || !KirbySells.is_open()) {
			MessageBox(0, L"Hubo un error al guardar los archivos. \n Vuelva a intentarlo", L"Error", MB_OK);
			return !Exito;
		}

		KirbyUsuarios.write((char*)Aux, sizeof(User));
		Product* prodAux = Aux->FirstProduct;
		Sell* sellAux = Aux->FirstSell;

		Product* prodDel = NULL;
		Sell* selldDel = NULL;

		while (prodAux != NULL) {
			KirbyProducts.write((char*)prodAux, sizeof(Product));
			prodDel = prodAux;
			prodAux = prodAux->Sig;
			delete prodDel;
		}

		while (sellAux != NULL) {
			KirbySells.write((char*)sellAux, sizeof(Sell));
			selldDel = sellAux;
			sellAux = sellAux->Sig;
			delete selldDel;
		}

		Aux = Aux->Sig;

	}
	
	return Exito;
}

bool readFiles() {
	bool Exito = true;

	ifstream KirbyUsuarios(L"C:\\Kirby\\Archivos\\Users.kirby", ios::binary, ios::trunc);

	if (!KirbyUsuarios.is_open()) {
		MessageBox(0, L"No se encontro informacion previa del programa \n Porfavor Registrese", L"Advertencia", MB_OK);
		return !Exito;
	}

	KirbyUsuarios.read((char*)&amountOfUsers, sizeof(amountOfUsers));

	if (amountOfUsers==0) {
		MessageBox(0, L"No se encontro informacion previa del programa \n Porfavor Registrese", L"Advertencia", MB_OK);
		return !Exito;
	}
	
	User* aux = NULL;
	User* lastAux = NULL;

	for (int i = 0; i < amountOfUsers; i++) {
		aux = new User;
		KirbyUsuarios.read((char*)aux, sizeof(User));

		if (i == 0) {
			UserList = aux;
			aux->Ante = NULL;
		}
		else {
			lastAux->Sig = aux;
			aux->Ante = lastAux;
		}
		aux->Sig = NULL;

		TCHAR directory[MAX_PATH] = L"";
		TCHAR auxChar[MAX_PATH] = L"";
		TCHAR dirPro[MAX_PATH] = L"";
		TCHAR dirSel[MAX_PATH] = L"";

		wcsncpy_s(directory, L"C:\\Kirby\\Archivos", MAX_PATH); //directory ready

		wcsncpy_s(auxChar, L"\\", MAX_PATH);
		wcscat_s(auxChar, aux->Nombre);
		wcscat_s(auxChar, L"\\"); //auxChar Ready

		wcscat_s(directory, auxChar); //User Folder Ready

		wcsncpy_s(dirPro, directory, MAX_PATH);
		wcsncpy_s(dirSel, directory, MAX_PATH);

		wcscat_s(dirPro, L"products.kirby");
		wcscat_s(dirSel, L"sells.kirby");

		ifstream KirbyProducts(dirPro, ios::binary, ios::trunc);
		ifstream KirbySells(dirSel, ios::binary, ios::trunc);

		if (KirbyProducts.is_open()) {
			Product* auxProd = NULL;
			Product* lastAuxProd = NULL;

			for (int i = 0; i < aux->amountProds; i++) {
				auxProd = new Product;
				KirbyProducts.read((char*)auxProd, sizeof(Product));

				if (i == 0) {
					aux->FirstProduct = auxProd;
					auxProd->Ante = NULL;
				}
				else {
					lastAuxProd->Sig = auxProd;
					auxProd->Ante = lastAuxProd;
				}
				auxProd->Sig = NULL;

				lastAuxProd = auxProd;
			}
		}

		if (KirbySells.is_open()) {
			Sell* auxSell = NULL;
			Sell* lastAuxSell = NULL;
			for (int i = 0; i < aux->amountSells; i++) {
				auxSell = new Sell;
				KirbySells.read((char*)auxSell, sizeof(Sell));

				if (i == 0) {
					aux->FirstSell = auxSell;
					auxSell->Ante = NULL;
				}
				else {
					lastAuxSell->Sig = auxSell;
					auxSell->Ante = lastAuxSell;
				}
				auxSell->Sig = NULL;

				lastAuxSell = auxSell;
			}
		}

		lastAux = aux;
	}

	return Exito;
}

//User

void makeheapUser(User* a[], int n) {
	int i, k, j;
	User* temp;
	for (i = 1; i < n; i++)

	{
		temp = a[i];
		k = (i - 1) / 2;
		j = i;
		while (j > 0 && wcscmp(a[k]->Nombre, temp->Nombre) < 0)
		{
			a[j] = a[k];
			j = k;
			k = (j - 1) / 2;
		}
		a[j] = temp;
	}
}

void heapsortUser(User* a[], int n) {
	int i, j;
	User* temp;
	for (i = n - 1; i >= 1; i--)
	{
		temp = a[i];
		a[i] = a[0];
		a[0] = temp;
		makeheapUser(a, i);
	}
}

User** getAllUsersArray() {

	User* Aux = UserList; //jajaXDD
	User** usuArray = new User *[amountOfUsers];

	for (int i = 0; i < amountOfUsers; i++) {
		usuArray[i] = Aux;
		Aux = Aux->Sig;
	}

	makeheapUser(usuArray, amountOfUsers);
	heapsortUser(usuArray, amountOfUsers);

	return usuArray;
}

void deleteUserArrayAfterUse(User** useless) {
	delete[] useless;
}

User* getLastUser(){
	User* aux = UserList;

	if (!aux)
		return NULL;

	while (aux->Sig != NULL)
		aux = aux->Sig;

	return aux;
}

User* addUser(wchar_t Nombre[], wchar_t contraseña[]){
	User* aux = new User();

	wcsncpy_s(aux->Nombre, Nombre, MAX_TEXTSTRING);
	wcsncpy_s(aux->Pass, contraseña , MAX_TEXTSTRING);

	User* lastuser;
	if (!(lastuser = getLastUser())) {
		UserList = aux;
	}
	else {
		lastuser->Sig = aux;
		aux->Ante = lastuser;
	}

	amountOfUsers++;

	return aux;
}

int BusquedaUserName(User* arreglo[], int tamano, TCHAR clave[])
{
	int Iarriba = tamano - 1;
	int Iabajo = 0;
	int Icentro;
	while (Iabajo <= Iarriba)
	{
		Icentro = (Iarriba + Iabajo) / 2;
		if (wcscmp(arreglo[Icentro]->Nombre, clave) == 0)
			return Icentro;
		else
			if (wcscmp(arreglo[Icentro]->Nombre, clave) > 0)//(clave < arreglo[Icentro])
				Iarriba = Icentro - 1;
			else
				Iabajo = Icentro + 1;
	}
	return -1;
}

//Product

void makeheapNameProduct(Product* a[], int n) {
	int i, k, j;
	Product* temp;
	for (i = 1; i < n; i++)

	{
		temp = a[i];
		k = (i - 1) / 2;
		j = i;
		while (j > 0 && wcscmp(a[k]->Nombre, temp->Nombre) < 0)
		{
			a[j] = a[k];
			j = k;
			k = (j - 1) / 2;
		}
		a[j] = temp;
	}
}

void heapsortNameProduct(Product* a[], int n) {
	int i, j;
	Product* temp;
	for (i = n - 1; i >= 1; i--)
	{
		temp = a[i];
		a[i] = a[0];
		a[0] = temp;
		makeheapNameProduct(a, i);
	}
}

void makeheapIdProduct(Product* a[], int n) {
	int i, k, j;
	Product* temp;
	for (i = 1; i < n; i++)

	{
		temp = a[i];
		k = (i - 1) / 2;
		j = i;
		while (j > 0 && a[k]->idProduct < temp->idProduct)
		{
			a[j] = a[k];
			j = k;
			k = (j - 1) / 2;
		}
		a[j] = temp;
	}
}

void heapsortIdProduct(Product* a[], int n) {
	int i, j;
	Product* temp;
	for (i = n - 1; i >= 1; i--)
	{
		temp = a[i];
		a[i] = a[0];
		a[0] = temp;
		makeheapIdProduct(a, i);
	}
}

Product** getAllProductsArray() {

	Product* Aux = LoggedUser->FirstProduct; //jajaXDD
	Product** proArray = new Product * [LoggedUser->amountProds];

	for (int i = 0; i < LoggedUser->amountProds; i++) {
		proArray[i] = Aux;
		Aux = Aux->Sig;
	}

	makeheapIdProduct(proArray, LoggedUser->amountProds);
	heapsortIdProduct(proArray, LoggedUser->amountProds);

	return proArray;
}

void deleteProductArrayAfterUse(Product** useless) {
	delete[] useless;
}

Product* getLastProduct() {
	Product* aux = LoggedUser->FirstProduct;

	if (!aux)
		return NULL;

	while (aux->Sig != NULL)
		aux = aux->Sig;

	return aux;
}

Product* addProduct(TCHAR Nombre[], TCHAR Marca[], TCHAR *BMPRaiz[], int stock, float price, TCHAR info[]) {
	LoggedUser->lastProdId++;


	Product* aux = new Product();

	wcsncpy_s(aux->Nombre, Nombre, MAX_TEXTSTRING);
	wcsncpy_s(aux->Marca, Marca, MAX_TEXTSTRING);
	wcsncpy_s(aux->BMPRaiz[0], BMPRaiz[0], MAX_PATH);
	wcsncpy_s(aux->BMPRaiz[1], BMPRaiz[1], MAX_PATH);
	wcsncpy_s(aux->info, info, MAX_LOADSTRING);
	aux->stock = stock;
	aux->price = price;
	aux->idProduct = LoggedUser->lastProdId;

	Product* lastProduct;
	if (!(lastProduct = getLastProduct())) {
		LoggedUser->FirstProduct = aux;
	}
	else {
		lastProduct->Sig = aux;
		aux->Ante = lastProduct;
	}

	LoggedUser->amountProds++;

	return aux;
}

bool isProductRelevant(int idProduct) {
	Sell* Aux = LoggedUser->FirstSell; //jajaXDD
	
	while (Aux != NULL) {
		if (Aux->idProduct == idProduct)
			return true;
		Aux = Aux->Sig;
	}

	return false;
}

int BusquedaProductName(Product* arreglo[], int tamano, TCHAR clave[])
{
	int Iarriba = tamano - 1;
	int Iabajo = 0;
	int Icentro;
	while (Iabajo <= Iarriba)
	{
		Icentro = (Iarriba + Iabajo) / 2;
		if (wcscmp(arreglo[Icentro]->Nombre, clave) == 0)
			return Icentro;
		else
			if (wcscmp(arreglo[Icentro]->Nombre, clave) > 0)//(clave < arreglo[Icentro])
				Iarriba = Icentro - 1;
			else
				Iabajo = Icentro + 1;
	}
	return -1;
}

int BusquedaProductId(Product* arreglo[], int tamano, int clave)
{
	int Iarriba = tamano - 1;
	int Iabajo = 0;
	int Icentro;
	while (Iabajo <= Iarriba)
	{
		Icentro = (Iarriba + Iabajo) / 2;
		if (arreglo[Icentro]->idProduct == clave)
			return Icentro;
		else
			if (clave < arreglo[Icentro]->idProduct)//(clave < arreglo[Icentro]) // clave < arreglo[Icentro]->idProduct
				Iarriba = Icentro - 1;
			else
				Iabajo = Icentro + 1;
	}
	return -1;
}

//Sell

void makeheapIdSell(Sell* a[], int n) {
	int i, k, j;
	Sell* temp;
	for (i = 1; i < n; i++)

	{
		temp = a[i];
		k = (i - 1) / 2;
		j = i;
		while (j > 0 && a[k]->idSell < temp->idSell)
		{
			a[j] = a[k];
			j = k;
			k = (j - 1) / 2;
		}
		a[j] = temp;
	}
}

void heapsortIdSell(Sell* a[], int n) {
	int i, j;
	Sell* temp;
	for (i = n - 1; i >= 1; i--)
	{
		temp = a[i];
		a[i] = a[0];
		a[0] = temp;
		makeheapIdSell(a, i);
	}
}

Sell** getAllSellsArray() {

	Sell* Aux = LoggedUser->FirstSell; //jajaXDD
	Sell** sellsArray = new Sell * [LoggedUser->amountSells];

	for (int i = 0; i < LoggedUser->amountSells; i++) {
		sellsArray[i] = Aux;
		Aux = Aux->Sig;
	}

	makeheapIdSell(sellsArray, LoggedUser->amountProds);
	heapsortIdSell(sellsArray, LoggedUser->amountProds);

	return sellsArray;
}

void deleteSellArrayAfterUse(Sell** useless) {
	delete[] useless;
}

Sell* getLastSell() {
	Sell* aux = LoggedUser->FirstSell;

	if (!aux)
		return NULL;

	while (aux->Sig != NULL)
		aux = aux->Sig;

	return aux;
}

Sell* getLastSendedSell() {
	Sell* aux = LoggedUser->SendedSells;

	if (!aux)
		return NULL;

	while (aux->Sig != NULL)
		aux = aux->Sig;

	return aux;
}

Sell* addSell(int idProduct, int cantidad, tm fechaEnvio, TCHAR Calle[], TCHAR Colonia[], TCHAR Ciudad[], TCHAR Estado[], TCHAR Mensaje[]) {

	LoggedUser->lastSellId++;

	Sell* aux = new Sell();

	wcsncpy_s(aux->Calle, Calle, MAX_TEXTSTRING);
	wcsncpy_s(aux->Colonia, Colonia, MAX_TEXTSTRING);
	wcsncpy_s(aux->Ciudad, Ciudad, MAX_TEXTSTRING);
	wcsncpy_s(aux->Estado, Estado, MAX_TEXTSTRING);
	wcsncpy_s(aux->Mensaje, Mensaje, MAX_LOADSTRING);
	aux->idProduct = idProduct;
	aux->cantidad = cantidad;
	aux->fechaEnvio = fechaEnvio;
	aux->idSell = LoggedUser->lastSellId;


	Sell* lastSell;
	if (!(lastSell = getLastSell())) {
		LoggedUser->FirstSell = aux;
	}
	else {
		lastSell->Sig = aux;
		aux->Ante = lastSell;
	}

	LoggedUser->amountSells++;

	return aux;
}

void sendSell(int idSell) {
	Sell* Aux = LoggedUser->FirstSell; //jajaXDD

	while (Aux != NULL) {
		if (Aux->idSell == idSell)
			break;
		Aux = Aux->Sig;
	}
	if (Aux == NULL) {
		return;//ERROR idSell not found
	}

	LoggedUser->amountSells--;
	if (Aux->Ante != NULL) {
		Aux->Ante->Sig = Aux->Sig;
	}
	if (Aux->Sig) {
		Aux->Sig->Ante = Aux->Ante;
	}

	Aux->Ante = NULL;
	Aux->Sig = NULL;

	Sell* lastSell;
	if (!(lastSell = getLastSendedSell())) {
		LoggedUser->SendedSells = Aux;
	}
	else {
		lastSell->Sig = Aux;
		Aux->Ante = lastSell;
	}

	LoggedUser->amountSendedSells++;

}

int BusquedaSellId(Sell* arreglo[], int tamano, int clave)
{
	int Iarriba = tamano - 1;
	int Iabajo = 0;
	int Icentro;
	while (Iabajo <= Iarriba)
	{
		Icentro = (Iarriba + Iabajo) / 2;
		if (arreglo[Icentro]->idSell == clave)
			return Icentro;
		else
			if (clave < arreglo[Icentro]->idSell)//(clave < arreglo[Icentro]) // clave < arreglo[Icentro]->idProduct
				Iarriba = Icentro - 1;
			else
				Iabajo = Icentro + 1;
	}
	return -1;
}

/*
TODO:
much more bullshit plz help me aaaaaa
*/