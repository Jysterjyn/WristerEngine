#include "PostEffectFunctions.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    // 輝度を変更
    texcolor = ChangeBrightness(texcolor, brightness);
    // UVずらし
    texcolor = ChangeUV(input, uvOffset);
    
    switch (effectType)
    {
        case 1: // 色反転
            texcolor = ColorFlip(texcolor);
            break;
        case 2: // グレースケール
            texcolor = GrayScale(texcolor);
            break;
        case 3: // 高輝度抽出
            texcolor = GetHighLumi(input);
            break;
        case 4: // 平均ぼかし
            texcolor = Blur(input, 3);
            break;
        case 5: // ガウシアンブラー
            texcolor = GaussianBlur(input);
            break;
        case 6: // 単方向のガウシアンブラー
            texcolor = GaussianBlurLinear(input);
            break;
        case 7: // クロスフィルタ
            texcolor *= CreateDotFilter(input);
            break;
        case 8: // ブルーム
            texcolor = Bloom(input);
            break;
        case 9: // 暗黒世界
            texcolor = Dark(input);
            break;
    }
    texcolor.a = 1;
    return texcolor;
}