#include "pch.h"
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <richedit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winspool.h>

#define HIBA_00 TEXT("Error:Program initialisation process.")
#define IDC_STATIC -1
#define LVM_SELECTED 2
#define WINSTYLE_NORMAL (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)
#define WINSTYLE_DIALOG (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU)
#define WINSTYLE_NONESIZEABLE (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX)
#define WINSTYLE_CAPTIONONLY (WS_DLGFRAME)
#define WINSTYLE_NOCAPTON (WS_POPUP)

HINSTANCE hInstGlob;
int SajatiCmdShow;

#define OBJ_ID100 100
#define OBJ_ID101 101
#define OBJ_ID102 102
HWND Form1;
HWND button1;
HWND button2;
HWND button3;
LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
char szClassName[] = "WindowsApp";

void text_printing();
void graphics_printing();
void bitmap_printing();
HDC GetPrinterDC(HWND Hwnd);
void set_print_scaling(HDC akt_hdc);

struct bmp_info {
 BITMAPFILEHEADER *pbmfh;
 BITMAPINFO *pbmi;
 BYTE *pBits;
 int Width, Height;
};
bmp_info mybitmap;

BITMAPFILEHEADER *DibLoadImage(PTSTR pstrFileName);
void draw_BMP(HDC hdc, BITMAPINFO *pbmi, BYTE *pBits, int xkoo, int ykoo, int width, int height, int bitmap_width, int bitmap_height);
void load_BMP(bmp_info *_bitmap, const char bitmapfile[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
 static TCHAR szAppName[] = TEXT("StdWinClassName");
 HWND hwnd;
 MSG msg;
 WNDCLASS wndclass0;
 SajatiCmdShow = iCmdShow;
 hInstGlob = hInstance;

 wndclass0.style = CS_HREDRAW | CS_VREDRAW;
 wndclass0.lpfnWndProc = WndProc0;
 wndclass0.cbClsExtra = 0;
 wndclass0.cbWndExtra = 0;
 wndclass0.hInstance = hInstance;
 wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
 wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
 wndclass0.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
 wndclass0.lpszMenuName = NULL;
 wndclass0.lpszClassName = TEXT("WIN0");

 if (!RegisterClass(&wndclass0))
 {
  MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR); return 0;
 }

 Form1 = CreateWindow(TEXT("WIN0"),
  TEXT("Printer basics"),
  WINSTYLE_NORMAL,
  0,
  0,
  280,
  270,
  NULL,
  NULL,
  hInstance,
  NULL);

 ShowWindow(Form1, SajatiCmdShow);
 UpdateWindow(Form1);
 while (GetMessage(&msg, NULL, 0, 0))
 {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 return msg.wParam;
}

LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 int s1;
 HDC hdc;
 PAINTSTRUCT ps;
 RECT rect;

 switch (message)
 {
 case WM_CREATE:
  button1 = CreateWindow(TEXT("button"), TEXT("Text")
   , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 50, 50, 170, 30
   , hwnd, (HMENU)(OBJ_ID100), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
  button2 = CreateWindow(TEXT("button"), TEXT("Graphics")
   , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 50, 100, 170, 30
   , hwnd, (HMENU)(OBJ_ID101), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
  button3 = CreateWindow(TEXT("button"), TEXT("Bitmap")
   , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 50, 150, 170, 30
   , hwnd, (HMENU)(OBJ_ID102), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
  return 0;
 case WM_COMMAND:
  switch (LOWORD(wParam))
  {
  case OBJ_ID100:text_printing(); break;
  case OBJ_ID101:graphics_printing(); break;
  case OBJ_ID102:bitmap_printing(); break;
  }
  return 0;
 case WM_SIZE:
  return 0;
 case WM_PAINT:
  hdc = BeginPaint(hwnd, &ps);
  EndPaint(hwnd, &ps);
  return 0;
 case WM_CLOSE:
  DestroyWindow(hwnd);
  return 0;
 case WM_DESTROY:
  PostQuitMessage(0);
  return 0;
 }
 return DefWindowProc(hwnd, message, wParam, lParam);
}

void text_printing()
{
 HDC hdc, hprinter;
 static DOCINFO di = { sizeof(DOCINFO), TEXT("Text content") };
 HFONT DisplayFont;

 hprinter = GetPrinterDC(Form1);
 StartDoc(hprinter, &di);
 StartPage(hprinter);
 set_print_scaling(hprinter);
 HPEN myPen;
 myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
 SelectObject(hprinter, myPen);
 SetDCBrushColor(hprinter, RGB(255, 0, 0));
 SetTextColor(hprinter, RGB(0, 200, 0));
 SetBkColor(hprinter, RGB(0, 0, 255));

 DisplayFont = CreateFont(166, 0, 0, 0, FW_DONTCARE, false, false, false, DEFAULT_CHARSET,
  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
  DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial Rounded MT Bold"));
 SelectObject(hprinter, DisplayFont);
 TextOut(hprinter, 0, 10, TEXT("Text1"), 7);
 TextOut(hprinter, 0, 100, TEXT("Text2"), 18);

 DeleteObject(myPen);
 EndPage(hprinter);
 EndDoc(hprinter);
 DeleteDC(hprinter);
}

void graphics_printing()
{
 HDC hdc, hprinter;
 static DOCINFO di = { sizeof(DOCINFO), TEXT("Graphics content") };

 hprinter = GetPrinterDC(Form1);
 StartDoc(hprinter, &di);
 StartPage(hprinter);
 set_print_scaling(hprinter);
 HPEN myPen;
 myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
 SelectObject(hprinter, myPen);

 MoveToEx(hprinter, 50, 500, NULL);
 LineTo(hprinter, 500, 50);
 MoveToEx(hprinter, 50, 50, NULL);
 LineTo(hprinter, 500, 500);

 DeleteObject(myPen);
 EndPage(hprinter);
 EndDoc(hprinter);
 DeleteDC(hprinter);
}

HDC GetPrinterDC(HWND Hwnd)
{
 HDC hdc;
 PRINTDLG pd = { 0 };
 pd.lStructSize = sizeof(pd);
 pd.hwndOwner = Hwnd;
 pd.Flags = PD_RETURNDC;
 PrintDlg(&pd);
 hdc = pd.hDC;
 return hdc;
}

BITMAPFILEHEADER *DibLoadImage(PTSTR pstrFileName)
{
 BOOL bSuccess;
 DWORD dwFileSize, dwHighSize, dwBytesRead;
 HANDLE hFile;
 BITMAPFILEHEADER *pbmfh;

 hFile = CreateFileA((LPCSTR)pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
  OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

 if (hFile == INVALID_HANDLE_VALUE) { return NULL; }
 dwFileSize = GetFileSize(hFile, &dwHighSize);

 if (dwHighSize) { CloseHandle(hFile); return NULL; }
 pbmfh = (BITMAPFILEHEADER *)malloc(dwFileSize);
 if (!pbmfh) { CloseHandle(hFile); return NULL; }
 bSuccess = ReadFile(hFile, pbmfh, dwFileSize, &dwBytesRead, NULL);
 CloseHandle(hFile);

 if (!bSuccess || (dwBytesRead != dwFileSize)
  || (pbmfh->bfType != *(WORD *)"BM")
  || (pbmfh->bfSize != dwFileSize))
 {
  free(pbmfh); return NULL;
 }
 return pbmfh;
}

void draw_BMP(HDC hdc, BITMAPINFO *pbmi, BYTE *pBits, int xkoo, int ykoo, int width, int height, int bitmap_width, int bitmap_height)
{
 SetStretchBltMode(hdc, COLORONCOLOR);
 StretchDIBits(hdc, xkoo, ykoo, width, height, 0, 0, bitmap_width, bitmap_height, pBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
}

void load_BMP(bmp_info *_bitmap, const char bitmapfile[])
{
 if (_bitmap->pbmfh) { free(_bitmap->pbmfh); _bitmap->pbmfh = NULL; }
 _bitmap->pbmfh = DibLoadImage(PTSTR(bitmapfile));
 if (_bitmap->pbmfh == NULL);
 else
 {
  _bitmap->pbmi = (BITMAPINFO *)(_bitmap->pbmfh + 1);
  _bitmap->pBits = (BYTE *)_bitmap->pbmfh + _bitmap->pbmfh->bfOffBits;
  if (_bitmap->pbmi->bmiHeader.biSize == sizeof(BITMAPCOREHEADER))
  {
   _bitmap->Width = ((BITMAPCOREHEADER *)_bitmap->pbmi)->bcWidth;
   _bitmap->Height = ((BITMAPCOREHEADER *)_bitmap->pbmi)->bcHeight;
  }
  else
  {
   _bitmap->Width = _bitmap->pbmi->bmiHeader.biWidth;
   _bitmap->Height = abs(_bitmap->pbmi->bmiHeader.biHeight);
  }
 }
}

void bitmap_printing()
{
 load_BMP(&mybitmap,"bitmap.bmp");
 HDC hdc, hprinter;
 static DOCINFO di = { sizeof(DOCINFO), TEXT("Bitmap content") };

 hprinter = GetPrinterDC(Form1);
 StartDoc(hprinter, &di);
 StartPage(hprinter);
 set_print_scaling(hprinter);
 draw_BMP(hprinter, mybitmap.pbmi, mybitmap.pBits, 10, 10, 1000, 600, 1000, 600);
 EndPage(hprinter);
 EndDoc(hprinter);
 DeleteDC(hprinter);
}

void set_print_scaling(HDC akt_hdc)
{
 /*RECT client_area;
 GetClientRect(Form1, &client_area);

 int WPrinter = GetDeviceCaps(akt_hdc, HORZRES);
 int HPrinter = GetDeviceCaps(akt_hdc, VERTRES);

 SetMapMode(akt_hdc, MM_ANISOTROPIC);

 SetWindowExtEx(akt_hdc, client_area.right,
  client_area.bottom, NULL);

 SetViewportExtEx(akt_hdc, WPrinter,
  HPrinter, NULL);*/

 HDC screenHDC = GetDC(Form1);

 int LogPixelsXWin = GetDeviceCaps(screenHDC, LOGPIXELSX);
 int LogPixelsYWin = GetDeviceCaps(screenHDC, LOGPIXELSY);
 ReleaseDC(Form1,screenHDC);

 HDC printerHDC = akt_hdc;

 int LogPixelsXWinPrinter = GetDeviceCaps(printerHDC, LOGPIXELSX);
 int LogPixelsYWinPrinter = GetDeviceCaps(printerHDC, LOGPIXELSY);

 SetMapMode(printerHDC, MM_ANISOTROPIC);
 SetWindowExtEx(printerHDC, LogPixelsXWin, LogPixelsYWin, NULL);

 SetViewportExtEx(printerHDC, LogPixelsXWinPrinter,
  LogPixelsYWinPrinter, NULL);
}