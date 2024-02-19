#include <windows.h>
#include <complex>
#include <tchar.h>
const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 100;
// void * nullptr = 0;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = _T("MandelbrotWindow");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "MandelbrotWindow", "Mandelbrot Set Animation",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int xOffset = 0;
    static double zoom = 1.0;

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    std::complex<double> c(
                        (x - WIDTH / 2.0 + xOffset) / (0.5 * zoom * WIDTH),
                        (y - HEIGHT / 2.0) / (0.5 * zoom * HEIGHT)
                    );

                    std::complex<double> z(0, 0);
                    int iter = 0;
                    while (iter < MAX_ITER && std::abs(z) < 2.0) {
                        z = z * z + c;
                        ++iter;
                    }

                    if (iter == MAX_ITER) {
                        SetPixel(hdc, x, y, RGB(0, 0, 0));
                    } else {
                        int color = 255 - (iter * 255) / MAX_ITER;
                        SetPixel(hdc, x, y, RGB(color, color, color));
                    }
                }
            }

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN: {
            if (wParam == VK_RIGHT) {
                xOffset += 50;
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_LEFT) {
                xOffset -= 50;
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_UP) {
                zoom *= 1.1;
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_DOWN) {
                zoom /= 1.1;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
