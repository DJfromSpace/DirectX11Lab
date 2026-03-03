#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <iostream>


IDXGISwapChain *gSwapChain = nullptr;
ID3D11Device *gDevice = nullptr;
ID3D11DeviceContext *gContext = nullptr;
ID3D11RenderTargetView * gRTV = nullptr;

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
    // ---- Create Win32 Window ---- //
    const wchar_t CLASS_NAME[] = L"DirectX11LabWindowClass";
    std::cout << "Hello DirectX" << std::endl;

    // Create the Window Class
    WNDCLASSW wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    // Register the Class
    RegisterClassW(&wc);

    // Create the Window
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
    if(!hwnd) return 0;
    // -------------------------- //

    // ---- D3D Initialization ----
    // Set up the D3D Swap Chain Descriptor
    DXGI_SWAP_CHAIN_DESC desc = {0};
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 1; // Simple Week 1 Setup
    desc.OutputWindow = hwnd;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    // Setup the D3D Swap Chain //
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, //default adapter
        D3D_DRIVER_TYPE_HARDWARE, // Use Hardware Accel
        nullptr, // No software raterizer
        0, // No speical flags, but use DEBUG in dev
        nullptr, // default feature levels
        0, 
        D3D11_SDK_VERSION,
        &desc,
        &gSwapChain,
        &gDevice,
        &featureLevel,
        &gContext
    );
    if(hr != 0) return -1;
    // Get Back Buffer
    ID3D11Texture2D *backBuffer = nullptr;
    gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    // Create render target view
    gDevice->CreateRenderTargetView(backBuffer, nullptr, &gRTV);
    // Bind the RTV, once or after resize
    gContext->OMSetRenderTargets(1, &gRTV, nullptr);
    // ------------------------ //

    // ---- Render Loop ----
    float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    ShowWindow(hwnd, nCmdShow);
    bool running = true;
    MSG msg{};
    while(running)
    {
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // Render
            gContext->ClearRenderTargetView(gRTV, clearColor);
            gSwapChain->Present(1, 0); // 1 = Vsync
        }
    }

    return 0;
}
