#include <windows.h>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

//hwnd
HWND R1_value;
HWND R2_value;
HWND C1_value;
HWND C2_value;
HWND U_combo;
HWND Amp_value;
HWND Freq_value;
HWND Okno_button;

HWND Input_static;
HWND Input_text;

HWND hwnd;
HWND hwnd2;
HWND hwnd3;

HWND scale_button;

#define ID_R1 100
#define ID_R2 101
#define ID_C1 102
#define ID_C2 103
#define ID_Amp 104
#define ID_Freq 105
#define ID_Okno 106

double R1 = 1000;
double R2 = 1000;
double C1 = 0.1;
double C2 = 0.1;
int sygnal_typ = 0;
double sygnal_amp = 1;
double sygnal_freq = 1;
int granica = 1000;
int input_typ;


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcChild(HWND hwnd2, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcInput(HWND hwnd3, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcText( HWND hwnd3, UINT mesg, WPARAM wParam, LPARAM lParam );

WNDPROC g_OldWndProc;
SCROLLINFO si;

void CreateInputWindow(int typ);
double IsNumber(LPSTR text1);
LPSTR GetValue(double liczba, char typ);
double GetMaxAmp(double liczba);
int GetCoord(double liczba, double kratka);
void DrawGraph(double sygnal[], double max_amp, int numer);
void Rozdzielacz();

void DrawTest();
#define ID_test 200


HINSTANCE hInstance;

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance = hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
 /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	WNDCLASSEX wc2;
	memset(&wc2,0,sizeof(wc2));
	wc2.cbSize		 = sizeof(WNDCLASSEX);
	wc2.lpfnWndProc	 = WndProcChild; /* This is where we will send messages to */
	wc2.hInstance	 = hInstance;
	wc2.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc2.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc2.lpszClassName = "WindowClassChild";
	wc2.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc2.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */
	
	WNDCLASSEX wc3;
	memset(&wc3,0,sizeof(wc3));
	wc3.cbSize		 = sizeof(WNDCLASSEX);
	wc3.lpfnWndProc	 = WndProcInput; /* This is where we will send messages to */
	wc3.hInstance	 = hInstance;
	wc3.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc3.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc3.lpszClassName = "WindowClassInput";
	wc3.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc3.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	if(!RegisterClassEx(&wc2)) {
		MessageBox(NULL, "Window2 Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	if(!RegisterClassEx(&wc3)) {
		MessageBox(NULL, "Window2 Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		1000, /* width */
		600, /* height */
		NULL,NULL,hInstance,NULL);

			
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}


	return msg.wParam;
}


//-------Funkcje obslugi komunikatow--------

//-------Funkjca okna g³ównego-------------
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		//--------Ustawianie bitmapy jako t³o--------------
		case WM_PAINT: {
			HBITMAP hbmObraz;
			hbmObraz =( HBITMAP ) LoadImage( NULL, "obwod.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			HDC hdc = GetDC( hwnd );
			HDC hdcNowy = CreateCompatibleDC( hdc );
			SelectObject( hdcNowy, hbmObraz );
			ReleaseDC(hwnd, hdc);
			HBITMAP hbmOld =( HBITMAP ) SelectObject( hdcNowy, hbmObraz );
					
			BITMAP bmInfo;
			GetObject( hbmObraz, sizeof( bmInfo ), & bmInfo );		
			
			
			PAINTSTRUCT ps;
			BeginPaint ( hwnd, &ps );
            BitBlt( hdc, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, hdcNowy, 0, 0, SRCCOPY );
            EndPaint( hwnd, &ps );
			
			DeleteDC(hdcNowy);
			break;
		}
		
		//---------Rysowanie kontrolek-------------
		case WM_CREATE: {
			//-----------------R1---------------------	
			R1_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				263, /* x */
				110, /* y */
				121, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_R1, hInstance, NULL );	
			SetWindowText( R1_value, GetValue(R1, 'R'));
							
			//----------------R2--------------------	
			R2_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				633, /* x */
				110, /* y */
				121, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_R2, hInstance, NULL );		
			SetWindowText( R2_value, GetValue(R2, 'R'));
				
			//------------------C1-----------------------	
			C1_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				345, /* x */
				230, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_C1, hInstance, NULL );			
			SetWindowText( C1_value, GetValue(C1, 'C'));
				
			//-----------------C2---------------------------	
			C2_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				693, /* x */
				230, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_C2, hInstance, NULL );	
			SetWindowText( C2_value, GetValue(C2, 'C'));
				
			//--------------------U--------------------
			U_combo = CreateWindowEx( WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
				10, /* x */
				290, /* y */
				135, /* width */
				80, /* height */
				hwnd, ( HMENU ) NULL, hInstance, NULL );
				
			SendMessage( U_combo, CB_ADDSTRING, 0,( LPARAM ) "Skok" );
			SendMessage( U_combo, CB_ADDSTRING, 0,( LPARAM ) "Fala Prostok¹tna" );
			SendMessage( U_combo, CB_ADDSTRING, 0,( LPARAM ) "Sinusoida" );
			SendMessage(U_combo, CB_SETCURSEL, 0, (LPARAM) 0);
			
			//--------------Amp---------------------
			Amp_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				10, /* x */
				220, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_Amp, hInstance, NULL );
			SetWindowText( Amp_value, GetValue(sygnal_amp, 'A') );
			
			//--------------Freq------------------------
			Freq_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				10, /* x */
				255, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_Freq, hInstance, NULL );
			SetWindowText( Freq_value, (LPSTR) "---" );
			
			//------------Przebiegi------------------
			Okno_button = CreateWindowEx(0, "BUTTON", "Rysuj przebiegi sygna³ów", WS_CHILD | WS_VISIBLE,
				50,
				450,
				200,
				50,
				hwnd, ( HMENU ) ID_Okno, hInstance, NULL );
			break;
		}
		
		//------------Obsluga kontrolek-----------------
		case WM_COMMAND:
		{
			if(HIWORD(wParam) == CBN_SELCHANGE)
            // If the user makes a selection from the list:
            //   Send CB_GETCURSEL message to get the index of the selected list item.
            { 
                int ItemIndex = SendMessage(U_combo, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                switch (ItemIndex)
				{
					case 0:
					{
						sygnal_typ = 0;
						SetWindowText( Freq_value, (LPSTR) "---" );
						break;
					}
					case 1:
					{
						sygnal_typ = 1;
						SetWindowText( Freq_value, GetValue(sygnal_freq, 'f') );
						break;
					}
					case 2:
					{
						sygnal_typ = 2;
						SetWindowText( Freq_value, GetValue(sygnal_freq, 'f') );
						break;
					}
						
				}              
          	}
          	
			switch (wParam)
			{		
				case ID_R1: {	 
					CreateInputWindow(1);
				}	
				break;
				
				case ID_R2:	{	    
					CreateInputWindow(2);
				}	
				break;
				
				case ID_C1: {	    
					CreateInputWindow(3);
				}	
				break;
				
				case ID_C2: {	    
					CreateInputWindow(4);
				}	
				break;
				
				case ID_Amp: {
					CreateInputWindow(5);
				}
				break;
				
				case ID_Freq: {
					if(sygnal_typ != 0)
						CreateInputWindow(6);
				}				
			    break;
			    
			    case ID_Okno: {
					DestroyWindow(hwnd2);
					hwnd2 = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClassChild","Caption",WS_POPUP|WS_OVERLAPPEDWINDOW|WS_VSCROLL,
						CW_USEDEFAULT, /* x */
						CW_USEDEFAULT, /* y */
						1200, /* width */
						1050, /* height */
						hwnd,NULL,hInstance,NULL);
					if(hwnd2 == NULL) {
						MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
						return 0;
					}
					
					RECT okno;
					GetClientRect( hwnd2, & okno );
					ZeroMemory( & si, sizeof( si ) );
					si.cbSize = sizeof( SCROLLINFO );
					si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
					si.nMin = 0;
					si.nMax = 1050;
					si.nPage = okno.bottom;
					si.nPos = 0;
					SetScrollInfo( hwnd2, SB_VERT, & si, TRUE );
					
					ShowWindow(hwnd2, SW_SHOW);
    				UpdateWindow( hwnd2 );
				}
				break;
			}

		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

//-------Funkcja okna do rysowania przebiegow------------------
LRESULT CALLBACK WndProcChild(HWND hwnd2, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {	
			ShowWindow(hwnd2, SW_HIDE);
			break;
		}
		
		//-----------------Ustawianie t³a-----------------------
		case WM_PAINT: {
			HBITMAP hbmObraz1;
			hbmObraz1 =( HBITMAP ) LoadImage( NULL, "wykres_x1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			HDC hdc1 = GetDC( hwnd2 );
			HDC hdcNowy1 = CreateCompatibleDC( hdc1 );
			SelectObject( hdcNowy1, hbmObraz1 );
			HBITMAP hbmOld =( HBITMAP ) SelectObject( hdcNowy1, hbmObraz1 );
					
			HBITMAP hbmObraz2;
			hbmObraz2 =( HBITMAP ) LoadImage( NULL, "wykres_x2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			HDC hdc2 = GetDC( hwnd2 );
			HDC hdcNowy2 = CreateCompatibleDC( hdc2 );
			SelectObject( hdcNowy2, hbmObraz2 );
			HBITMAP hbmOld2 =( HBITMAP ) SelectObject( hdcNowy2, hbmObraz2 );
					
			BITMAP bmInfo1;
			GetObject( hbmObraz1, sizeof( bmInfo1 ), & bmInfo1 );	
			
			BITMAP bmInfo2;
			GetObject( hbmObraz2, sizeof( bmInfo2 ), & bmInfo2 );		
			
			
			PAINTSTRUCT ps;
			BeginPaint ( hwnd2, &ps );
            BitBlt( hdc1, 100, 0, bmInfo1.bmWidth, bmInfo1.bmHeight, hdcNowy1, 0, 0, SRCCOPY );
            BitBlt( hdc2, 100, 505, bmInfo2.bmWidth, bmInfo2.bmHeight, hdcNowy2, 0, 0, SRCCOPY );
            EndPaint( hwnd2, &ps );
			
			Rozdzielacz();
			
			DeleteDC(hdcNowy1);
			DeleteDC(hdcNowy2);
			break;
		}
		
		case WM_CREATE: {					
			scale_button = CreateWindowEx( 0, "BUTTON", "Rysuj", WS_CHILD | WS_VISIBLE,
				10, /* x */
				100, /* y */
				80, /* width */
				20, /* height */
				hwnd2, ( HMENU ) ID_test, hInstance, NULL );
			break;
		}
	
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case ID_test:{
					DrawTest();
					break;
				}
			}
		}
		
		//----------Obsluga suwaka-------------
		case WM_VSCROLL: {
			SCROLLINFO si;
		    ZeroMemory( & si, sizeof( si ) );
		    si.cbSize = sizeof( SCROLLINFO );
		    si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
		    GetScrollInfo( hwnd2, SB_VERT, & si );
		    
		    int pozycja = si.nPos;
		    switch( LOWORD( wParam ) ) {
			    case SB_TOP:
			        pozycja = 0;
			        break;
			    case SB_BOTTOM:
			        pozycja = 1050;
			        break;
			    case SB_LINEUP:
			        if( pozycja > 0 ) {
			            pozycja--;
			        }
			        break;
			    case SB_LINEDOWN:
			        if( pozycja < 1050 ) {
			            pozycja++;
			        }
			        break;
			    case SB_PAGEUP:
			        pozycja -= si.nPage;
			        if( pozycja < 0 ) {
			            pozycja = 0;
			        }
			        break;
			    case SB_PAGEDOWN:
			        pozycja += si.nPage;
			        if( pozycja > 1050 ) {
			            pozycja = 1050;
			        }
			        break;
			    case SB_THUMBPOSITION:
			        pozycja = si.nTrackPos;
			        break;
			    case SB_THUMBTRACK:
			        pozycja = si.nTrackPos;
			        break;
			    }
			    
			    int dy = -( pozycja - si.nPos );
			    ZeroMemory( & si, sizeof( si ) );
			    si.cbSize = sizeof( SCROLLINFO );
			    si.fMask = SIF_POS;
			    si.nPos = pozycja;
			    
			    ScrollWindowEx( hwnd2, 0, dy,( CONST RECT * ) NULL,( CONST RECT * ) NULL,( HRGN ) NULL,( LPRECT ) NULL, SW_SCROLLCHILDREN );
				UpdateWindow( hwnd );
			    SetScrollInfo( hwnd2, SB_VERT, & si, TRUE );
		}
		break;	
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd2, Message, wParam, lParam);
	}
	return 0;
}

//----------Funkcja okna do wprowadzania danych--------------
LRESULT CALLBACK WndProcInput(HWND hwnd3, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {	
			ShowWindow(hwnd3, SW_HIDE);
			break;
		}
		
		//---Wybor treœci kontrolek w zale¿noœci od naciœniêtego przycisku w oknie g³ównym---
		case WM_CREATE: {
			switch(input_typ)
			{
				case 1:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ wartoœæ rezystancji R1", WS_CHILD | WS_VISIBLE | SS_CENTER,
					50, /* x */
					20, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					Input_text = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
					50, /* x */
					60, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(R1, '-') );
					break;
				}
				case 2:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ wartoœæ rezystancji R2", WS_CHILD | WS_VISIBLE | SS_CENTER,
					50, /* x */
					20, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					
					
					Input_text = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
					50, /* x */
					60, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(R2, '-') );
					
					break;
				}
				case 3:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ wartoœæ pojemnoœci C1", WS_CHILD | WS_VISIBLE | SS_CENTER,
					50, /* x */
					20, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					
					Input_text = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
					50, /* x */
					60, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(C1, '-') );
					break;
				}
				case 4:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ wartoœæ pojemnoœci C2", WS_CHILD | WS_VISIBLE | SS_CENTER,
					50, /* x */
					20, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					
					Input_text = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
					50, /* x */
					60, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(C2, '-') );
					break;
				}
				case 5:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ wartoœæ amplitudy sygna³u", WS_CHILD | WS_VISIBLE | SS_CENTER,
					50, /* x */
					20, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					
					Input_text = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
					50, /* x */
					60, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(sygnal_amp, '-') );
					break;
				}
				case 6:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ wartoœæ czêstotliwoœci sygna³u", WS_CHILD | WS_VISIBLE | SS_CENTER,
					50, /* x */
					20, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					
					Input_text = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
					50, /* x */
					60, /* y */
					300, /* width */
					20, /* height */
					hwnd3, ( HMENU ) NULL, hInstance, NULL );
					g_OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(sygnal_freq, '-') );
					break;
				}

			}				

			break;
		}
	
		//---------Obsluga klawiatury(enter i escape)------------
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_RETURN: {
					DWORD dlugosc = GetWindowTextLength( Input_text );
					LPSTR Bufor =( LPSTR ) GlobalAlloc( GPTR, dlugosc + 1 );
					GetWindowText( Input_text, Bufor, dlugosc + 1 );
					
					if(IsNumber(Bufor) != -1)
					{
						switch(input_typ)
						{
							case 1: {
								R1 = IsNumber(Bufor);
								SetWindowText( R1_value, GetValue(R1, 'R'));
								break;
							}
							case 2: {
								R2 = IsNumber(Bufor);
								SetWindowText( R2_value, GetValue(R2, 'R'));
								break;
							}
							case 3: {
								C1 = IsNumber(Bufor);
								SetWindowText( C1_value, GetValue(C1, 'C'));
								break;
							}
							case 4: {
								C2 = IsNumber(Bufor);
								SetWindowText( C2_value, GetValue(C2, 'C'));
								break;
							}
							case 5: {
								sygnal_amp = IsNumber(Bufor);
								SetWindowText( Amp_value, GetValue(sygnal_amp, 'A'));
								break;
							}
							case 6: {
								sygnal_freq = IsNumber(Bufor);
								SetWindowText( Freq_value, GetValue(sygnal_freq, 'f'));
								break;
							}
						}
						GlobalFree( Bufor );
						DestroyWindow(hwnd3);
					}
					else
						MessageBox( hwnd, "Podaj liczbê", NULL, MB_ICONINFORMATION );
					switch(input_typ)
					{
						case 1:
							SetWindowText(Input_text, GetValue(R1, '-'));
							break;
						case 2:
							SetWindowText(Input_text, GetValue(R2, '-'));
							break;
						case 3:
							SetWindowText(Input_text, GetValue(C1, '-'));
							break;
						case 4:
							SetWindowText(Input_text, GetValue(C2, '-'));
							break;
						case 5:
							SetWindowText(Input_text, GetValue(sygnal_amp, '-'));
							break;
						case 6:
							SetWindowText(Input_text, GetValue(sygnal_freq, '-'));
							break;
					}  
					break;
				}
				case VK_ESCAPE: {
					DestroyWindow(hwnd3);
					break;
				}
			}
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd3, Message, wParam, lParam);
	}
	return 0;
}

//Funkcja podklasy okna typu text, uzywana w WndProcInput(hwnd3)
LRESULT CALLBACK WndProcText( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    switch( Message )
    {
	//Przeslanie komunikatow o wcisnieciu przyciskow enter lub escape do okna hwnd3
    case WM_KEYDOWN: {
    	switch(( int ) wParam )
	    {
	    case VK_RETURN:
	        CallWindowProc( WndProcInput, hwnd3, Message, wParam, lParam );
	        break;
	        
	    case VK_ESCAPE:
	        CallWindowProc( WndProcInput, hwnd3, Message, wParam, lParam );
	        break;
	    }
    }
    default:
    	return CallWindowProc( g_OldWndProc, hwnd, Message, wParam, lParam );
    }
    return 0;
    
}
//---------Funkcje----------------

//Funkcja tworzenia okna WndProcInput
void CreateInputWindow(int typ)
{
	input_typ = typ;
	
	DestroyWindow(hwnd3);
	hwnd3 = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClassInput","Caption",WS_POPUP|WS_OVERLAPPEDWINDOW,
	500, /* x */
	400, /* y */
	400, /* width */
	200, /* height */
	hwnd,NULL,hInstance,NULL);
	if(hwnd3 == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		exit(0);
	}
					
	ShowWindow(hwnd3, SW_SHOW);
   	UpdateWindow( hwnd3 );
}

//Funkcja sprawdzajaca, czy tekst wczytany z pola typu EDIT jest liczba double
//Dopuszcza sie jedna kropke, spacje na koncu i uzywanie przedrostkow (p, n, u, m, k, M, G, T)
double IsNumber(LPSTR text1) 
{
	string text = text1;
	bool kropka = false; 
    for(int i = 0; i < text.length() - 1; i++) 
    {
		if(text[i]>'9' || text[i]<'0')
			{
				if(text[text.length() -2] == ' ')
					break;
					
				if(kropka)
					return -1;
				if(!kropka && text[i] == '.')
					kropka = true;
				else
					return -1;
			}
	}
	if(text[text.length() - 1] <= '9' && text[text.length() - 1] >= '0')
		return atof(text1);
	else if(text[text.length() - 1] == '.' || text[text.length() - 1] == ' ')
		return atof(text1);
	else if(text[text.length() - 1] == 'k')
		return atof(text1)*1e3;
	else if(text[text.length() - 1] == 'M')
		return atof(text1)*1e6;
	else if(text[text.length() - 1] == 'G')
		return atof(text1)*1e9;
	else if(text[text.length() - 1] == 'T')
		return atof(text1)*1e12;
	else if(text[text.length() - 1] == 'm')
		return atof(text1)*1e-3;
	else if(text[text.length() - 1] == 'u')
		return atof(text1)*1e-6;
	else if(text[text.length() - 1] == 'n')
		return atof(text1)*1e-9;
	else if(text[text.length() - 1] == 'p')
		return atof(text1)*1e-12;
	else
		return -1;
}

//Funkcja zamieniajaca liczbe double na typ LPSTR, ktory mozna wyswietlac w kontrolkach
//Liczba wyœwietlana jest z zakresu <1;999> z dodatkowym przedrostkiem, jesli jest potrzebny
//Obsluguje liczby z zakresy <1e-12;1e15>
LPSTR GetValue(double liczba, char typ)
{
	string napis;
	ostringstream strs;
	bool minus = false;
	if(abs(liczba) != liczba)
	minus = true;
	liczba = abs(liczba);
	
	if(liczba < 1e-12)
		liczba = 1e-12;
	else if(liczba > 1e15)
		liczba = 1e15;
	
	if(liczba >= 1e-12 && liczba < 1e-9)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e12;
		if(typ != '-')
			napis = strs.str() + " p";
		else
			napis = strs.str() + "p";
	}
	if(liczba >= 1e-9 && liczba < 1e-6)
	{
		if(minus)
			liczba = -1*liczba;
			strs << liczba*1e9;
	if(typ != '-')
			napis = strs.str() + " n";
		else
			napis = strs.str() + "n";
	}
	if(liczba >= 1e-6 && liczba < 1e-3)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e6;
		if(typ != '-')
			napis = strs.str() + " u";
		else
			napis = strs.str() + "u";
	}
	if(liczba >= 1e-3 && liczba < 1)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e3;
		if(typ != '-')
			napis = strs.str() + " m";
		else
			napis = strs.str() + "m";
	}
	if(liczba >= 1 && liczba < 1e3)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba;
		if(typ != '-')
			napis = strs.str() + " ";
		else
			napis = strs.str();
	}
	if(liczba >= 1e3 && liczba < 1e6)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-3;
		if(typ != '-')
			napis = strs.str() + " k";
		else
		napis = strs.str() + "k";
	}
	if(liczba >= 1e6 && liczba < 1e9)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-6;
		if(typ != '-')
			napis = strs.str() + " M";
		else
		napis = strs.str() + "M";
	}
	if(liczba >= 1e9 && liczba < 1e12)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-9;
		if(typ != '-')
			napis = strs.str() + " G";
		else
		napis = strs.str() + "G";
	}
	if(liczba >= 1e12 && liczba <= 1e15)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-12;
		if(typ != '-')
			napis = strs.str() + " T";
		else
		napis = strs.str() + "T";
	}
	
	switch(typ)
	{
		case 'R':
		{
			napis += "Ohm";
			break;
		}
		case 'C':
		{
			napis += "F";
			break;
		}
		case 'A':
		{
			napis += "V";
			break;	
		}
		case 'f':
		{
			napis += "Hz";
			break;
		}
		case '-':
			break;
	}
	
	LPSTR wynik = &napis[0];
	return wynik;		
}

//Funkcja uzywana do poprawy wizualizacji wartosci maksymalnej amplitudy
double GetMaxAmp(double liczba)
{
	if(liczba >= 1e-12 && liczba < 1e-9)
	{
		liczba *= 1e12;
		liczba = ceil(liczba);
		liczba /= 1e12;
	}
	if(liczba >= 1e-9 && liczba < 1e-6)
	{
		liczba *= 1e9;
		liczba = ceil(liczba);
		liczba /= 1e9;
	}
	if(liczba >= 1e-6 && liczba < 1e-3)
	{
		liczba *= 1e6;
		liczba = ceil(liczba);
		liczba /= 1e6;
	}
	if(liczba >= 1e-3 && liczba < 1)
	{
		liczba *= 1e3;
		liczba = ceil(liczba);
		liczba /= 1e3;
	}
	if(liczba >= 1 && liczba < 1e3)
	{
		liczba = ceil(liczba);
	}
	if(liczba >= 1e3 && liczba < 1e6)
	{
		liczba *= 1e-3;
		liczba = ceil(liczba);
		liczba /= 1e-3;
	}
	if(liczba >= 1e6 && liczba < 1e9)
	{
		liczba *= 1e-6;
		liczba = ceil(liczba);
		liczba /= 1e-6;
	}
	if(liczba >= 1e9 && liczba < 1e12)
	{
		liczba *= 1e-9;
		liczba = ceil(liczba);
		liczba /= 1e-9;
	}
	if(liczba >= 1e12 && liczba < 1e15)
	{
		liczba *= 1e-12;
		liczba = ceil(liczba);
		liczba /= 1e-12;
	}
	return liczba;
}

//Fukncja zwracajaca pixel, do ktorego rysowany bedzie wykres
int GetCoord(double liczba, double kratka)
{
	int Coord = round(liczba/kratka);
	return Coord;
}

//Do wyjebania potem
void DrawTest()
{
	double tablica[granica];
	double max_amp = 0;
	
	for(int i = 0; i < granica; i++)
	{
		tablica[i] = -i*i*i;
		if(abs(tablica[i]) > max_amp)
			max_amp = abs(tablica[i]);
	}
	max_amp = GetMaxAmp(max_amp);
	DrawGraph(tablica, max_amp, 1);
	
	max_amp = 0;
	bool a = false;
	int x = 0;
	for(int i = 0; i < granica; i++)
	{
		tablica[i] = i;
		if (abs(tablica[i]) > max_amp)
			max_amp = abs(tablica[i]);
	}
	DrawGraph(tablica, max_amp, 2);
}

//Funkcja rysujaca kreske rozdzilajaca w WndProcChild
void Rozdzielacz()
{
	HDC hdcOkno = GetDC(hwnd2);
	POINT stary_punkt;
	HPEN Pioro = CreatePen( PS_SOLID, 3, 0xFFFFFF );
	MoveToEx(hdcOkno, 0, 502, &stary_punkt);
	LineTo(hdcOkno, 1201, 502);
	DeleteObject(Pioro);
	ReleaseDC(hwnd2, hdcOkno);
}

//Funkcja rysujaca wykres w odpowiednim miejscu w WndProcChild
void DrawGraph(double sygnal[], double max_amp, int numer)
{
	HDC hdcOkno = GetDC(hwnd2);
	HPEN NiebieskiePioro = CreatePen( PS_SOLID, 1, 0xFF0000 );
	POINT stary_punkt;
	double kratka_x = max_amp/250;
	double kratka_y = granica/1000;
	int start_coord_x = 100;
	int start_coord_y;
	
	switch(numer)
	{
		case 1:{
			start_coord_y = 251;
			break;
		}
		case 2:{
			start_coord_y = 756;
			break;
		}
	}
	
	MoveToEx(hdcOkno, start_coord_x, start_coord_y, &stary_punkt);
	SelectObject(hdcOkno, NiebieskiePioro);
	for(int i = 0; i < granica; i++)
	{
		int coord_y = GetCoord(sygnal[i], kratka_x);
		LineTo(hdcOkno, start_coord_x + i + 1, start_coord_y - coord_y);
	}
	
	DeleteObject(NiebieskiePioro);
	ReleaseDC(hwnd2, hdcOkno);
}

