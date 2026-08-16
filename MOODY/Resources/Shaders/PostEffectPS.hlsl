#include "PostEffectFunctions.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    // UV・輝度の調整
    float4 texcolor = float4(tex.Sample(smp, input.uv + uvOffset).rgb * brightness, 1);
    
    //float4 texcolor = Sharpen(input);
    
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
    
    if (length(texcolor.rgb) == 0)
    {
        // 黒の場合は透過する
        texcolor.a = 0;
    }
    else
    {
        texcolor.a = 1;
    }
    return texcolor;
}