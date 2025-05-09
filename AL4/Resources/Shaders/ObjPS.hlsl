#include "Obj.hlsli"
#include "Functions.hlsli"

Texture2D<float4> mainTex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> subTex : register(t1); // 1番スロットに設定されたテクスチャ
Texture2D<float> blendMask : register(t2); // 2番スロットに設定されたテクスチャ
Texture2D<float> specularMask : register(t3); // 3番スロットに設定されたテクスチャ
Texture2D<float> dissolveMask : register(t4); // 4番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
    float4 mainTexColor = mainTex.Sample(smp, texTrans[0].GetUV(input.uv)) * color[0];
    float4 subTexColor = subTex.Sample(smp, texTrans[1].GetUV(input.uv)) * color[1];
	// 光計算関数へ送るデータ
    LightData lightData;
    // マスク値の取得
    float blendMaskVal = blendMask.Sample(smp, texTrans[2].GetUV(input.uv)) * maskPow.x;
    lightData.specularMaskVal = specularMask.Sample(smp, texTrans[3].GetUV(input.uv)) * maskPow.y;
    float dissolveMaskVal = dissolveMask.Sample(smp, input.uv);
    // ディゾルブを行う
    clip(dissolveMaskVal - maskPow.z);
    // テクスチャブレンド
    float4 texcolor = lerp(mainTexColor, subTexColor, blendMaskVal);
    // 光計算関数へ送るデータの代入
    lightData.normal = input.normal;
    lightData.worldpos = input.worldpos.xyz;
    lightData.eyedir = normalize(cameraPos - input.worldpos.xyz); // 頂点から視点への方向ベクトル
    lightData.shininess = lightGroup.shininess; // 光沢度
	// 環境光
    float3 ambient = material.ambient.rgb  * lightGroup.ambientColor;
	// シェーディングによる色
    float4 shadecolor = float4(ambient, material.ambient.a);
    shadecolor.rgb += lightGroup.ComputeLightEffect(lightData, material);
    return shadecolor * texcolor;
}