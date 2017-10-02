#include <string>
#include "ground.h"

/*
Title: hw1
Author: Peter King
Date: 05/2017
Description:
a simple arkanoid like game done for hw1

*/

#define MAX_LOADSTRING 1000
// Globale Variablen:
HINSTANCE hInst;								// Aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters
HFONT editfont;
HWND hMain = NULL;
//-----------------------
static char MainWin[] = "MainWin";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void draw_line(HDC DC, int x, int y, int a, int b, int red, int green, int blue, int width);

HBRUSH  hWinCol = CreateSolidBrush(RGB(180, 180, 180));

//ball pos
int bX = 250;
int bY = 250;
//ball direction
int bdX = 5;
int bdY = -10;
//shuffle
int sX = 450;

//mouse pos
int mX;

//astroid pos & size
int aY = 50;
int aS = 20;

//end game
bool endgame = false;


//brick class
class Brick;
Brick* brick1;
Brick* brick2;
Brick* brick3;
Brick* brick4;
Brick* brick5;

//score
int score = 0;
//count
int counts = 0;

HWND g_hwnd=NULL;
//timer:
#define TIMER1 111

class Brick {
private:
	int posUpperLeftX;
	int posUpperLeftY;
	int posBottomRightX;
	int posBottomRightY;
	int thickness;
	bool visible;
	Brick() {
		posUpperLeftX = 0;
		posUpperLeftY = 0;
		posBottomRightX = 0;
		posBottomRightY = 0;
		thickness = 0;
		visible = true;
	}
public:
	Brick(int x, int y) {
		posUpperLeftX = x;
		posUpperLeftY = y;
		posBottomRightX = x+ 40;
		posBottomRightY = y + 20;
		thickness = 1;
		visible = true;
	}
	void display(HDC dc) {
		if (visible) {
			draw_line(dc, posUpperLeftX, posUpperLeftY, posUpperLeftX + 40, posUpperLeftY, 0, 255, 0, thickness);//top
			draw_line(dc, posUpperLeftX, posUpperLeftY, posUpperLeftX, posUpperLeftY + 20, 0, 255, 0, thickness);//left
			draw_line(dc, posUpperLeftX, posUpperLeftY + 20, posUpperLeftX + 40, posUpperLeftY + 20, 0, 255, 0, thickness);//botmm
			draw_line(dc, posUpperLeftX + 40, posUpperLeftY, posUpperLeftX + 40, posUpperLeftY + 20, 0, 255, 0, thickness);//right
		}
		
		
	}
	void moveup() {
		//moving down screen
		posUpperLeftY++;

	}
	bool collision(int x, int y) {
	//returns true if inside box
		if (x > posUpperLeftX && x < posBottomRightX && y > posUpperLeftY && y < posBottomRightY) {
			//visible = false;
			posUpperLeftY = 0;
			bdY = -bdY; // chaning ball direction
			score++; //updating score
			
			return true;//collision
		}
		
		
		return false;

	}

};

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
/*ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

*/

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{

	
	hInst = hInstance;												//						save in global variable for further use
	// TODO: Hier Code einfügen.
	MSG msg;
	
	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, 103, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, 104, szWindowClass, MAX_LOADSTRING);
	//register Window													<<<<<<<<<<			STEP ONE: REGISTER WINDOW						!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	WNDCLASSEX wcex;												//						=> Filling out struct WNDCLASSEX
	BOOL Result = TRUE;
	wcex.cbSize = sizeof(WNDCLASSEX);								//						size of this struct (don't know why
	wcex.style = CS_HREDRAW | CS_VREDRAW;							//						?
	wcex.lpfnWndProc = (WNDPROC)WndProc;							//						The corresponding Proc File -> Message loop switch-case file
	wcex.cbClsExtra = 0;											//
	wcex.cbWndExtra = 0;											//
	wcex.hInstance = hInstance;										//						The number of the program
	wcex.hIcon = LoadIcon(hInstance, NULL);							//
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);						//
	wcex.hbrBackground = hWinCol;									//						Background color
	wcex.lpszMenuName = NULL;										//
	wcex.lpszClassName = MainWin;									//						Name of the window (must the the same as later when opening the window)
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);					//
	Result = (RegisterClassEx(&wcex) != 0);							//						Register this struct in the OS

				//													STEP TWO: OPENING THE WINDOW with x,y position and xlen, ylen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	hMain = CreateWindow(MainWin, "TestWin CST 320", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, hInst, NULL);
	if (!hMain)return 0;
	ShowWindow(hMain, nCmdShow);
	UpdateWindow(hMain);

	//													STEP THRE: Going into the infinity message loop							  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Hauptmeldungsschleife:
	bool quit = FALSE;
	while (!quit)
	{
		//if (GetMessage(&msg, NULL, 0, 0) > 0)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT){ quit = TRUE; break; }

			
				TranslateMessage(&msg);								//						IF a meessage occurs, the WinProc will be called!!!!
				DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
///////////////////////////////////////////////////
void redr_win_full(HWND hwnd, bool erase)
	{
	RECT rt;
	GetClientRect(hwnd, &rt);
	InvalidateRect(hwnd, &rt, erase);
	}
void draw_text(HDC DC,char text[], int x, int y, int r, int g, int b)
	{
	COLORREF rgb = RGB(r, g, b);
	SetTextColor(DC, rgb);
	SetBkMode(DC, TRANSPARENT);
	TextOut(DC, x, y, text, strlen(text));
	}
///////////////////////////////////
//		This Function is called every time the Left Mouse Button is down
///////////////////////////////////
void OnLBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is down
///////////////////////////////////
void OnRBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
	{
	
	}
///////////////////////////////////
//		This Function is called every time a character key is pressed
///////////////////////////////////
void OnChar(HWND hwnd, UINT ch, int cRepeat)
{

}
///////////////////////////////////
//		This Function is called every time the Left Mouse Button is up
///////////////////////////////////
void OnLBU(HWND hwnd, int x, int y, UINT keyFlags)
{

	
}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is up
///////////////////////////////////
void OnRBU(HWND hwnd, int x, int y, UINT keyFlags)
	{
	

	}
///////////////////////////////////
//		This Function is called every time the Mouse Moves
///////////////////////////////////
void OnMM(HWND hwnd, int x, int y, UINT keyFlags)
{
	mX = x;//getting mouse coords
if ((keyFlags & MK_LBUTTON) == MK_LBUTTON)
	{
	}

if ((keyFlags & MK_RBUTTON) == MK_RBUTTON)
	{
	}
}
///////////////////////////////////
//		This Function is called once at the begin of a program
///////////////////////////////////
#define TIMER1 1

BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
g_hwnd = hwnd;
	editfont = CreateFont(-10, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");	
	if (!SetTimer(hwnd, TIMER1, 20, NULL))
		{
		MessageBox(hwnd, "No Timers Available", "Info", MB_OK);
		return FALSE;
		}
	
	
	brick1 = new Brick(15, 35);
	brick2 = new Brick(110, 35);
	brick3 = new Brick(205, 35);
	brick4 = new Brick(300, 35);
	brick5 = new Brick(405, 35);
	


	return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND hwin;
	
	switch (id)
	{
	default:
		break;
	}

}
//************************************************************************
void OnTimer(HWND hwnd, UINT id)
	{
	redr_win_full(hwnd, FALSE);
	counts++;
	//ball birck colission
	brick1->collision(bX, bY);
	brick2->collision(bX, bY);
	brick3->collision(bX, bY);
	brick4->collision(bX, bY);
	brick5->collision(bX, bY);

	//shuffle collisions
	if (bY > 400 && bX < mX + 30 && bX > mX) {//if ball is even with shuffle and between start and end of line
		bdY = -bdY;//swap Y direction
				   //bdX = -bdX;//swap X direction

	}

	



	//checking for ball colissions
	if (bY < 500 && bX < 15 || bX > 485) {
		bdX = -bdX;//swap X direction
			}
	if (bY <500 && bY< 15) {
		bdY = -bdY;//swap Y directio
	}
	if (bY > 500) {

		endgame = true;;
		//PostQuitMessage(1);//end game
	}
	
	
	
	bX += bdX;//adding ball movment
	bY += bdY;//adding ball movment

	if (counts % 50 == 0) {
		brick1->moveup();
		brick2->moveup();
		brick3->moveup();
		brick4->moveup();
		brick5->moveup();
	}
	}



//************************************************************************
///////////////////////////////////
//		This Function is called every time the window has to be painted again
///////////////////////////////////


void OnPaint(HWND hwnd)
	{
	PAINTSTRUCT PaintStruct;
	double_buffer_ doublebuffer;
	HDC dc = doublebuffer.begin(hwnd, &PaintStruct, RGB(255, 255, 255));
	draw_line(dc, 0, 0,500, 500, 0, 0, 0, 700);//background color

	//walls
	draw_line(dc, 0, 0, 0, 485, 0, 255, 0, 15);//wall left
	draw_line(dc, 0, 0, 485, 0, 0, 255, 0, 15);//wall top
	draw_line(dc, 485, 0, 485, 485, 0, 255, 0, 15);//wall right
	

	brick1->display(dc);
	brick2->display(dc);
	brick3->display(dc);
	brick4->display(dc);
	brick5->display(dc);



	for (int yy = 20; yy < 40; yy++)
		for (int xx = 20; xx < 50; xx++)
			//SetPixel(dc, xx, yy, RGB(0, 200, 0));
	
	//draw_line(dc, bX, bY, bX -(bdX * 2), bY - (bdY * 2), 0, 255, 0, 10);//taill

	draw_line(dc, bX, bY, bX, bY, 0, 255, 0, 10);//ball
	draw_line(dc, mX, 400, mX+30, 400, 194, 194, 214, 10);//shuffle

	
	string temp = to_string(score);
	char* char_type = new char[temp.length()];
	strcpy(char_type, temp.c_str());

	draw_text(dc, "Score:", 10, 10, 20, 255, 0);
	draw_text(dc, char_type, 55, 10, 20, 255, 0);

	if (endgame) {
		draw_text(dc, "GAME OVER:", 200, 200, 20, 255, 0);

		draw_text(dc, "SCORE:", 205, 215, 20, 255, 0);
		draw_text(dc, char_type, 260, 215, 20, 255, 0);
	}

	doublebuffer.end();

	}
//****************************************************************************

void draw_line(HDC DC,int x, int y, int a, int b, int red, int green, int blue, int width)
	{
	HPEN Stift = CreatePen(PS_SOLID, width, RGB(red, green, blue));
	SelectObject(DC, Stift);
	MoveToEx(DC, x, y, NULL);
	LineTo(DC, a, b);
	DeleteObject(Stift);
	}
//*************************************************************************
void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
	{

	switch (vk)
		{
			default:break;
			
		}
	}

//*************************************************************************
void OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
	{
	switch (vk)
		{
			default:break;
			
		}

	}


//**************************************************************************
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	SCROLLINFO si;


	
	switch (message)
	{
	/*
	#define HANDLE_MSG(hwnd, message, fn)    \
    case (message): return HANDLE_##message((hwnd), (wParam), (lParam), (fn))
	*/
		
		HANDLE_MSG(hwnd, WM_CHAR, OnChar);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLBD);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLBU);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMM);	
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
		HANDLE_MSG(hwnd, WM_KEYDOWN, OnKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, OnKeyUp);

	case WM_ERASEBKGND:
		return (LRESULT)1;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}


