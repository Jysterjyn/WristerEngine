#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float3 vel : VELOCITY, float3 acc : ACCEL,
float2 scales : SCALES, float2 nowTime : NOWTIME, uint type : TYPE)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    switch (type)
    {
        case 0:
            output.pos = pos;
            output.pos.xyz += (vel + acc * nowTime.y) * nowTime.y;
            output.scale = lerp(scales.x, scales.y, nowTime.y / nowTime.x);
            break;
        default:
            output.pos = pos;
            output.scale = scales.x;
            break;
    }
    return output;
}