#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : TEXCOORD, float3 vel : VELOCITY,
float3 acc : ACCEL, float2 scales : SCALES, float2 nowTime : NOWTIME)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.pos = pos;
    output.pos.xyz += (vel + acc * nowTime.y) * nowTime.y;
    output.scale = lerp(scales.x, scales.y, nowTime.y / nowTime.x);
    return output;
}