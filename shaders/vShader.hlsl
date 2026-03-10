struct VSInput // Must match the vertex format from the .cpp
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput // Will carry data from vertex shader to pixel shader
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOutput VSMain(VSInput input) // Converts float3 pos into float4 clip-space pos and passes color
{
    VSOutput output;

    output.position = float4(input.position, 1.0f);
    output.color = input.color;

    return output;
}

float4 PSMain(VSOutput input) : SV_TARGET // Returns the interpolated color for each pixel
{
    return input.color;
}