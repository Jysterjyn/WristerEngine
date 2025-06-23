#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : SCALE, float4 color : COLOR)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    //output.pos = pos;
    //output.pos.xyz += (vel + acc * nowTime.y) * nowTime.y;
    //output.scale = lerp(scales.x, scales.y, nowTime.y / nowTime.x);
    output.pos = pos;
    output.scale = scale;
    output.color = color;
    return output;
}