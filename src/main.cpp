#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <iostream>


IDXGISwapChain* gSwapChain = nullptr;
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gContext = nullptr;
ID3D11RenderTargetView* gRTV = nullptr;
ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11InputLayout* gInputLayout = nullptr;

// Vertex format!
struct Vertice {
    float pos[3] = {}; // x, y, z
    float color[4] = {}; // rgba
} ;

Vertice tri_verts[3] = {
    {{0.0f, 0.5f, 0.0f},{1.0f,1.0f,1.0f,1.0f}},
    {{0.5f, -0.5f, 0.0f},{1.0f,1.0f,1.0f,1.0f}},
    {{-0.5f, -0.5f, 0.0f},{1.0f,1.0f,1.0f,1.0f}}
};

// Window procedure callback.
// Parameters:
// - hwnd: Handle to the window receiving the message.
// - msg: The message ID (WM_DESTROY, WM_PAINT, etc).
// - wParam: Message-specific additional data.
// - lParam: Message-specific additional data.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
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
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
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
    if (!hwnd) return 0;
    // -------------------------- //

    // ---- D3D Initialization ----
    // Set up the Viewport
    D3D11_VIEWPORT viewport = { 0 };
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = 1280.0f;
    viewport.Height = 720.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    // Set up the D3D Swap Chain Descriptor
    DXGI_SWAP_CHAIN_DESC desc = { 0 };
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
    if (FAILED(hr)) return -1;

    // Create the vertex buffer
    D3D11_BUFFER_DESC vBufDesc = { 0 };
    vBufDesc.ByteWidth = sizeof(tri_verts);
    vBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBufDesc.CPUAccessFlags = 0;
    vBufDesc.MiscFlags = 0;
    vBufDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subdata = { 0 };
    subdata.pSysMem = tri_verts;
    subdata.SysMemPitch = 0;
    subdata.SysMemSlicePitch = 0;

    HRESULT hr2 = gDevice->CreateBuffer(&vBufDesc, &subdata, &gVertexBuffer);
    if (FAILED(hr2)) return -1;

    // Compile From File!
    ID3DBlob *vsBlob = nullptr;
    ID3DBlob *psBlob = nullptr;
    ID3DBlob *errorBlob = nullptr;
    HRESULT compileResult;
    // Vertex Shader Compile
    compileResult= D3DCompileFromFile(
        L"shaders\\vShader.hlsl", // pFileName
        nullptr, // pDefines
        nullptr, // pIncludes
        "VSMain", // pEntryPoint
        "vs_5_0", // pTarget
        0, // Flags1
        0, // Flags2
        &vsBlob, // ppCode, pass a blob
        &errorBlob
        );
    if (FAILED(compileResult)) return -1;

    if (errorBlob)
    {
        errorBlob->Release();
        errorBlob = nullptr;
    }

    // Pixel Shader Compile
    compileResult = D3DCompileFromFile(
        L"shaders\\vShader.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        0,
        0,
        &psBlob,
        &errorBlob
    );
    if (FAILED(compileResult)) return -1;

    if (errorBlob)
    {
        errorBlob->Release();
        errorBlob = nullptr;
    }

    // Create runtime shader objects from the compiled HLSL bytecode.
    hr = gDevice->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &gVertexShader
    );
    if (FAILED(hr)) return -1;

    hr = gDevice->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &gPixelShader
    );
    if (FAILED(hr)) return -1;

    // Bind everything in the render path!


    // Set up the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {
            "POSITION",                      // SemanticName, MUST MATCH semantic on shader file, i.e. POSITION
            0,                               // SemanticIndex, 0 unless we have texcoords
            DXGI_FORMAT_R32G32B32_FLOAT,     // 3 floats, must match the data in the buffer
            0,                               // InputSlot, only one buffer currently so 0
            0,                               // AlignedByteOffset, where the field will start inside one Vertice
            D3D11_INPUT_PER_VERTEX_DATA,     // InputSlotClass, normal per-vertex data, not instancing
            0                                // InstanceDataStepRate, 0 there is no instancing
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,  // 4 floats
            0,
            12,                              // starts after 3 floats = 12 bytes
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };
    // Creat the input layout
    hr = gDevice->CreateInputLayout(
        layout,
        2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &gInputLayout
    );
    if (FAILED(hr)) return -1;

    // Get Back Buffer
    ID3D11Texture2D* backBuffer = nullptr;
    hr = gSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr) || backBuffer == nullptr) return -1;

    // Create render target view
    hr = gDevice->CreateRenderTargetView(backBuffer, nullptr, &gRTV);
    backBuffer->Release();
    if (FAILED(hr)) return -1;
    // Bind the RTV, once or after resize
    gContext->OMSetRenderTargets(1, &gRTV, nullptr);
    // ------------------------ //

    // Set Viewport
    gContext->RSSetViewports(1, &viewport);

    // Set up the vertex buffers
    UINT stride = sizeof(Vertice);
    UINT offset = 0;
    gContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &stride, &offset);
    gContext->IASetInputLayout(gInputLayout);
    gContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Set the shaders
    gContext->VSSetShader(gVertexShader, nullptr, 0);
    gContext->PSSetShader(gPixelShader, nullptr, 0);

    // ---- Render Loop ----
    float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    ShowWindow(hwnd, nCmdShow);
    bool running = true;
    MSG msg{};
    while (running)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                running = false;
        }
        else // Render
        {
            gContext->ClearRenderTargetView(gRTV, clearColor);

            // Draw
            gContext->Draw(3, 0);

            gSwapChain->Present(1, 0); // 1 = Vsync
        }
    }

    return 0;
}
