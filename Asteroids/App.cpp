#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <iostream>

#include "Engine.h"
#include "App.h"
#include <chrono>

#pragma comment(lib, "d2d1")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        App app;

        if (SUCCEEDED(app.Initialize()))
        {
            app.RunMessageLoop();
        }

        CoUninitialize();
    }

    return 0;
}

App::App() : hwnd(NULL)
{
    engine = new Engine();
}

App::~App() {
}

void App::RunMessageLoop() {
    MSG msg;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int frames = 0;
    double framesTime = 0;

    boolean running = true;
    while (running)
    {

        end = std::chrono::steady_clock::now();
        double elapsed_secs = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
        begin = end;

        // Display FPS
        framesTime += elapsed_secs;
        frames++;
        if (framesTime > 1) {
            WCHAR fpsText[32];
            swprintf(fpsText, 32, L"Game: %d FPS", frames);
            SetWindowText(hwnd, fpsText);
            frames = 0;
            framesTime = 0;
        }

        // Messages and user input
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                running = false;
        }

        // Game logic
        engine->Logic(elapsed_secs);

        // Drawing
        engine->Render();
    }
}

HRESULT App::Initialize()
{
    HRESULT hr = S_OK;

    // Register the window class.
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = App::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wcex.lpszClassName = L"D2DMainApp";

    ATOM x = RegisterClassEx(&wcex);

    // Create the window.
    hwnd = CreateWindowEx(
        NULL,
        L"D2DMainApp",
        L"Game",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        RESOLUTION_X,
        RESOLUTION_Y,
        NULL,
        NULL,
        HINST_THISCOMPONENT,
        this
    );
    hr = hwnd ? S_OK : E_FAIL;

    // Adjust it so the client area is RESOLUTION_X/RESOLUTION_Y
    RECT rect1;
    GetWindowRect(hwnd, &rect1);
    RECT rect2;
    GetClientRect(hwnd, &rect2);

    SetWindowPos(
        hwnd,
        NULL,
        rect1.left,
        rect1.top,
        RESOLUTION_X + ((rect1.right - rect1.left) - (rect2.right - rect2.left)),
        RESOLUTION_Y + ((rect1.bottom - rect1.top) - (rect2.bottom - rect2.top)),
        NULL
    );

    if (SUCCEEDED(hr))
    {
        engine->InitializeD2D(hwnd);

        ShowWindow(hwnd, SW_SHOWNORMAL);
        UpdateWindow(hwnd);
    }

    return hr;
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        App* pApp = (App*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pApp)
        );

        result = 1;
    }
    else
    {
        App* pApp = reinterpret_cast<App*>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
            )));

        bool wasHandled = false;

        if (pApp)
        {
            switch (message)
            {

            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_KEYDOWN:
            {
                pApp->engine->KeyDown(wParam);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_KEYUP:
            {
                pApp->engine->KeyUp(wParam);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_MOUSEMOVE:
            {
                //pApp->engine->MousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_LBUTTONUP:
            {
                //pApp->engine->MouseButtonUp(true, false);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_LBUTTONDOWN:
            {
                //pApp->engine->MouseButtonDown(true, false);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_RBUTTONUP:
            {
                //pApp->engine->MouseButtonUp(false, true);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_RBUTTONDOWN:
            {
                //pApp->engine->MouseButtonDown(false, true);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            result = 1;
            wasHandled = true;
            break;
            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}