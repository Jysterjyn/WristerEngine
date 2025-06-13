#include "PostEffect.hlsli"
SamplerState smp : register(s0);
Texture2D<float4> tex : register(t0);

float4 ChangeBrightness(float4 col, float brightness)
{
    return float4(col.rgb * brightness, 1);
}

float4 ColorFlip(float4 col)
{
    return float4(float3(1, 1, 1) - col.rgb, 1);
}

float4 ChangeUV(VSOutput i, float2 uvOffset)
{
    return float4(tex.Sample(smp, i.uv + uvOffset).rgb, 1);
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float GrayScale(float4 col)
{
    return col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
}

float4 GetHighLumi(VSOutput i)
{
    float4 col = tex.Sample(smp, i.uv);
    float grayScale = GrayScale(col);
    float extract = smoothstep(0.6, 0.9, grayScale);
    return col * extract;
}

float4 Blur(VSOutput i, float blurRange)
{
    float4 col = float4(0, 0, 0, 0);
    float num = 0;
    float2 texSize;
    tex.GetDimensions(texSize.x, texSize.y);

    for (float py = -blurRange; py <= blurRange; py++)
    {
        for (float px = -blurRange; px <= blurRange; px++)
        {
            float2 pickUV = i.uv + float2(px / texSize.x, py / texSize.y);
            col += tex.Sample(smp, pickUV);
            num++;
        }
    }
    col.rgb /= num; // かけた「重み」分、結果から割る
    col.a = 1;
    return col;
}

float4 GaussianBlur(VSOutput i)
{
    float totalWeight = 0, sigma = 0.005, stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 0);

    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
            float2 pickUV = i.uv + float2(px, py);
            float weight = Gaussian(i.uv, pickUV, sigma);
			// Gaussianで取得した「重み」を色にかける
            col += tex.Sample(smp, pickUV) * weight;
			// かけた「重み」の合計値を控えておく
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight; // かけた「重み」分、結果から割る
    col.a = 1;
    return col;
}

// 直線のガウシアンブラー
float4 GaussianBlurLinear(VSOutput i)
{
    float totalWeight = 0;
    float4 color = float4(0, 0, 0, 0);
    float pickRange = 0.06; // ガウス関数式でいうσ

    // 直線なのでfor文は一つ
    for (float j = -pickRange; j <= pickRange; j += 0.005)
    {
        float x = cos(angle) * j; // 角度から座標を指定
        float y = sin(angle) * j;
        float2 pickUV = i.uv + float2(x, y); // 色取得する座標
        // 自作のガウス関数で計算
        float weight = Gaussian(i.uv, pickUV, pickRange);
		// 取得する色にweightを掛ける
        color += tex.Sample(smp, pickUV) * weight;
		// 掛けるweightの合計値を控えておく
        totalWeight += weight;
    }
    color /= totalWeight; // 足し合わせた色をweightの合計値で割る
    return color;
}

float4 CreateDotFilter(VSOutput i)
{
    float2 texSize = float2(0, 0);
    float level = 0;
    tex.GetDimensions(0, texSize.x, texSize.y, level);
    float2 st = i.uv / texSize.x * 20;
    st = frac(st * texSize.xy);
    float l = distance(st, float2(0.5, 0.5));
    float4 col = float4(1, 1, 1, 1) * 1 - step(0.3, l);
    col.a = 1.0;
    return col;
}

float4 Bloom(VSOutput i)
{
    float4 highLumi = GetHighLumi(i); // 高輝度抽出
    float4 blur = GaussianBlur(i); // ぼかし
    return tex.Sample(smp, i.uv) + highLumi + blur; // テクスチャ合成
}

float4 Dark(VSOutput i)
{
    float4 texcolor = tex.Sample(smp, i.uv);
    
    if (pow(i.uv.x - spotlightCenterUV.x, 2) * 1280 / 720 + 
        pow(i.uv.y - spotlightCenterUV.y, 2) / 1280 * 720 >= 0.05)
    {
        return float4(0, 0, 0, 0);
    }
    return texcolor;
}