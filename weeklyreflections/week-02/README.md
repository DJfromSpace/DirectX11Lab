# Week 2 - Vertex Buffers, Shaders, and First Triangle

Week 2 was the first real step into the graphics pipeline itself.

The goals for this week were:

- understand vertex buffers
- understand input layouts
- write a basic vertex shader
- write a basic pixel shader
- configure the pipeline
- draw a triangle

This was the week where the lab stopped being just "clear the screen" and started becoming actual rendering.

## What I Built

By the end of this week, the project could take vertex data defined in C++, move it into a GPU vertex buffer, feed that data into HLSL shaders, and draw a triangle to the screen.

That required building the full minimum path:

- CPU-side vertex data
- GPU vertex buffer
- HLSL shader file
- shader compilation
- runtime shader creation
- input layout creation
- viewport setup
- pipeline binding
- `Draw(3, 0)`

## The Main Mental Model

The clean way I think about Week 2 is this:

1. I define triangle vertex data on the CPU.
2. `CreateBuffer` uploads that data into a GPU vertex buffer.
3. The input layout explains how one C++ vertex maps to the vertex shader input.
4. The vertex shader runs once per vertex.
5. The rasterizer turns that triangle into covered pixels.
6. The pixel shader decides the final pixel color.
7. `Present` shows the result on screen.

That was the first moment where the Direct3D 11 pipeline started to feel concrete instead of theoretical.

## What Each Piece Did

### Vertex buffer

The vertex buffer is the actual triangle data stored in GPU memory.

I used:

- `D3D11_BUFFER_DESC`
- `D3D11_SUBRESOURCE_DATA`
- `ID3D11Device::CreateBuffer`

The important distinction I wanted to keep clear was:

- `ByteWidth` = size of the whole buffer
- `stride` = size of one vertex
- `offset` = where reading starts in the buffer

### HLSL shaders

This week also introduced the first shader file.

The vertex shader:

- takes in `POSITION` and `COLOR`
- outputs `SV_POSITION`
- passes color through

The pixel shader:

- receives the interpolated color
- returns it as `SV_TARGET`

That made the roles of the two stages very easy to see.

### Input layout

The input layout was one of the most important parts to understand.

It is the contract between:

- the C++ vertex struct
- the vertex buffer memory
- the HLSL vertex shader input

If that mapping is wrong, the shader reads garbage even when the program builds fine.

### Viewport

The viewport tells the rasterizer how clip-space output maps onto the actual render target area.

That was another useful reminder that a successful build does not mean the pipeline is configured correctly.

### Draw call

`Draw(3, 0)` was the final step that made everything visible.

By that point, all of the earlier setup had to already be correct:

- render target bound
- viewport set
- vertex buffer bound
- input layout bound
- topology set
- shaders bound

The draw call is simple. Getting to the point where it can do something useful is the real work.

## Important API Pieces

- `ID3D11Device::CreateBuffer`
- `D3DCompileFromFile`
- `ID3D11Device::CreateVertexShader`
- `ID3D11Device::CreatePixelShader`
- `ID3D11Device::CreateInputLayout`
- `ID3D11DeviceContext::RSSetViewports`
- `ID3D11DeviceContext::IASetVertexBuffers`
- `ID3D11DeviceContext::IASetInputLayout`
- `ID3D11DeviceContext::IASetPrimitiveTopology`
- `ID3D11DeviceContext::VSSetShader`
- `ID3D11DeviceContext::PSSetShader`
- `ID3D11DeviceContext::Draw`

## Problems That Actually Taught Me Something

The biggest useful bug this week was the triangle not showing even though the build succeeded and the background clear was working.

That was important because it proved something:

- the window was fine
- the swap chain was fine
- the render target was fine
- the render loop was fine

So the problem had to be in the actual draw setup.

The issue turned out to be triangle winding order and back-face culling. That was a good lesson because it showed that even when the data and shaders are valid, rasterizer rules still matter.

## What I Understand Better Now

After Week 2, I understand a lot more clearly how data moves through the Direct3D 11 pipeline:

- vertex data starts on the CPU
- gets uploaded into a GPU buffer
- gets interpreted by the input layout
- runs through the vertex shader
- gets rasterized
- gets shaded per pixel
- gets written to the render target
- gets shown by the swap chain

That is the first real rendering path, and it is the base I need before moving into constant buffers, transforms, textures, and more engine-style rendering structure.

## Why This Week Matters

Week 1 proved the app and render target were alive.
Week 2 proved the graphics pipeline itself was alive.

That is a major step because drawing a triangle is not really about the triangle. It is about verifying that the core rendering path works from CPU data all the way to the screen.
