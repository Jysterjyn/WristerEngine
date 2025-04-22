#include <Primitive.hlsli>

VSOutput main(float4 pos : POSITION, float4 color : COLOR)
{
    VSOutput output;
    output.svpos = mul(matCamera, pos);
    output.color = color;
    return output;
}