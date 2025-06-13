cbuffer cbuff0 : register(b0)
{
    uint effectType; // 掛けるポストエフェクトの種類
    float angle; // ぼかす角度(ラジアン)
    float brightness; // 輝度
    float2 uvOffset; // UVずらし
    float2 spotlightCenterUV; // ライトの中心位置(UV)
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};