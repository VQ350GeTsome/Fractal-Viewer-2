// FractalViewer.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Window.h"
#include <windowsx.h> // For GET_X_LPARAM & GET_Y_LPARAM 
#include <string>

#include <omp.h>	// For parallel processing

#include "Fractal.h"
#include "ComplexNumber.h"
#include "PrefabFractals.h"

#define MAX_LOADSTRING 100

// Global Variables:
int width = 250, height = width / 2;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Our fractal
Fractal<long double>* fractal = new Fractal<long double>(width, height, fractals.CUSTOM);

HBITMAP hFractalBmp = NULL;
int* iterations = nullptr;
uint32_t* pixels = nullptr;

// Tracking the client area size
int clientWidth = width, clientHeight = height;

// For panning
int lastPanX = 0;
int lastPanY = 0;
bool isPanning = false;

// Control Settings
bool holdlessJulia = false;

// Forward declarations of functions included in this code module:
inline void 		refreshJulia(HWND);
inline void 		refreshMandelbrot(HWND);
inline void			refreshGradient(HWND);

inline void 		refreshScreen(HWND);
inline void			refreshFractal(HWND, int);

inline POINT		scaleMouse(HWND, LPARAM);
inline POINT        scaleMouse(int, int);

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FRACTALVIEWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRACTALVIEWER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRACTALVIEWER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FRACTALVIEWER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Store instance handle in our global variable

	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr
	);


	if (!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {

		// Handle window resizing
		case WM_SIZE: {
			clientWidth = LOWORD(lParam);
			clientHeight = HIWORD(lParam);
			InvalidateRect(hWnd, nullptr, FALSE);
			return 0;
		}

		// Create bitmap and pixel buffer when the window is created
		case WM_CREATE: {

			BITMAPINFO bmi = {};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -height;   // top-down
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;      // BGRA
			bmi.bmiHeader.biCompression = BI_RGB;

			void* pixelBuffer = nullptr;

			hFractalBmp = CreateDIBSection(
				nullptr,
				&bmi,
				DIB_RGB_COLORS,
				&pixelBuffer,
				nullptr,
				0
			);

			pixels = static_cast<uint32_t*>(pixelBuffer);
			iterations = new int[width * height];

			refreshScreen(hWnd);
			refreshGradient(hWnd);
			return 0;
		}
		break;

		// Menu bar commands
		case WM_COMMAND: {
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId) {
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_REFRESH:
					refreshScreen(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		// When painting is needed
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			HDC memDC = CreateCompatibleDC(hdc);
			SelectObject(memDC, hFractalBmp);

			SetStretchBltMode(hdc, COLORONCOLOR); // important for crisp pixels

			StretchBlt(
				hdc,
				0, 0, clientWidth, clientHeight,   // destination (window size)
				memDC,
				0, 0, width, height,               // source (fixed buffer)
				SRCCOPY
			);


			DeleteDC(memDC);
			EndPaint(hWnd, &ps);
			return 0;
		}
		break;

		// Key presses
		case WM_KEYDOWN: {

			switch (wParam) {

				// R refreshes the fractal
				case 'R': {
					fractal->reset();
					refreshScreen(hWnd);
				}
				break;
				case 'I': {
					// Get Mouse position
					POINT pt;
					GetCursorPos(&pt);
					ScreenToClient(hWnd, &pt);
					pt = scaleMouse(pt.x, pt.y);

					std::string info = fractal->toString(pt.x, pt.y);
					MessageBoxA(hWnd, info.c_str(), "Fractal Info", MB_OK);
				}
				break;
			}
		}
		break;

		// Left mouse button sets new Julia C
		case WM_LBUTTONDOWN: {
			POINT pt = scaleMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			fractal->setNewJuliaC(pt.x, pt.y);
			refreshJulia(hWnd);
			refreshGradient(hWnd);
		}	
		break;

		case WM_MOUSEMOVE: {

			if (isPanning && (wParam & MK_MBUTTON)) {
				POINT pt = scaleMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				fractal->pan(lastPanX, lastPanY, pt.x, pt.y);  
				refreshFractal(hWnd, pt.x);

				lastPanX = pt.x;
				lastPanY = pt.y;
			}

			if ((wParam & MK_LBUTTON) || holdlessJulia) {
				POINT pt = scaleMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				fractal->setNewJuliaC(pt.x, pt.y);
				refreshJulia(hWnd);
				refreshGradient(hWnd);
			}
		}
		break;

		case WM_MBUTTONDOWN: {
			POINT pt = scaleMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			lastPanX = pt.x;
			lastPanY = pt.y;
			isPanning = true;
		}
		break;

		// Middle mouse button sets new center
		case WM_MBUTTONUP: {
			isPanning = false;
		}
		break;

		// If Shift + scroll, change gradient size
		// Else zoom in/out
		case WM_MOUSEWHEEL: {
			double wDelta = GET_WHEEL_DELTA_WPARAM(wParam),
				   notches = wDelta / WHEEL_DELTA;

			// If shift is held, change gradient size
			if (wParam & MK_SHIFT) {
				fractal->changeGradientSize(notches);
				refreshGradient(hWnd);
				return 0;
			}
			// Else zoom in/out
			else {
				double zoomPercent = -0.334, // Percent per notch
					zoomFactor = 1 + (notches * zoomPercent);
				POINT pt = scaleMouse(hWnd, lParam);
				fractal->zoomInOut(pt.x, pt.y, zoomFactor);
				refreshFractal(hWnd, pt.x);
			}
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}

// Refresh only the Mandelbrot side
inline void refreshMandelbrot(HWND hWnd) {

	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x < width / 2) {
				iterations[y * width + x] = fractal->computePixel(x, y);
			}
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);
}
// Refresh only the Julia side
inline void refreshJulia(HWND hWnd) {

	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x >= width / 2) {
				iterations[y * width + x] = fractal->computePixel(x, y);
			}
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);
}
// Refresh both sides
inline void refreshBoth(HWND hWnd) {
	refreshMandelbrot(hWnd);
	refreshJulia(hWnd);
}
// Refresh one side based on x position
inline void refreshFractal(HWND hWnd, int x) {
	if (width / 2 > x) refreshMandelbrot(hWnd);
	else refreshJulia(hWnd);
	refreshGradient(hWnd);
}
// Refresh the gradient colors
inline void refreshGradient(HWND hWnd) {

	#pragma omp parallel for schedule(dynamic)
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			pixels[y * width + x] = fractal->applyGradient(iterations[y * width + x]);
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);

}
// Refresh bth sides and gradient
inline void refreshScreen(HWND hWnd) {
	refreshBoth(hWnd);
	refreshGradient(hWnd);
}

// Scale mouse coordinates from client size to fractal size
inline POINT scaleMouse(HWND hWnd, LPARAM lParam) {
	POINT p;

	p.x = GET_X_LPARAM(lParam);
	p.y = GET_Y_LPARAM(lParam);

	ScreenToClient(hWnd, &p);

	p.x = p.x * width / clientWidth;
	p.y = p.y * height / clientHeight;

	return p;
}
inline POINT scaleMouse(int x, int y) {
	POINT p;

	p.x = x * width / clientWidth;
	p.y = y * height / clientHeight;

	return p;
}

