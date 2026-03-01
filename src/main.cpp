#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <iostream>

// Window procedure callback.
// Parameters:
// - hwnd: Handle to the window receiving the message.
// - msg: The message ID (WM_DESTROY, WM_PAINT, etc).
// - wParam: Message-specific additional data.
// - lParam: Message-specific additional data.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    // Use the wide default procedure to match CreateWindowExW/RegisterClassW.
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Win32 application entry point.
// Parameters:
// - hInstance: Handle to this executable module instance.
// - hPrevInstance: Previous instance handle (unused today).
// - lpCmdLine: Raw command-line string (unused here).
// - nCmdShow: Initial show state for the window.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    // Step 1: Define a unique class name for this window type.
    const wchar_t CLASS_NAME[] = L"DirectX11LabWindowClass";
    std::cout << "Hello DirectX" << std::endl;

    // Step 2: Describe the window class (callback, owner instance, class name).
    WNDCLASSW wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    // Step 3: Register the class with Windows so we can create windows from it.
    RegisterClassW(&wc);

    // Step 4: Create a window instance using the registered class.
    HWND hwnd = CreateWindowExW(
        0, // dwExStyle: extended style flags.
        CLASS_NAME, // lpClassName: registered window class name.
        L"Direct X 11 Lab Window C++", // lpWindowName: title bar text.
        WS_OVERLAPPEDWINDOW, // dwStyle: standard top-level window style.
        CW_USEDEFAULT, // X: default horizontal position.
        CW_USEDEFAULT, // Y: default vertical position.
        1280, // nWidth: window width.
        720, // nHeight: window height.
        nullptr, // hWndParent: no parent for top-level window.
        nullptr, // hMenu: no menu.
        hInstance, // hInstance: module instance.
        nullptr // lpParam: no extra creation data.
    );

    // Step 5: Abort if window creation failed.
    if(!hwnd) return 0;

    // Step 6: Make the created window visible on screen.
    ShowWindow(hwnd, nCmdShow);

    // Step 7: Run the message loop so the window can process events.
    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        // GetMessage parameters:
        // - &msg: Receives the next message from the queue.
        // - nullptr: Read messages for the current thread (not a specific window only).
        // - 0: Minimum message filter value (no lower bound).
        // - 0: Maximum message filter value (no upper bound).

        // TranslateMessage parameters:
        // - &msg: Message to translate (for example key down -> character message).
        TranslateMessage(&msg);

        // DispatchMessage parameters:
        // - &msg: Message to send to the window procedure (WindowProc).
        DispatchMessage(&msg);
    }

    return 0;
}
