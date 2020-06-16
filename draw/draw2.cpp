
#include "stdafx.h"
#include "draw2.h"

#pragma region zmienne
//moje zmienne 
const int lokXMax = 10;
const int lokYMax = 10;
const int maxObiektow = 8;
const int rozmiarObiektu = 40;
const int rozmiarHaka = 4;
const int czas = 25;
const int czas2 = 25;
const int multipliAnim = 4;

int liczbaObiektow;
int trybAnim;
int szukanyTyp;
int liczbaPowtorzen;
bool czyMoznaDzialac;

int szukanaX;
int szukanaY;
int docelowaX;
int docelowaY;
int tymczasowaX;
int tymczasowaY;

struct pozycja
{
	int x;
	int y;
};

struct lokacja
{
	bool czyZajeta;
	bool czyUziemiona;
	int idObiektu;
	pozycja pozycja;
};

struct obiekt
{
	bool czyIstnieje;
	unsigned short int typ;
	pozycja lokacja;
	pozycja pozycja;
};

struct hakDzwigu {
	bool czyZaczepiony;
	int idObiektu;
	pozycja lokacja;
	pozycja pozycja;
};

hakDzwigu hak;
obiekt *obiekty[maxObiektow];
lokacja *lokacje[lokXMax+1][lokYMax+1];


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

												// buttons
HWND hwndButton;

RECT drawArea1 = { 0, 0, 600, 800 };

#pragma endregion

#pragma region funkcje
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);
#pragma endregion

void Obliczenia() {

	if (hak.czyZaczepiony) {
		obiekty[hak.idObiektu]->lokacja.x = hak.lokacja.x;
		obiekty[hak.idObiektu]->lokacja.y = hak.lokacja.y;
	}

	for (int i = 0; i <= lokXMax; i++)
	{
		for (int j = 0; j <= lokYMax; j++) {
			lokacje[i][j]->czyZajeta = false;
			if (j != lokYMax) {
				lokacje[i][j]->czyUziemiona = false;
			}
			else
			{
				lokacje[i][j]->czyUziemiona = true;
			}
		}
	}

	for (int i = 0; i < maxObiektow; i++) {
		if (obiekty[i]->czyIstnieje) {
			obiekty[i]->pozycja.x = lokacje[obiekty[i]->lokacja.x][obiekty[i]->lokacja.y]->pozycja.x;
			obiekty[i]->pozycja.y = lokacje[obiekty[i]->lokacja.x][obiekty[i]->lokacja.y]->pozycja.y;
			lokacje[obiekty[i]->lokacja.x][obiekty[i]->lokacja.y]->czyZajeta = true;
			lokacje[obiekty[i]->lokacja.x][obiekty[i]->lokacja.y]->idObiektu = i;
			if (obiekty[i]->lokacja.y>0) lokacje[obiekty[i]->lokacja.x][obiekty[i]->lokacja.y - 1]->czyUziemiona = true;
		}
	}

	hak.pozycja.x = lokacje[hak.lokacja.x][hak.lokacja.y]->pozycja.x + rozmiarObiektu / 2 - rozmiarHaka / 2;
	hak.pozycja.y = lokacje[hak.lokacja.x][hak.lokacja.y]->pozycja.y - rozmiarHaka;
}

void Rysuj(HDC hdc)
{
	Graphics graphics(hdc);

	Pen pen(Color(255, 0, 255, 0));
	Pen pen2(Color(255, 0, 0, 0));

	SolidBrush brush1(Color(255, 0, 128, 128));
	SolidBrush brushB(Color(255, 128, 128, 128));
	SolidBrush brush2(Color(255, 128, 0, 0));
	SolidBrush brush3(Color(255, 0, 255, 0));

	graphics.FillRectangle(&brushB, rozmiarObiektu/4, rozmiarObiektu/4,
		rozmiarObiektu/2, (lokYMax + 2) * rozmiarObiektu + 3* rozmiarObiektu/4);
	graphics.FillRectangle(&brushB, rozmiarObiektu/4, rozmiarObiektu/4,
		(lokXMax+2) * rozmiarObiektu, rozmiarObiektu/2);
	graphics.DrawLine(&pen2, 0, (lokYMax + 3) *rozmiarObiektu + 1, 
		(lokXMax+2) * rozmiarObiektu, (lokYMax+3) * rozmiarObiektu + 1);
	
	for (int i = 0; i < maxObiektow; i++) {
		if (obiekty[i]->czyIstnieje) {
			switch (obiekty[i]->typ)
			{
			case 1:
				graphics.FillEllipse(&brush2, obiekty[i]->pozycja.x, obiekty[i]->pozycja.y, rozmiarObiektu, rozmiarObiektu);
				break;
			case 2:
				graphics.FillRectangle(&brush1, obiekty[i]->pozycja.x, obiekty[i]->pozycja.y,
					rozmiarObiektu, rozmiarObiektu);
				break;
			case 3:
				Point p1 = { obiekty[i]->pozycja.x + rozmiarObiektu / 2,obiekty[i]->pozycja.y };
				Point p2 = { obiekty[i]->pozycja.x + rozmiarObiektu ,obiekty[i]->pozycja.y + rozmiarObiektu };
				Point p3 = { obiekty[i]->pozycja.x ,obiekty[i]->pozycja.y + rozmiarObiektu };
				Point trojkat[] = { p1,p2,p3 };
				graphics.FillPolygon(&brush3, trojkat, 3);
				break;		
			}
		}
	}

	graphics.FillRectangle(&brushB, hak.pozycja.x, hak.pozycja.y, rozmiarHaka, rozmiarHaka);

	graphics.DrawLine(&pen2, hak.pozycja.x + rozmiarHaka/2, 3*rozmiarObiektu/4,
		hak.pozycja.x + rozmiarHaka/2, hak.pozycja.y );
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea==NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	//Obliczenia();
	Rysuj(hdc);
	EndPaint(hWnd, &ps);
}

void Start()
{
	liczbaObiektow = 0;
	liczbaPowtorzen = 0;

	for (int i = 0; i <= lokXMax; i++)
	{
		for (int j = 0; j <= lokYMax; j++) {
			lokacje[i][j] = new lokacja;
			lokacje[i][j]->pozycja.x = (i+1) * rozmiarObiektu ;
			lokacje[i][j]->pozycja.y = (j+2) * rozmiarObiektu ;
			lokacje[i][j]->czyZajeta = false;
			if (j == lokYMax) {
				lokacje[i][j]->czyUziemiona = true;
			}
			else
			{
				lokacje[i][j]->czyUziemiona = false;
			}
		}
	}
	for (int i = 0; i < maxObiektow; i++) 
	{
		obiekty[i] = new obiekt;
		obiekty[i]->czyIstnieje = false;
	}

	hak.lokacja.x = lokXMax;
	hak.lokacja.y = lokYMax;
	hak.pozycja.x = lokacje[hak.lokacja.x][hak.lokacja.y]->pozycja.x + rozmiarObiektu / 2 - rozmiarHaka / 2;
	hak.pozycja.y = lokacje[hak.lokacja.x][hak.lokacja.y]->pozycja.y - rozmiarHaka;
	hak.czyZaczepiony = false;

	czyMoznaDzialac = true;
}

#pragma region main

// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}
#pragma endregion

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	
	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       
	
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("UP"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		655, 5,                                  // the left and top co-ordinates
		50, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)BUTTON_UP,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DOWN"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		655, 115,                                  // the left and top co-ordinates
		50, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)BUTTON_DOWN,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      
		TEXT("HOOK"),                  
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
		655, 60,                                  
		50, 50,                              
		hWnd,                                 
		(HMENU)BUTTON_HOOK,                   
		hInstance,                            
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("LEFT"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 60,
		50, 50,
		hWnd,
		(HMENU)BUTTON_LEFT,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("RIGHT"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		710, 60,
		50, 50,
		hWnd,
		(HMENU)BUTTON_RIGHT,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("ADD C"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 255,
		50, 50,
		hWnd,
		(HMENU)BUTTON_AD_1,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("ADD S"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		655, 255,
		50, 50,
		hWnd,
		(HMENU)BUTTON_AD_2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("ADD T"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		710, 255,
		50, 50,
		hWnd,
		(HMENU)BUTTON_AD_3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("AUTO"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		655, 185,
		50, 50,
		hWnd,
		(HMENU)BUTTON_AUTO,
		hInstance,
		NULL);

	Start();

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case BUTTON_UP:
			if (czyMoznaDzialac) {
				if (hak.lokacja.y != 0) {
					czyMoznaDzialac = false;
					szukanyTyp = 1;
					liczbaPowtorzen = 0;
					SetTimer(hWnd, TMR_2, czas2, 0);
					hak.lokacja.y -= 1;
				}
			}
			break;
		case BUTTON_DOWN:
			if (czyMoznaDzialac) {
				if (hak.lokacja.y != lokYMax) {
					if (hak.czyZaczepiony) {
						if (!lokacje[hak.lokacja.x][hak.lokacja.y + 1]->czyZajeta) {
							czyMoznaDzialac = false;
							szukanyTyp = 2;
							liczbaPowtorzen = 0;
							SetTimer(hWnd, TMR_2, czas2, 0);
							hak.lokacja.y += 1;
						}
					}
					else
					{
						if (!lokacje[hak.lokacja.x][hak.lokacja.y]->czyZajeta) {
							czyMoznaDzialac = false;
							szukanyTyp = 2;
							liczbaPowtorzen = 0;
							SetTimer(hWnd, TMR_2, czas2, 0);
							hak.lokacja.y += 1;
						}
					}
				}
			}
			break;
		case BUTTON_LEFT:
			if (czyMoznaDzialac) {
				if (hak.lokacja.x != 0) {
					if (hak.czyZaczepiony) {
						if (!lokacje[hak.lokacja.x - 1][hak.lokacja.y]->czyZajeta) {
							czyMoznaDzialac = false;
							szukanyTyp = 3;
							liczbaPowtorzen = 0;
							SetTimer(hWnd, TMR_2, czas2, 0);
							hak.lokacja.x -= 1;
						}
					}
					else
					{
						if (hak.lokacja.y != 0) {
							if (!lokacje[hak.lokacja.x - 1][hak.lokacja.y - 1]->czyZajeta) {
								czyMoznaDzialac = false;
								szukanyTyp = 3;
								liczbaPowtorzen = 0;
								SetTimer(hWnd, TMR_2, czas2, 0);
								hak.lokacja.x -= 1;
							}
						}
						else
						{
							czyMoznaDzialac = false;
							szukanyTyp = 3;
							liczbaPowtorzen = 0;
							SetTimer(hWnd, TMR_2, czas2, 0);
							hak.lokacja.x -= 1;
						}
					}
				}
			}
			break;
		case BUTTON_RIGHT:
			if (czyMoznaDzialac) {
				if (hak.lokacja.x != lokXMax) {
					if (hak.czyZaczepiony) {
						if (!lokacje[hak.lokacja.x + 1][hak.lokacja.y]->czyZajeta) {
							czyMoznaDzialac = false;
							szukanyTyp = 4;
							liczbaPowtorzen = 0;
							SetTimer(hWnd, TMR_2, czas2, 0);
							hak.lokacja.x += 1;
						}
					}
					else
					{
						if (hak.lokacja.y != 0) {
							if (!lokacje[hak.lokacja.x + 1][hak.lokacja.y - 1]->czyZajeta) {
								czyMoznaDzialac = false;
								szukanyTyp = 4;
								liczbaPowtorzen = 0;
								SetTimer(hWnd, TMR_2, czas2, 0);
								hak.lokacja.x += 1;
							}
						}
						else
						{
							czyMoznaDzialac = false;
							szukanyTyp = 4;
							liczbaPowtorzen = 0;
							SetTimer(hWnd, TMR_2, czas2, 0);
							hak.lokacja.x += 1;
						}
					}
				}
			}
			
			break;
		case BUTTON_HOOK:
			if (czyMoznaDzialac) {
				if (!hak.czyZaczepiony) {
					if (lokacje[hak.lokacja.x][hak.lokacja.y]->czyZajeta) {
						hak.czyZaczepiony = true;
						hak.idObiektu = lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu;
						Obliczenia();
						repaintWindow(hWnd, hdc, ps, &drawArea1);
					}
				}
				else if (lokacje[hak.lokacja.x][hak.lokacja.y]->czyUziemiona) {
					hak.czyZaczepiony = false;
					lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu = hak.idObiektu;
					Obliczenia();
					repaintWindow(hWnd, hdc, ps, &drawArea1);
				}
			}
			break;
		case BUTTON_AD_1:
			if ((!lokacje[lokXMax][lokYMax]->czyZajeta) && (liczbaObiektow < maxObiektow)) {
				obiekty[liczbaObiektow]->lokacja.x = lokXMax;
				obiekty[liczbaObiektow]->lokacja.y = lokYMax;
				obiekty[liczbaObiektow]->czyIstnieje = true;
				obiekty[liczbaObiektow]->typ = 1;
				liczbaObiektow++;
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;
		case BUTTON_AD_2:
			if ((!lokacje[lokXMax][lokYMax]->czyZajeta) && (liczbaObiektow < maxObiektow)) {
				obiekty[liczbaObiektow]->lokacja.x = lokXMax;
				obiekty[liczbaObiektow]->lokacja.y = lokYMax;
				obiekty[liczbaObiektow]->czyIstnieje = true;
				obiekty[liczbaObiektow]->typ = 2;
				liczbaObiektow++;
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;
		case BUTTON_AD_3:
			if ((!lokacje[lokXMax][lokYMax]->czyZajeta) && (liczbaObiektow < maxObiektow)) {
				obiekty[liczbaObiektow]->lokacja.x = lokXMax;
				obiekty[liczbaObiektow]->lokacja.y = lokYMax;
				obiekty[liczbaObiektow]->czyIstnieje = true;
				obiekty[liczbaObiektow]->typ = 3;
				liczbaObiektow++;
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;
		case BUTTON_AUTO:
			if ((czyMoznaDzialac)&&(!hak.czyZaczepiony)) {
				czyMoznaDzialac = false;
				szukanyTyp = 1;
				for (int i = lokXMax; i >= 0; i--) {
					if (!lokacje[i][lokYMax]->czyZajeta) {
						docelowaX = i;
					}
				}
				for (int i = lokXMax; i >= 0; i--) {
					if ((!lokacje[i][lokYMax]->czyZajeta)&&(docelowaX!=i)) {
						tymczasowaX = i;
					}
				}
				szukanaY = 0;
				tymczasowaY = lokYMax;
				docelowaY = lokYMax;
				trybAnim = 1;
				SetTimer(hWnd, TMR_1, czas, 0);
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Rysuj(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
#pragma region timer1
		case TMR_1:
			switch (trybAnim)
			{
			case 1:
				if (hak.lokacja.y > szukanaY) {
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					hak.lokacja.y++;
				}
				else
				{
					tymczasowaY = lokYMax;
					bool czyJestSzukany = false;
					KillTimer(hWnd, TMR_1);
					for (int i = 0; i < maxObiektow; i++) {
						if (obiekty[i]->typ == szukanyTyp) {
							szukanaX = obiekty[i]->lokacja.x;
							szukanaY = obiekty[i]->lokacja.y;
							czyJestSzukany = true;
						}
					}
					if (!czyJestSzukany) {
						szukanyTyp = 1;
						czyMoznaDzialac = true;
					}
					else
					{
						for (int i = szukanaY; i > 0; i--) {
							if (lokacje[szukanaX][i - 1]->czyZajeta) {
								liczbaPowtorzen++;
							}
						}
						if (liczbaPowtorzen == 0) {
							trybAnim = 2;
						}
						else
						{
							szukanaY -= liczbaPowtorzen;
							trybAnim = 8;
						}
						SetTimer(hWnd, TMR_1, czas, 0);
					}
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 2:
				if (hak.lokacja.x > szukanaX) {
					hak.lokacja.x--;
				}
				else if (hak.lokacja.x < szukanaX) {
					hak.lokacja.x++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 3;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 3:
				if (hak.lokacja.y > szukanaY) {
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 4;
					szukanaY = 0;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 4:
				if (hak.lokacja.y > szukanaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y--;
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y++;
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 5;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 5:
				if (hak.lokacja.x > docelowaX) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.x--;
					hak.lokacja.x--;
				}
				else if (hak.lokacja.x < docelowaX) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.x++;
					hak.lokacja.x++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 6;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 6:
				if (hak.lokacja.y > docelowaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y--;
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < docelowaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y++;
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					docelowaY--;
					szukanaY = 0;
					trybAnim = 7;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 7:
				if (hak.lokacja.y > szukanaY) {
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					if (szukanyTyp != 3) {
						szukanaY = 0;
						trybAnim = 1;
						szukanyTyp++;
						SetTimer(hWnd, TMR_1, czas, 0);
					}
					else
					{
						szukanyTyp = 1;
						czyMoznaDzialac = true;
					}
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 8:
				if (hak.lokacja.x > szukanaX) {
					hak.lokacja.x--;
				}
				else if (hak.lokacja.x < szukanaX) {
					hak.lokacja.x++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 9;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 9:
				if (hak.lokacja.y > szukanaY) {
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 10;
					szukanaY = 0;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 10:
				if (hak.lokacja.y > szukanaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y--;
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y++;
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 11;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 11:
				if (hak.lokacja.x > tymczasowaX) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.x--;
					hak.lokacja.x--;
				}
				else if (hak.lokacja.x < tymczasowaX) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.x++;
					hak.lokacja.x++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					trybAnim = 12;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 12:
				if (hak.lokacja.y > tymczasowaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y--;
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < tymczasowaY) {
					obiekty[lokacje[hak.lokacja.x][hak.lokacja.y]->idObiektu]->lokacja.y++;
					hak.lokacja.y++;
				}
				else
				{
					KillTimer(hWnd, TMR_1);
					tymczasowaY--;
					szukanaY = 0;
					trybAnim = 13;
					SetTimer(hWnd, TMR_1, czas, 0);
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			case 13:
				if (hak.lokacja.y > szukanaY) {
					hak.lokacja.y--;
				}
				else if (hak.lokacja.y < szukanaY) {
					hak.lokacja.y++;
				}
				else
				{
					liczbaPowtorzen--;
					KillTimer(hWnd, TMR_1);
					if (liczbaPowtorzen == 0) {
						szukanaY = 0;
						trybAnim = 1;
						for (int i = lokXMax; i >= 0; i--) {
							if ((!lokacje[i][lokYMax]->czyZajeta) && (docelowaX != i)) {
								tymczasowaX = i;
							}
						}
						SetTimer(hWnd, TMR_1, czas, 0);
					}
					else
					{
						bool czyJestSzukany = false;
						for (int i = 0; i < maxObiektow; i++) {
							if (obiekty[i]->typ == szukanyTyp) {
								szukanaY = obiekty[i]->lokacja.y;
								czyJestSzukany = true;
							}
						}	
						if (!czyJestSzukany) {
							szukanyTyp = 1;
							czyMoznaDzialac = true;
						}
						else
						{
							szukanaY -= liczbaPowtorzen;
							trybAnim = 8;
							SetTimer(hWnd, TMR_1, czas, 0);
						}	
					}
				}
				Obliczenia();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				break;
			}
			break;
#pragma endregion

#pragma region timer2
			case TMR_2:
				switch (szukanyTyp)
				{
				case 1:
					if (liczbaPowtorzen < rozmiarObiektu) {
						if (hak.czyZaczepiony) {
							obiekty[hak.idObiektu]->pozycja.y-=multipliAnim;
							hak.pozycja.y-=multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
						else
						{
							hak.pozycja.y-= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
					}
					else
					{
						liczbaPowtorzen = 0;
						KillTimer(hWnd, TMR_2);
						Obliczenia();
						repaintWindow(hWnd, hdc, ps, &drawArea1);
						czyMoznaDzialac = true;
					}
					break;
				case 2:
					if (liczbaPowtorzen < rozmiarObiektu) {
						if (hak.czyZaczepiony) {
							obiekty[hak.idObiektu]->pozycja.y+= multipliAnim;
							hak.pozycja.y+= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
						else
						{
							hak.pozycja.y+= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
					}
					else
					{
						liczbaPowtorzen = 0;
						KillTimer(hWnd, TMR_2);
						Obliczenia();
						repaintWindow(hWnd, hdc, ps, &drawArea1);
						czyMoznaDzialac = true;
					}
					break;
				case 3:
					if (liczbaPowtorzen < rozmiarObiektu) {
						if (hak.czyZaczepiony) {
							obiekty[hak.idObiektu]->pozycja.x-= multipliAnim;
							hak.pozycja.x-= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
						else
						{
							hak.pozycja.x-= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
					}
					else
					{
						liczbaPowtorzen = 0;
						KillTimer(hWnd, TMR_2);
						Obliczenia();
						repaintWindow(hWnd, hdc, ps, &drawArea1);
						czyMoznaDzialac = true;
					}
					break;
				case 4:
					if (liczbaPowtorzen < rozmiarObiektu) {
						if (hak.czyZaczepiony) {
							obiekty[hak.idObiektu]->pozycja.x+= multipliAnim;
							hak.pozycja.x+= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
						else
						{
							hak.pozycja.x+= multipliAnim;
							repaintWindow(hWnd, hdc, ps, &drawArea1);
							liczbaPowtorzen+= multipliAnim;
						}
					}
					else
					{
						liczbaPowtorzen = 0;
						KillTimer(hWnd, TMR_2);
						Obliczenia();
						repaintWindow(hWnd, hdc, ps, &drawArea1);
						czyMoznaDzialac = true;
					}
					break;
				}
				break;
#pragma endregion
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
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
