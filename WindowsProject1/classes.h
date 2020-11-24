#pragma once

struct Product {
	Product* Ante = NULL;
	int idProduct=0;
	TCHAR Nombre[MAX_TEXTSTRING];
	TCHAR Marca[MAX_TEXTSTRING];
	TCHAR info[MAX_LOADSTRING];
	TCHAR BMPRaiz[2][MAX_PATH];
	int stock;
	float price;
	Product* Sig = NULL;
};

struct Sell {
	Sell* Ante = NULL;
	int idSell;
	TCHAR Calle[MAX_TEXTSTRING];
	TCHAR Colonia[MAX_TEXTSTRING];
	TCHAR Ciudad[MAX_TEXTSTRING];
	TCHAR Estado[MAX_TEXTSTRING];
	TCHAR Mensaje[MAX_LOADSTRING];
	tm fechaEnvio;
	int idProduct;
	int cantidad; //Precio total es calculado usando Cantidad x Precio del producto
	Sell* Sig = NULL;
};

struct User {
	User* Ante = NULL;
	TCHAR Nombre[MAX_TEXTSTRING];
	TCHAR Alias[MAX_TEXTSTRING];
	TCHAR BMPRaiz[MAX_PATH];
	TCHAR Pass[MAX_TEXTSTRING];
	Product* FirstProduct = NULL;
	int amountProds = 0;
	int lastProdId = 0;
	Sell* FirstSell = NULL;
	Sell* SendedSells = NULL; //Sells History, sells move to this list after being send.
	int amountSells = 0;
	int amountSendedSells = 0;
	int lastSellId = 0;
	User* Sig = NULL;
};