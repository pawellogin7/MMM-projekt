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
HWND Przebiegi_button;
//hwnd2
HWND max_amp_x1;
HWND max_amp_x2;
HWND time_x1;
HWND time_x2;
HWND simulation_time;
HWND simulation_time_static;

HWND x1_m1_static;
HWND x1_m1_A_static;
HWND x1_m1_t_static;
HWND x1_amp_marker1;
HWND x1_czas_marker1;

HWND x1_m2_static;
HWND x1_m2_A_static;
HWND x1_m2_t_static;
HWND x1_amp_marker2;
HWND x1_czas_marker2;

HWND x1_d_static;
HWND x1_d_A_static;
HWND x1_d_t_static;
HWND x1_amp_delta;
HWND x1_czas_delta;

HWND x2_m1_static;
HWND x2_m1_A_static;
HWND x2_m1_t_static;
HWND x2_amp_marker1;
HWND x2_czas_marker1;

HWND x2_m2_static;
HWND x2_m2_A_static;
HWND x2_m2_t_static;
HWND x2_amp_marker2;
HWND x2_czas_marker2;

HWND x2_d_static;
HWND x2_d_A_static;
HWND x2_d_t_static;
HWND x2_amp_delta;
HWND x2_czas_delta;
//hwnd3
HWND Input_static;
HWND Input_text;

HWND hwnd;
HWND hwnd2;
HWND hwnd3;

#define ID_R1 100
#define ID_R2 101
#define ID_C1 102
#define ID_C2 103
#define ID_Amp 104
#define ID_Freq 105
#define ID_Przebiegi 106
#define ID_czas 107

double R1 = 1000;
double R2 = 1000;
double C1 = 0.1;
double C2 = 0.1;
int sygnal_typ = 0;
double sygnal_amp = 1;
double sygnal_freq = 1;
double czas_symulacji = 1; //w sekundach
double MaxAmp_x1 = 0;
double MaxAmp_x2 = 0;
int input_typ;

bool m1_x1 = false;
bool m2_x1 = false;
bool m1_x2 = false;
bool m2_x2 = false;
int m1_x1_x = 0;
int m1_x1_y = 0;
int m2_x1_x = 0;
int m2_x1_y = 0;
int m1_x2_x = 0;
int m1_x2_y = 0;
int m2_x2_x = 0;
int m2_x2_y = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcChild(HWND hwnd2, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcInput(HWND hwnd3, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcText( HWND hwnd3, UINT mesg, WPARAM wParam, LPARAM lParam );

WNDPROC OldWndProc;
SCROLLINFO si;
HINSTANCE hInstance;

void CreateInputWindow(int typ);
void ZmienDelte();
void DrawMarkers();
void ZerujMarkery();
bool IsNumber(LPSTR text1);
double GetNumber(LPSTR text1);
double GetNumberTime(LPSTR text1);
double GetNumberAmp(LPSTR text1);
LPSTR usunKoniec(LPSTR text1);
LPSTR GetValue(double liczba, char typ, bool zero_acceptable);
double GetMaxAmp(double liczba);
LPSTR GetAmpValue(double liczba);
int GetCoord(double liczba, double kratka);
void GraphCalculations();
void DrawGraph(double sygnal[], double max_amp, int numer);
void Rozdzielacz(int szerokosc, int y, int kolor);
double FalaProstokatna(int liczba);



//-----------------------------------------WinMain-------------------------------------------------------

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
	wc.hIcon		 = LoadIcon(hInstance, MAKEINTRESOURCE(1000));
	wc.hIconSm		 = LoadIcon(hInstance, MAKEINTRESOURCE(1000));


	WNDCLASSEX wc2;
	memset(&wc2,0,sizeof(wc2));
	wc2.cbSize		 = sizeof(WNDCLASSEX);
	wc2.lpfnWndProc	 = WndProcChild; /* This is where we will send messages to */
	wc2.hInstance	 = hInstance;
	wc2.hCursor		 = LoadCursor(NULL, IDC_CROSS);
	wc2.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc2.lpszClassName = "WindowClassChild";
	wc2.hIcon		 = LoadIcon(hInstance, MAKEINTRESOURCE(1001));
	wc2.hIconSm		 = LoadIcon(hInstance, MAKEINTRESOURCE(1001));
	
	WNDCLASSEX wc3;
	memset(&wc3,0,sizeof(wc3));
	wc3.cbSize		 = sizeof(WNDCLASSEX);
	wc3.lpfnWndProc	 = WndProcInput; /* This is where we will send messages to */
	wc3.hInstance	 = hInstance;
	wc3.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc3.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc3.lpszClassName = "WindowClassInput";
	wc3.hIcon		 = LoadIcon(hInstance, MAKEINTRESOURCE(1002));
	wc3.hIconSm		 = LoadIcon(hInstance, MAKEINTRESOURCE(1002));
	

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	if(!RegisterClassEx(&wc2)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	if(!RegisterClassEx(&wc3)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","MMM Projekt - Przesuwnik Fazowy",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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



//----------------------------Funkcje obslugi komunikatow------------------------------

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
			hbmObraz =( HBITMAP ) LoadImage( NULL, "pictures/obwod.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
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
				273, /* x */
				110, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_R1, hInstance, NULL );	
			SetWindowText( R1_value, GetValue(R1, 'R', 0));
							
			//----------------R2--------------------	
			R2_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				643, /* x */
				110, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_R2, hInstance, NULL );		
			SetWindowText( R2_value, GetValue(R2, 'R', 0));
				
			//------------------C1-----------------------	
			C1_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				345, /* x */
				230, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_C1, hInstance, NULL );			
			SetWindowText( C1_value, GetValue(C1, 'C', 0));
				
			//-----------------C2---------------------------	
			C2_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				693, /* x */
				230, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_C2, hInstance, NULL );	
			SetWindowText( C2_value, GetValue(C2, 'C', 0));
				
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
			SetWindowText( Amp_value, GetValue(sygnal_amp, 'A', 0) );
			
			//--------------Freq------------------------
			Freq_value = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				10, /* x */
				255, /* y */
				100, /* width */
				20, /* height */
				hwnd, ( HMENU ) ID_Freq, hInstance, NULL );
			SetWindowText( Freq_value, (LPSTR) "---" );
			
			//------------Przebiegi------------------
			Przebiegi_button = CreateWindowEx(0, "BUTTON", "Rysuj przebiegi sygna³ów", WS_CHILD | WS_VISIBLE,
				70,
				450,
				200,
				50,
				hwnd, ( HMENU ) ID_Przebiegi, hInstance, NULL );
			break;
		}
		
		//------------Obsluga kontrolek-----------------
		case WM_COMMAND:
		{
			if(HIWORD(wParam) == CBN_SELCHANGE)
            // If the user makes a selection from the list:
            //  Send CB_GETCURSEL message to get the index of the selected list item.
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
						SetWindowText( Freq_value, GetValue(sygnal_freq, 'f', 0) );
						break;
					}
					case 2:
					{
						sygnal_typ = 2;
						SetWindowText( Freq_value, GetValue(sygnal_freq, 'f', 0) );
						break;
					}
				}
				InvalidateRect(hwnd2, NULL, false);
				ZerujMarkery();
				UpdateWindow(hwnd2);        
          	}
          	
			switch (wParam)
			{		
				case ID_R1: {	 
					CreateInputWindow(1);
					break;
				}	
							
				case ID_R2:	{	    
					CreateInputWindow(2);
					break;
				}	
						
				case ID_C1: {	    
					CreateInputWindow(3);
					break;
				}	
								
				case ID_C2: {	    
					CreateInputWindow(4);
					break;
				}	
								
				case ID_Amp: {
					CreateInputWindow(5);
					break;
				}
								
				case ID_Freq: {
					if(sygnal_typ != 0)
						CreateInputWindow(6);
					break;
				}				
			    			    
			    case ID_Przebiegi: {
					DestroyWindow(hwnd2);
					hwnd2 = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClassChild","Przebiegi sygna³ów",WS_POPUP|WS_OVERLAPPEDWINDOW|WS_VSCROLL,
						CW_USEDEFAULT, /* x */
						CW_USEDEFAULT, /* y */
						1250, /* width */
						1070, /* height */
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
					si.nMax = 1070;
					si.nPage = okno.bottom;
					si.nPos = 0;
					SetScrollInfo( hwnd2, SB_VERT, & si, TRUE );
					
					ShowWindow(hwnd2, SW_SHOW);
    				UpdateWindow( hwnd2 );
    				break;
				}
			}
		}
		
		case WM_LBUTTONDOWN:
		{
			
						
		break;
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
			hbmObraz1 =( HBITMAP ) LoadImage( NULL, "pictures/wykres_x1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			HDC hdc1 = GetDC( hwnd2 );
			HDC hdcNowy1 = CreateCompatibleDC( hdc1 );
			SelectObject( hdcNowy1, hbmObraz1 );
			HBITMAP hbmOld =( HBITMAP ) SelectObject( hdcNowy1, hbmObraz1 );
					
			HBITMAP hbmObraz2;
			hbmObraz2 =( HBITMAP ) LoadImage( NULL, "pictures/wykres_x2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
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
            BitBlt( hdc1, 150, 15, bmInfo1.bmWidth, bmInfo1.bmHeight, hdcNowy1, 0, 0, SRCCOPY );
            BitBlt( hdc2, 150, 554, bmInfo2.bmWidth, bmInfo2.bmHeight, hdcNowy2, 0, 0, SRCCOPY );
            EndPaint( hwnd2, &ps );
			
			GraphCalculations();
			DrawMarkers();
			
			Rozdzielacz(8, 531, 0);
			Rozdzielacz(2, 534, 2);	
			DeleteDC(hdcNowy1);
			DeleteDC(hdcNowy2);
			break;
		}
		
		case WM_CREATE: {									
			max_amp_x1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
				15, /* x */
				8, /* y */
				120, /* width */
				21, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );	
				
			max_amp_x2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
				15, /* x */
				547, /* y */
				120, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			time_x1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				1110, /* x */
				272, /* y */
				81, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			time_x2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				1110, /* x */
				811, /* y */
				81, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			
			simulation_time_static = CreateWindowEx( 0, "STATIC", "Czas symulacji:" , WS_CHILD | WS_VISIBLE | SS_CENTER,
				15, /* x */
				35, /* y */
				120, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			
			simulation_time = CreateWindowEx( 0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
				25, /* x */
				55, /* y */
				100, /* width */
				20, /* height */
				hwnd2, ( HMENU ) ID_czas, hInstance, NULL );
			
			//----------x1 Marker1-------------------
			x1_m1_static = CreateWindowEx( 0, "STATIC", "MARKER 1", WS_CHILD | WS_VISIBLE | SS_LEFT,
				5, /* x */
				115, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_m1_A_static = CreateWindowEx( 0, "STATIC", "A:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				135, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_m1_t_static = CreateWindowEx( 0, "STATIC", "t:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				155, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			
			x1_amp_marker1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				135, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x1_amp_marker1, (LPSTR) "--------" );
				
			x1_czas_marker1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				155, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x1_czas_marker1, (LPSTR) "--------" );
			
			//----------x1 Marker2-------------------
			x1_m2_static = CreateWindowEx( 0, "STATIC", "MARKER 2", WS_CHILD | WS_VISIBLE | SS_LEFT,
				5, /* x */
				200, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_m2_A_static = CreateWindowEx( 0, "STATIC", "A:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				220, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_m2_t_static = CreateWindowEx( 0, "STATIC", "t:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				240, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			
			x1_amp_marker2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				220, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x1_amp_marker2, (LPSTR) "--------" );
				
			x1_czas_marker2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				240, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x1_czas_marker2, (LPSTR) "--------" );
			
			
			//---------------x1 delta-----------------------
			x1_d_static = CreateWindowEx( 0, "STATIC", "DELTA", WS_CHILD | WS_VISIBLE | SS_LEFT,
				5, /* x */
				330, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_d_A_static = CreateWindowEx( 0, "STATIC", "A:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				350, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_d_t_static = CreateWindowEx( 0, "STATIC", "t:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				370, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x1_amp_delta = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				350, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x1_amp_delta, (LPSTR) "--------" );
				
			x1_czas_delta = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				370, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x1_czas_delta, (LPSTR) "--------" );
			
			//----------x2 Marker1-------------------
			x2_m1_static = CreateWindowEx( 0, "STATIC", "MARKER 1", WS_CHILD | WS_VISIBLE | SS_LEFT,
				5, /* x */
				660, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_m1_A_static = CreateWindowEx( 0, "STATIC", "A:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				680, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_m1_t_static = CreateWindowEx( 0, "STATIC", "t:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				700, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			
			x2_amp_marker1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				680, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x2_amp_marker1, (LPSTR) "--------" );
				
			x2_czas_marker1 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				700, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x2_czas_marker1, (LPSTR) "--------" );
			
			//----------x2 Marker2-------------------
			x2_m2_static = CreateWindowEx( 0, "STATIC", "MARKER 2", WS_CHILD | WS_VISIBLE | SS_LEFT,
				5, /* x */
				745, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_m2_A_static = CreateWindowEx( 0, "STATIC", "A:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				765, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_m2_t_static = CreateWindowEx( 0, "STATIC", "t:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				785, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			
			x2_amp_marker2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				765, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x2_amp_marker2, (LPSTR) "--------" );
				
			x2_czas_marker2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				785, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x2_czas_marker2, (LPSTR) "--------" );
			
			
			//---------------x2 delta-----------------------
			x2_d_static = CreateWindowEx( 0, "STATIC", "DELTA", WS_CHILD | WS_VISIBLE | SS_LEFT,
				5, /* x */
				875, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_d_A_static = CreateWindowEx( 0, "STATIC", "A:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				895, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_d_t_static = CreateWindowEx( 0, "STATIC", "t:", WS_CHILD | WS_VISIBLE | SS_RIGHT,
				0, /* x */
				915, /* y */
				23, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
				
			x2_amp_delta = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				895, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x2_amp_delta, (LPSTR) "--------" );
				
			x2_czas_delta = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
				25, /* x */
				915, /* y */
				110, /* width */
				20, /* height */
				hwnd2, ( HMENU ) NULL, hInstance, NULL );
			SetWindowText( x2_czas_delta, (LPSTR) "--------" );
					
			break;
		}
		
		case WM_CTLCOLORSTATIC:{
			HBRUSH g_hBrush = CreateSolidBrush( RGB( 255, 255, 255 ) );
			HWND hCtl =( HWND ) lParam;
			HDC hDC =( HDC ) wParam;
			
   			if( hCtl == max_amp_x1 || hCtl == max_amp_x2 || hCtl == time_x1 || hCtl == time_x2 || hCtl == simulation_time_static ||
			    hCtl == x1_d_static || hCtl == x1_d_A_static || hCtl == x1_d_t_static || hCtl == x1_amp_delta || hCtl == x1_czas_delta ||
				hCtl == x2_d_static || hCtl == x2_d_A_static || hCtl == x2_d_t_static || hCtl == x2_amp_delta || hCtl == x2_czas_delta )
			{
				SetBkMode( hDC, TRANSPARENT );
				return( LRESULT ) g_hBrush;
			}
			else if( hCtl == x1_m1_static || hCtl == x1_m1_A_static || hCtl == x1_m1_t_static || hCtl == x1_amp_marker1 || hCtl == x1_czas_marker1 ||
				 hCtl == x2_m1_static || hCtl == x2_m1_A_static || hCtl == x2_m1_t_static || hCtl == x2_amp_marker1 || hCtl == x2_czas_marker1)
			{
				SetTextColor( hDC, RGB( 0, 255, 0 ) );
				return( LRESULT ) g_hBrush;       		
			}
			else if( hCtl == x1_m2_static || hCtl == x1_m2_A_static || hCtl == x1_m2_t_static || hCtl == x1_amp_marker2 || hCtl == x1_czas_marker2 || 
				hCtl == x2_m2_static || hCtl == x2_m2_A_static || hCtl == x2_m2_t_static || hCtl == x2_amp_marker2 || hCtl == x2_czas_marker2)
			{
				SetBkMode( hDC, TRANSPARENT );
        		SetTextColor( hDC, RGB( 255, 0, 0 ) );
				return( LRESULT ) g_hBrush;
			}
			break;
		}
	
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case ID_czas: {	 
					CreateInputWindow(7);
				}	
				break;
			}
		}
		
		case WM_KEYDOWN:{
			switch(wParam){
				case VK_RETURN: {
					DestroyWindow(hwnd2);
					break;
				}	
				case VK_ESCAPE: {
					DestroyWindow(hwnd2);
					break;
				}
			}
			break;
		}
		
		case WM_LBUTTONDOWN:
		{
		    POINT cur;
   			HRGN hRgn = CreateRectRgn( 150, 15, 1152, 516 );
   			HRGN hRgn2 = CreateRectRgn( 150, 554, 1152, 1055);
    		GetCursorPos( & cur );
    		ScreenToClient( hwnd2, & cur );    		
    		COLORREF color;
			HDC hDC;
			hDC = GetDC(hwnd2);
			
			bool pixel_wykres = false;
			int blad = 2;			
			for(int i = -blad; i <= blad; i++)
			{
				for(int j = -blad; j <= blad; j++)
				{
					color = GetPixel(hDC, cur.x + i, cur.y + j);
					if(color == 0xFF0000)
					{
						pixel_wykres = true;
						break;
					}
				}
			}			
			
			if( PtInRegion( hRgn, cur.x, cur.y) )
			{
	    		if (pixel_wykres)
	    		{
	   				SetWindowText( x1_amp_marker1, GetAmpValue( MaxAmp_x1 * (266 - cur.y) / 250 ) );
	   				SetWindowText( x1_czas_marker1, GetValue( (czas_symulacji * (cur.x - 151) / 1000), 't', 1) );
	   				m1_x1 = true;
	   				m1_x1_x = cur.x;
	   				m1_x1_y = cur.y;
				}
				else
				{
					SetWindowText( x1_amp_marker1, (LPSTR) "--------" );
	   				SetWindowText( x1_czas_marker1, (LPSTR) "--------" );
	   				m1_x1 = false;	   				
				}
				InvalidateRect(hwnd2, NULL, false);
				UpdateWindow(hwnd2);
			}
			else if( PtInRegion( hRgn2, cur.x, cur.y) )
			{
	    		if (pixel_wykres)
	    		{
	   				SetWindowText( x2_amp_marker1, GetAmpValue( MaxAmp_x2 * (805 - cur.y) / 250 ) );
	   				SetWindowText( x2_czas_marker1, GetValue( (czas_symulacji * (cur.x - 151) / 1000), 't', 1) );
	   				m1_x2 = true;
	   				m1_x2_x = cur.x;
	   				m1_x2_y = cur.y;
				}
				else
				{
					SetWindowText( x2_amp_marker1, (LPSTR) "--------" );
	   				SetWindowText( x2_czas_marker1, (LPSTR) "--------" );
	   				m1_x2 = false;
				}
				InvalidateRect(hwnd2, NULL, false);
				UpdateWindow(hwnd2);
			}
			else
				break;
				
		    break;
		}

		case WM_RBUTTONDOWN:
		{
		    POINT cur;
   			HRGN hRgn = CreateRectRgn( 150, 15, 1152, 516 );
   			HRGN hRgn2 = CreateRectRgn( 150, 554, 1152, 1055);
    		GetCursorPos( & cur );
    		ScreenToClient( hwnd2, & cur );  		
    		COLORREF color;
			HDC hDC;
			hDC = GetDC(hwnd2);
			
			bool pixel_wykres = false;
			int blad = 2;			
			for(int i = -blad; i <= blad; i++)
			{
				for(int j = -blad; j <= blad; j++)
				{
					color = GetPixel(hDC, cur.x + i, cur.y + j);
					if(color == 0xFF0000)
					{
						pixel_wykres = true;
						break;
					}
				}
			}			
			
			if( PtInRegion( hRgn, cur.x, cur.y) )
			{
	    		if (pixel_wykres)
	    		{
	   				SetWindowText( x1_amp_marker2, GetAmpValue( MaxAmp_x1 * (266 - cur.y) / 250) );
	   				SetWindowText( x1_czas_marker2, GetValue( (czas_symulacji * (cur.x - 151) / 1000), 't', 1) );
	   				m2_x1 = true;
	   				m2_x1_x = cur.x;
	   				m2_x1_y = cur.y;
				}
				else
				{
					SetWindowText( x1_amp_marker2, (LPSTR) "--------" );
	   				SetWindowText( x1_czas_marker2, (LPSTR) "--------" );
	   				m2_x1 = false;
				}
				InvalidateRect(hwnd2, NULL, false);
				UpdateWindow(hwnd2);
			}
			else if( PtInRegion( hRgn2, cur.x, cur.y) )
			{
	    		if (pixel_wykres)
	    		{
	   				SetWindowText( x2_amp_marker2, GetAmpValue( MaxAmp_x2 * (805 - cur.y) / 250) );
	   				SetWindowText( x2_czas_marker2, GetValue( (czas_symulacji * (cur.x - 151) / 1000), 't', 1) );
	   				m2_x2 = true;
	   				m2_x2_x = cur.x;
	   				m2_x2_y = cur.y;
				}
				else
				{
					SetWindowText( x2_amp_marker2, (LPSTR) "--------" );
	   				SetWindowText( x2_czas_marker2, (LPSTR) "--------" );
	   				m2_x2 = false;
				}
				InvalidateRect(hwnd2, NULL, false);
				UpdateWindow(hwnd2);
			}
			else
				break;
			
		    break;
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
			        pozycja = 1070;
			        break;
			    case SB_LINEUP:
			        if( pozycja > 0 ) {
			            pozycja--;
			        }
			        break;
			    case SB_LINEDOWN:
			        if( pozycja < 1070 ) {
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
			        if( pozycja > 1070 ) {
			            pozycja = 1070;
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
				UpdateWindow( hwnd2 );
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
		
		//---Wybor treci kontrolek w zale¿noci od naciniêtego przycisku w oknie g³ównym---
		case WM_CREATE: {
			switch(input_typ)
			{
				case 1:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ rezystancjê R1", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(R1, '-', 0) );
					break;
				}
				case 2:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ rezystancjê R2", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(R2, '-', 0) );					
					break;
				}
				case 3:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ pojemnoæ C1", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(C1, '-', 0) );
					break;
				}
				case 4:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ pojemnoæ C2", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(C2, '-', 0) );
					break;
				}
				case 5:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ 2amplitudê sygna³u", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(sygnal_amp, '-', 0) );
					break;
				}
				case 6:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ czêstotliwoæ sygna³u", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(sygnal_freq, '-', 0) );
					break;
				}
				case 7:{
					Input_static = CreateWindowEx( 0, "STATIC", "Zmieñ czas symulacji", WS_CHILD | WS_VISIBLE | SS_CENTER,
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
					OldWndProc = ( WNDPROC ) SetWindowLong( Input_text, GWL_WNDPROC,( LONG ) WndProcText );
					SetWindowText( Input_text, GetValue(czas_symulacji, '-', 0) );
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
					
					if(IsNumber(Bufor))
					{
						switch(input_typ)
						{
							case 1: {
								R1 = GetNumber(Bufor);
								SetWindowText( R1_value, GetValue(R1, 'R', 0));
								break;
							}
							case 2: {
								R2 = GetNumber(Bufor);
								SetWindowText( R2_value, GetValue(R2, 'R', 0));
								break;
							}
							case 3: {
								C1 = GetNumber(Bufor);
								SetWindowText( C1_value, GetValue(C1, 'C', 0));
								break;
							}
							case 4: {
								C2 = GetNumber(Bufor);
								SetWindowText( C2_value, GetValue(C2, 'C', 0));
								break;
							}
							case 5: {
								sygnal_amp = GetNumber(Bufor);
								SetWindowText( Amp_value, GetValue(sygnal_amp, 'A', 0));
								break;
							}
							case 6: {
								sygnal_freq = GetNumber(Bufor);
								SetWindowText( Freq_value, GetValue(sygnal_freq, 'f', 0));
								break;
							}
							case 7: {
								czas_symulacji = GetNumber(Bufor);
								SetWindowText( simulation_time, GetValue(czas_symulacji, 't', 0));
								break;
							}
						}
						InvalidateRect(hwnd2, NULL, false);
						UpdateWindow(hwnd2);
						ZerujMarkery();
						GlobalFree( Bufor );
						DestroyWindow(hwnd3);
					}
					else
						MessageBox( hwnd, "Podaj liczbê z zakresu <1e-12; 1e15>", NULL, MB_ICONINFORMATION );
					switch(input_typ)
					{
						case 1:
							SetWindowText(Input_text, GetValue(R1, '-', 0));
							break;
						case 2:
							SetWindowText(Input_text, GetValue(R2, '-', 0));
							break;
						case 3:
							SetWindowText(Input_text, GetValue(C1, '-', 0));
							break;
						case 4:
							SetWindowText(Input_text, GetValue(C2, '-', 0));
							break;
						case 5:
							SetWindowText(Input_text, GetValue(sygnal_amp, '-', 0));
							break;
						case 6:
							SetWindowText(Input_text, GetValue(sygnal_freq, '-', 0));
							break;
						case 7:
							SetWindowText(Input_text, GetValue(czas_symulacji, '-', 0));
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
    	return CallWindowProc( OldWndProc, hwnd, Message, wParam, lParam );
    }
    return 0;
    
}



//--------------------------Funkcje------------------------------

//Funkcja tworzenia okna WndProcInput
void CreateInputWindow(int typ)
{
	input_typ = typ;
	
	DestroyWindow(hwnd3);
	hwnd3 = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClassInput",NULL,WS_POPUP|WS_OVERLAPPEDWINDOW,
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

void ZmienDelte()
{	
	double M1, M2;
	DWORD dlugosc1 = GetWindowTextLength( x1_amp_marker1 );
	LPSTR Bufor1 =( LPSTR ) GlobalAlloc( GPTR, dlugosc1 + 1 );
	GetWindowText( x1_amp_marker1, Bufor1, dlugosc1 + 1 );
	DWORD dlugosc2 = GetWindowTextLength( x1_amp_marker2 );
	LPSTR Bufor2 =( LPSTR ) GlobalAlloc( GPTR, dlugosc2 + 1 );			
	GetWindowText( x1_amp_marker2, Bufor2, dlugosc2 + 1 );
	if(m1_x1 && m2_x1)
	{
		M1 = GetNumberAmp(Bufor1);
		M2 = GetNumberAmp(Bufor2);
		SetWindowText( x1_amp_delta, GetAmpValue( abs(M2 - M1) ) );
		GlobalFree(Bufor1);
		GlobalFree(Bufor2);
		
		dlugosc1 = GetWindowTextLength( x1_czas_marker1 );
		Bufor1 =( LPSTR ) GlobalAlloc( GPTR, dlugosc1 + 1 );
		GetWindowText( x1_czas_marker1, Bufor1, dlugosc1 + 1 );
		dlugosc2 = GetWindowTextLength( x1_czas_marker2 );
		Bufor2 =( LPSTR ) GlobalAlloc( GPTR, dlugosc2 + 1 );
		GetWindowText( x1_czas_marker2, Bufor2, dlugosc1 + 1 );
		M1 = GetNumberTime(Bufor1);
		M2 = GetNumberTime(Bufor2);
		GetWindowText( x1_czas_marker2, Bufor2, dlugosc2 + 1 );
		SetWindowText( x1_czas_delta, GetValue( abs(M2 - M1), 't', 1) );
		GlobalFree(Bufor1);
		GlobalFree(Bufor2);
	}
	else	
	{
		SetWindowText( x1_amp_delta, (LPSTR) "--------" );
		SetWindowText( x1_czas_delta, (LPSTR) "--------" );
	}
	
	dlugosc1 = GetWindowTextLength( x2_amp_marker1 );
	Bufor1 =( LPSTR ) GlobalAlloc( GPTR, dlugosc1 + 1 );
	GetWindowText( x2_amp_marker1, Bufor1, dlugosc1 + 1 );
	dlugosc2 = GetWindowTextLength( x2_amp_marker2 );
	Bufor2 =( LPSTR ) GlobalAlloc( GPTR, dlugosc2 + 1 );			
	GetWindowText( x2_amp_marker2, Bufor2, dlugosc2 + 1 );
	if(m1_x2 && m2_x2)
	{
		M1 = GetNumberAmp(Bufor1);
		M2 = GetNumberAmp(Bufor2);
		SetWindowText( x2_amp_delta, GetAmpValue( abs(M2 - M1) ) );
		GlobalFree(Bufor1);
		GlobalFree(Bufor2);
		
		dlugosc1 = GetWindowTextLength( x2_czas_marker1 );
		Bufor1 =( LPSTR ) GlobalAlloc( GPTR, dlugosc1 + 1 );
		GetWindowText( x2_czas_marker1, Bufor1, dlugosc1 + 1 );
		dlugosc2 = GetWindowTextLength( x2_czas_marker2 );
		Bufor2 =( LPSTR ) GlobalAlloc( GPTR, dlugosc2 + 1 );
		GetWindowText( x2_czas_marker2, Bufor2, dlugosc2 + 1 );
		M1 = GetNumberTime(Bufor1);
		M2 = GetNumberTime(Bufor2);
		SetWindowText( x2_czas_delta, GetValue( abs(M2 - M1), 't', 1) );
		GlobalFree(Bufor1);
		GlobalFree(Bufor2);
	}
	else	
	{
		SetWindowText( x2_amp_delta, (LPSTR) "--------" );
		SetWindowText( x2_czas_delta, (LPSTR) "--------" );
	}
}

void DrawMarkers()
{
	HDC hdcOkno = GetDC(hwnd2);
	HPEN GreenPen = CreatePen( PS_SOLID, 1, 0x00FF00 );
	HPEN RedPen = CreatePen( PS_SOLID, 1, 0x0000FF );
	HPEN YellowPen = CreatePen( PS_SOLID, 1, 0x00FFFF ); 
	POINT stary_punkt;
	

		//--------Marker 1 dla x1---------
		if(m1_x1 == true)
		{
				if(m2_x1 == true && m1_x1_y == m2_x1_y)
					SelectObject(hdcOkno, YellowPen);
				else
					SelectObject(hdcOkno, GreenPen);								
				MoveToEx(hdcOkno, 150, m1_x1_y, &stary_punkt);
				LineTo(hdcOkno, 1152, m1_x1_y);
				
				if(m2_x1 == true && m1_x1_x == m2_x1_x)
					SelectObject(hdcOkno, YellowPen);
				else
					SelectObject(hdcOkno, GreenPen);					
				MoveToEx(hdcOkno, m1_x1_x, 15, &stary_punkt);
				LineTo(hdcOkno, m1_x1_x, 516);
		}
		
		//--------Marker 2 dla x1---------
	
		if(m2_x1 == true)
		{
			if(m1_x1 == true && m1_x1_y == m2_x1_y)
				SelectObject(hdcOkno, YellowPen);
			else
				SelectObject(hdcOkno, RedPen);								
			MoveToEx(hdcOkno, 150, m2_x1_y, &stary_punkt);
			LineTo(hdcOkno, 1152, m2_x1_y);
			
			if(m1_x1 == true && m1_x1_x == m2_x1_x)
				SelectObject(hdcOkno, YellowPen);
			else
				SelectObject(hdcOkno, RedPen);					
			MoveToEx(hdcOkno, m2_x1_x, 15, &stary_punkt);
			LineTo(hdcOkno, m2_x1_x, 516);
		}

		//--------Marker 1 dla x2---------
		if(m1_x2 == true)
		{
				if(m2_x2 == true && m1_x2_y == m2_x2_y)
					SelectObject(hdcOkno, YellowPen);
				else
					SelectObject(hdcOkno, GreenPen);								
				MoveToEx(hdcOkno, 150, m1_x2_y, &stary_punkt);
				LineTo(hdcOkno, 1152, m1_x2_y);
				
				if(m2_x2 == true && m1_x2_x == m2_x2_x)
					SelectObject(hdcOkno, YellowPen);
				else
					SelectObject(hdcOkno, GreenPen);					
				MoveToEx(hdcOkno, m1_x2_x, 554, &stary_punkt);
				LineTo(hdcOkno, m1_x2_x, 1065);
		}
		//--------Marker 2 dla x2---------	
		if(m2_x2 == true)
		{
			if(m1_x2 == true && m1_x2_y == m2_x2_y)
				SelectObject(hdcOkno, YellowPen);
			else
				SelectObject(hdcOkno, RedPen);								
			MoveToEx(hdcOkno, 150, m2_x2_y, &stary_punkt);
			LineTo(hdcOkno, 1152, m2_x2_y);
			
			if(m1_x2 == true && m1_x2_x == m2_x2_x)
				SelectObject(hdcOkno, YellowPen);
			else
				SelectObject(hdcOkno, RedPen);					
			MoveToEx(hdcOkno, m2_x2_x, 554, &stary_punkt);
			LineTo(hdcOkno, m2_x2_x, 1065);
		}
	
	SelectObject(hdcOkno, GreenPen);
	DeleteObject(GreenPen);
	SelectObject(hdcOkno, RedPen);
	DeleteObject(RedPen);
	SelectObject(hdcOkno, YellowPen);
	DeleteObject(YellowPen);
	ReleaseDC(hwnd2, hdcOkno);
}


void ZerujMarkery()
{
	m1_x1 = false;
	m2_x1 = false;
	m1_x2 = false;
	m2_x2 = false;
	SetWindowText( x1_amp_marker1, (LPSTR) "--------" );
	SetWindowText( x1_czas_marker1, (LPSTR) "--------" );
	SetWindowText( x1_amp_marker2, (LPSTR) "--------" );
	SetWindowText( x1_czas_marker2, (LPSTR) "--------" );
	SetWindowText( x1_amp_delta, (LPSTR) "--------" );
	SetWindowText( x1_czas_delta, (LPSTR) "--------" );
	SetWindowText( x2_amp_marker1, (LPSTR) "--------" );
	SetWindowText( x2_czas_marker1, (LPSTR) "--------" );
	SetWindowText( x2_amp_marker2, (LPSTR) "--------" );
	SetWindowText( x2_czas_marker2, (LPSTR) "--------" );
	SetWindowText( x2_amp_delta, (LPSTR) "--------" );
	SetWindowText( x2_czas_delta, (LPSTR) "--------" );
}

//Funkcja sprawdzajaca, czy tekst wczytany z pola typu EDIT jest liczba double
//Dopuszcza sie jedna kropke, spacje na koncu i uzywanie przedrostkow (p, n, u, m, k, M, G, T)
bool IsNumber(LPSTR text1) 
{
	string text = text1;
	bool kropka = false;
	bool nonZero = false;
    for(int i = 0; i < text.length() - 1; i++) 
    {
    	if(text[i]<='9' && text[i]>='1')
    		nonZero = true;
		if(text[i]>'9' || text[i]<'0')
			{
				if(text[text.length() -2] == ' ')
					break;
				if(kropka)
					return false;
				if(!kropka && text[i] == '.')
					kropka = true;
				else
					return false;
			}
	}
	if(text[text.length() - 1] <= '9' && text[text.length() - 1] >= '1')
		return true;
	else if(text[text.length() - 1] == '0' && nonZero)
		return true;	
	else if( (text[text.length() - 1] == '.' || text[text.length() - 1] == ' ') && nonZero)
		return true;
	else if(text[text.length() - 1] == 'k' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'M' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'G' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'T' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'm' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'u' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'n' && nonZero)
		return true;
	else if(text[text.length() - 1] == 'p' && nonZero)
		return true;
	else
		return false;
}

double GetNumber(LPSTR text1)
{
	string text = text1;
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
		return 0;
}

double GetNumberTime(LPSTR text1)
{
	string text = text1;
	if(text[text.length() - 2] <= '9' && text[text.length() - 2] >= '0')
		return atof(text1);
	else if(text[text.length() - 2] == '.' || text[text.length() - 2] == ' ')
		return atof(text1);
	else if(text[text.length() - 2] == 'k')
		return atof(text1)*1e3;
	else if(text[text.length() - 2] == 'M')
		return atof(text1)*1e6;
	else if(text[text.length() - 2] == 'G')
		return atof(text1)*1e9;
	else if(text[text.length() - 2] == 'T')
		return atof(text1)*1e12;
	else if(text[text.length() - 2] == 'm')
		return atof(text1)*1e-3;
	else if(text[text.length() - 2] == 'u')
		return atof(text1)*1e-6;
	else if(text[text.length() - 2] == 'n')
		return atof(text1)*1e-9;
	else if(text[text.length() - 2] == 'p')
		return atof(text1)*1e-12;
	else
		return 0;
}

double GetNumberAmp(LPSTR text1)
{
	string text = text1;
	LPSTR potega_text;	
	double podstawa;
	double potega;
	
	podstawa = atof(text1);	
	for(int i = 0; i < text.length(); i++)
	{
		if(text[i] == '^')
		{
			potega_text = &text[i+1];
			break;			
		}	
	}
	potega = atof(potega_text);	
	return podstawa*pow(10, potega);
}

//Funkcja zamieniajaca liczbe double na typ LPSTR, ktory mozna wyswietlac w kontrolkach
//Liczba wywietlana jest z zakresu <1;999> z dodatkowym przedrostkiem, jesli jest potrzebny
//Obsluguje liczby z zakresy <1e-12;1e15>
LPSTR GetValue(double liczba, char typ, bool zero_acceptable)
{
	string napis;
	ostringstream strs;
	bool minus = false;
	if(abs(liczba) != liczba)
	minus = true;
	liczba = abs(liczba);
	
	if(liczba < 1e-12 && !zero_acceptable)
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
	else if(liczba >= 1e-9 && liczba < 1e-6)
	{
		if(minus)
			liczba = -1*liczba;
			strs << liczba*1e9;
		if(typ != '-')
			napis = strs.str() + " n";
		else
			napis = strs.str() + "n";
	}
	else if(liczba >= 1e-6 && liczba < 1e-3)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e6;
		if(typ != '-')
			napis = strs.str() + " u";
		else
			napis = strs.str() + "u";
	}
	else if(liczba >= 1e-3 && liczba < 1)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e3;
		if(typ != '-')
			napis = strs.str() + " m";
		else
			napis = strs.str() + "m";
	}
	else if( (liczba >= 1 && liczba < 1e3) || liczba == 0)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba;
		if(typ != '-')
			napis = strs.str() + " ";
		else
			napis = strs.str();
	}
	else if(liczba >= 1e3 && liczba < 1e6)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-3;
		if(typ != '-')
			napis = strs.str() + " k";
		else
		napis = strs.str() + "k";
	}
	else if(liczba >= 1e6 && liczba < 1e9)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-6;
		if(typ != '-')
			napis = strs.str() + " M";
		else
		napis = strs.str() + "M";
	}
	else if(liczba >= 1e9 && liczba < 1e12)
	{
		if(minus)
			liczba = -1*liczba;
		strs << liczba*1e-9;
		if(typ != '-')
			napis = strs.str() + " G";
		else
		napis = strs.str() + "G";
	}
	else if(liczba >= 1e12 && liczba <= 1e15)
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
		case 't':
		{
			napis += "s";
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
	if(abs(liczba) < pow(10, -60))
		return 1*pow(10, -60);
	else if(abs(liczba) > pow(10, 60))
		return 1*pow(10, 60);
	for(double i = -60; i <= 60; i += 3)
	{	
		if(abs(liczba >= pow(10, i) && abs(liczba) < pow(10, i + 3)))
		{
			liczba *= pow(10, -i);
			liczba = ceil(liczba);
			liczba /= pow(10, -i);
		}
	}
	return abs(liczba);
}

LPSTR GetAmpValue(double liczba)
{
	string napis;
	ostringstream strs;
	ostringstream strs1;
	
	if(liczba == 0)
	{
		strs << 0;
		napis = strs.str();
	}
	else if(abs(liczba) < pow(10, -60))
	{
		if(liczba == abs(liczba))
		{
			strs << 1;
			napis = strs.str();
			napis = strs.str() + "*10^";
			strs1 << -60;
			napis += strs1.str();
		}
		else
		{
			strs << -1;
			napis = strs.str();
			napis = strs.str() + "*10^";
			strs1 << -60;
			napis += strs1.str();
		}
	}
	else if(abs(liczba) > pow(10, 60))
	{
		if(liczba == abs(liczba))
		{
			strs << 1;
			napis = strs.str();
			napis = strs.str() + "*10^";
			strs1 << 60;
			napis += strs1.str();
		}
		else
		{
			strs << -1;
			napis = strs.str();
			napis = strs.str() + "*10^";
			strs1 << 60;
			napis += strs1.str();
		}
	}	
	else
	{
		for(double i = -60; i <= 60; i += 3)
		{	
			if(abs(liczba) >= pow(10, i) && abs(liczba) < pow(10, i + 3))
			{
				strs << liczba*pow(10, -i);
				napis = strs.str() + "*10^";
				strs1 << i;
				napis += strs1.str();
				break;
			}
		}
	}
	napis += " V";	
	
	LPSTR wynik = &napis[0];
	return wynik;
}


//
void GraphCalculations()
{
	MaxAmp_x1 = 0;
	MaxAmp_x2 = 0;
		
	int granica = 1000;
	double U[granica];
	double X1[granica];
	double F1 = 0;
	double F1p = 0;
	double F1pp = 0;
	double X2[granica];
	double F2 = 0;
	double F2p = 0;
	double F2pp = 0;
	double liczba_okresow = czas_symulacji*sygnal_freq;
	double krok = liczba_okresow/1000;
	
	switch(sygnal_typ)
	{
		case 0:
		{
			for ( int i = 0 ; i < granica ; i++ )
			{
				U[i] = sygnal_amp;
			}
			break;
		}
		case 1:
		{
			for ( int i = 0 ; i < granica ; i++ ){
				U[i] = FalaProstokatna(i);
			}
			break;
		}
		case 2:
		{
			for ( int i = 0 ; i < granica ; i++ ){
				U[i] = sin(((i*2*M_PI*liczba_okresow)/1000));
			}
			break;
		}
	}
	
	X1[0] = 0;
	X2[0] = 0;
	X1[1] = krok*( (1/(R1*C1))*U[0] )/3.0;
	X2[1] = 0;
	X1[2] = X1[1] + krok*( 4*(1/(R1*C1))*U[0] + ((-(R1+R2)/(R1*R2*C1))*X1[1] + (1/(R1*C1))*U[1]) )/3.0;
	X2[2] = krok*( (1/(R2*C2))*X1[1] )/3.0;
	F1p = (1/(R1*C1))*U[0];
	F1 = (-(R1+R2)/(R1*R2*C1))*X1[1] + (1/(R1*C1))*U[1];
	F2 = (1/(R2*C2))*X1[1];
	
	for ( int i = 3 ; i < granica ; i++)
	{
		F1pp = F1p;
		F1p = F1;
		F1 = (-(R1+R2)/(R1*R2*C1))*X1[i-1] + (1/(R2*C1))*X2[i-1] + (1/(R1*C1))*U[i-1];	
		
		X1[i] = X1[i-1] + krok*( F1pp + 4*F1p + F1 )/3.0;
		
		F2pp = F2p;
		F2p = F2;
		F2 = (1/(R2*C2))*X1[i-1] - (1/(R2*C2))*X2[i-1];
		
		X2[i] = X2[i-1] + krok*( F2pp + 4*F2p + F2 )/3.0;
		
		//Zabezpieczenie przed wartosciami mniejszymi niz 10^-60 i wiekszymi niz 10^60
		//Ogolnie tego brakowalo, juz raczej wykresy sie nie beda wypierdalac
		if(abs(X1[i]) < pow(10, -60))
		{
			if(X1[i] == abs(X1[i]))
				X1[i] = 1*pow(10, -60);
			else
				X1[i] = -1*pow(10, -60);
		}
		else if(X1[i] > pow(10, 60))
		{
			if(X1[i] == abs(X1[i]))
				X1[i] = 1*pow(10, 60);
			else
				X1[i] = -1*pow(10, 60);
		}
			
		if(abs(X2[i]) < pow(10, -60))
		{
			if(X2[i] == abs(X2[i]))
				X2[i] = 1*pow(10, -60);
			else
				X2[i] = -1*pow(10, -60);
		}
		else if(X2[i] > pow(10, 60))
		{
			if(X2[i] == abs(X2[i]))
				X2[i] = 1*pow(10, 60);
			else
				X2[i] = -1*pow(10, 60);
		}
				
		if( abs(X1[i]) > MaxAmp_x1 ){
			MaxAmp_x1 = abs(X1[i]);
		}
		if( abs(X2[i]) > MaxAmp_x2 ){
			MaxAmp_x2 = abs(X2[i]);
		}
	}	
	
	SetWindowText( simulation_time, GetValue(czas_symulacji, 't', 0) );
	
	MaxAmp_x1 = GetMaxAmp(MaxAmp_x1);
	SetWindowText( max_amp_x1, GetAmpValue(MaxAmp_x1) );
	SetWindowText( time_x1, GetValue(czas_symulacji, 't', 0) );
	if(m1_x1 == true)
	{
		SetWindowText( x1_amp_marker1, GetAmpValue( MaxAmp_x1 * (266 - m1_x1_y) / 250 ) );
		SetWindowText( x1_czas_marker1, GetValue( (czas_symulacji * (m1_x1_x - 151) / 1000), 't', 1) );
	}
	if(m2_x2 == true)
	{
		SetWindowText( x1_amp_marker2, GetAmpValue( MaxAmp_x1 * (266 - m2_x1_y) / 250 ) );
		SetWindowText( x1_czas_marker2, GetValue( (czas_symulacji * (m2_x1_x - 151) / 1000), 't', 1) );
	}
	DrawGraph(X1, MaxAmp_x1, 1);
	
	MaxAmp_x2 = GetMaxAmp(MaxAmp_x2);
	SetWindowText( max_amp_x2, GetAmpValue(MaxAmp_x1) );
	SetWindowText( time_x2, GetValue(czas_symulacji, 't', 0) );
	if(m1_x2 == true)
	{
		SetWindowText( x2_amp_marker1, GetAmpValue( MaxAmp_x1 * (805 - m1_x2_y) / 250 ) );
		SetWindowText( x2_czas_marker1, GetValue( (czas_symulacji * (m1_x2_x - 151) / 1000), 't', 1) );
	}
	if(m2_x2 == true)
	{
		SetWindowText( x2_amp_marker2, GetAmpValue( MaxAmp_x1 * (805- m2_x2_y) / 250 ) );
		SetWindowText( x2_czas_marker2, GetValue( (czas_symulacji * (m2_x2_x - 151) / 1000), 't', 1) );
	}
	ZmienDelte();
	DrawGraph(X2, MaxAmp_x2, 2);
}

//Funkcja rysujaca kreske rozdzielajaca w WndProcChild
void Rozdzielacz(int szerokosc, int y, int kolor)
{
	HDC hdcOkno = GetDC(hwnd2);
	POINT stary_punkt;
	HPEN Pioro;
	switch(kolor)
	{
		case 0:
			Pioro = CreatePen( PS_SOLID, 1, 0x000000 );
			break;
		case 1:
			Pioro = CreatePen( PS_SOLID, 1, 0x828282 );
			break;
		case 2:
			Pioro = CreatePen( PS_SOLID, 1, 0xFF1111 );
			break;
		case 3:
			Pioro = CreatePen( PS_SOLID, 1, 0x00FF00 );
			break;
		case 4:
			Pioro = CreatePen( PS_SOLID, 1, 0x0000FF );
			break;
	}
	SelectObject(hdcOkno, Pioro);
	for(int i = 0; i < szerokosc; i++)
	{
		MoveToEx(hdcOkno, 0, y + i, &stary_punkt);
		LineTo(hdcOkno, 1251, y + i);
	}
	DeleteObject(Pioro);
	ReleaseDC(hwnd2, hdcOkno);
}

//Funkcja rysujaca wykres w odpowiednim miejscu w WndProcChild
void DrawGraph(double sygnal[], double max_amp, int numer)
{
	HDC hdcOkno = GetDC(hwnd2);
	HPEN NiebieskiePioro = CreatePen( PS_SOLID, 1, 0xFF0000 );
	POINT stary_punkt;
	double kratka_y = max_amp/250;
	int start_coord_x = 152;
	int start_coord_y;
	
	switch(numer)
	{
		case 1:{
			start_coord_y = 266;
			break;
		}
		case 2:{
			start_coord_y = 805;
			break;
		}
	}
	
	MoveToEx(hdcOkno, start_coord_x, start_coord_y, &stary_punkt);
	SelectObject(hdcOkno, NiebieskiePioro);
	for(int i = 0; i < 1000; i++)
	{
		int coord_y = round(sygnal[i]/kratka_y);
		LineTo(hdcOkno, start_coord_x + i + 1, start_coord_y - coord_y);
	}
	
	DeleteObject(NiebieskiePioro);
	ReleaseDC(hwnd2, hdcOkno);
}


double FalaProstokatna(int liczba)
{
	int T_pol_pixele = round( 500 / (czas_symulacji * sygnal_freq) );
	liczba = liczba % (T_pol_pixele * 2);
	if(liczba < T_pol_pixele)
		return sygnal_amp;
	else
		return -1*sygnal_amp;
}

