# Week 1 - Window Setup and First Clear

Week 1 was about getting a real DirectX 11 application boot path working instead of just reading about the API in the abstract.

The goal here was not to render anything fancy. It was to prove that the project could:

- create a Win32 window
- initialize the Direct3D 11 device, swap chain, and device context
- grab the back buffer
- create a render target view
- clear the screen to a known color
- present frames to the window

## What I Built

By the end of this week, the lab could open a native window and clear the back buffer every frame. That may sound small, but it is the foundation for everything else.

Once that path works, I know:

- the window is valid
- the swap chain is valid
- the render target is valid
- the render loop is running
- `Present` is working

That removes a lot of guesswork for later weeks.

## Main Concepts

### Win32 window creation

I needed:

- a window class
- a window procedure
- a real application entry point
- a message loop that keeps the app alive and responsive

This was the part that made the project feel like an actual Windows graphics application instead of just a code sample.

### Device and swap chain creation

`D3D11CreateDeviceAndSwapChain` was the key setup call.

This gave me:

- the `ID3D11Device`
- the `ID3D11DeviceContext`
- the `IDXGISwapChain`

That is basically the core starting package for a small DirectX 11 renderer.

### Back buffer and render target view

The swap chain owns the back buffer. I had to get that back buffer and turn it into a render target view so the GPU had an actual output target to render into.

That was the first time the pipeline really started to make sense:

- the swap chain owns the image
- the render target view is how the pipeline sees that image

### Clear and present

Clearing to a solid color was the first visual proof that the pipeline was alive.

The clear color mattered because it gave me a dead simple test:

- if the screen changes to the clear color, the render path is active
- if it does not, the problem is earlier in setup

## Important API Pieces

- `RegisterClassW`
- `CreateWindowExW`
- `PeekMessage`
- `TranslateMessage`
- `DispatchMessage`
- `D3D11CreateDeviceAndSwapChain`
- `IDXGISwapChain::GetBuffer`
- `ID3D11Device::CreateRenderTargetView`
- `ID3D11DeviceContext::OMSetRenderTargets`
- `ID3D11DeviceContext::ClearRenderTargetView`
- `IDXGISwapChain::Present`

## What Mattered Most

The biggest lesson from Week 1 was that DirectX 11 is not just about graphics API calls. The Win32 side matters too.

If the message loop is wrong, the app behaves wrong.
If the back buffer setup is wrong, nothing appears.
If the render target is not bound, clears and draws go nowhere useful.

So Week 1 was really about building a stable boot sequence.

## Common Problems Worth Remembering

- building successfully does not mean the runtime setup is valid
- a locked `.exe` can stop the linker from writing the next build
- if the message loop is wrong, the app may stop rendering or stop responding
- if the render target view is invalid, the clear will not do what I expect

## Why This Week Matters

This week is the floor the rest of the lab stands on.

No vertex buffers, no shaders, no textures, no constant buffers, no engine architecture work matters if the application cannot:

- create a window
- create the device
- render to the back buffer
- show the frame on screen

That was the real point of Week 1.
