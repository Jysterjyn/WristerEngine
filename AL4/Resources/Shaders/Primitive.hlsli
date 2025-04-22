cbuffer ConstBufferData : register(b0)
{
    matrix matCamera;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
};