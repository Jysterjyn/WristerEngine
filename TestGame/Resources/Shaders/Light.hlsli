struct DirLight
{
    float3 lightv; // ライトへの方向の単位ベクトル
    float3 lightcolor; // ライトの色(RGB)
    uint active;
};

struct PointLight
{
    float3 lightpos; // ライト座標
    float3 lightcolor; // ライトの色(RGB)
    float3 lightatten; // ライト距離減衰係数
    uint active;
};

struct SpotLight
{
    float3 lightv; // ライトの光線方向の逆ベクトル
    float3 lightpos; // ライト座標
    float3 lightcolor; // ライトの色(RGB)
    float3 lightatten; // ライト距離減衰係数
    float2 lightfactoranglecos; // ライト減衰角度のcos
    uint active;
};

struct CircleShadow
{
    float3 dir; // 投影方向の逆ベクトル(単位ベクトル)
    float3 casterPos; // キャスター座標
    float distanceCasterLight; // キャスターとライトの距離
    float3 atten; // 距離減衰係数
    float2 factorAngleCos; // 減衰角度のcos
    uint active;
};

static const uint DIRLIGHT_NUM = 3;
static const uint POINTLIGHT_NUM = 3;
static const uint SPOTLIGHT_NUM = 3;
static const uint CIRCLESHADOW_NUM = 3;

struct Material
{
    float3 ambient; // アンビエント係数
    float3 diffuse; // ディフューズ係数
    float3 specular; // スペキュラー係数
};

struct LightData
{
    float3 normal;
    float3 worldpos;
    float3 eyedir;
    float shininess;
    float3 texcolor;
    float specularMaskVal;
};

struct LightGroup
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
    SpotLight spotLights[SPOTLIGHT_NUM];
    CircleShadow circleShadows[CIRCLESHADOW_NUM];

    float3 ComputeDirLight(LightData lightData, Material material);
    float3 ComputePointLight(LightData lightData, Material material);
    float3 ComputeSpotLight(LightData lightData, Material material);
    float3 ComputeCircleShadow(float3 worldpos, Material material);
    float3 ComputeLightEffect(LightData lightData, Material material);
};

float3 ComputeDiffuseSpecular(float3 lightv, LightData lightData, Material material)
{
    // ライトに向かうベクトルと法線の内積
    float3 dotlightnormal = dot(lightv, lightData.normal);
    // 反射光ベクトル
    float3 reflect = normalize(-lightv + 2 * dotlightnormal * lightData.normal);
	// 拡散反射光
    float3 diffuse = saturate(dotlightnormal * material.diffuse);
	// 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, lightData.eyedir)), lightData.shininess) * material.specular;
    return diffuse * lightData.texcolor + specular * lightData.specularMaskVal;
}

float3 LightGroup::ComputeDirLight(LightData lightData, Material material)
{
    float3 sumColor;
    for (uint i = 0; i < DIRLIGHT_NUM; i++)
    {
        DirLight dirLight = dirLights[i];
        if (!dirLight.active)
        {
            continue;
        }
		// 全て加算する
        sumColor += ComputeDiffuseSpecular(dirLight.lightv, lightData, material) * dirLight.lightcolor;
    }
    return sumColor;
}

float3 LightGroup::ComputePointLight(LightData lightData, Material material)
{
    float3 sumColor;
    // 点光源
    for (uint i = 0; i < POINTLIGHT_NUM; i++)
    {
        PointLight pointLight = pointLights[i];
        if (!pointLight.active)
        {
            continue;
        }
		// ライトへのベクトル
        float3 lightv = pointLight.lightpos - lightData.worldpos;
		// ベクトルの長さ
        float d = length(lightv);
		// 正規化し、単位ベクトルにする
        lightv = normalize(lightv);
		// 距離減衰係数
        float atten = 1.0f / (pointLight.lightatten.x + pointLight.lightatten.y * d + pointLight.lightatten.z * d * d);
		// 全て加算する
        sumColor += atten * ComputeDiffuseSpecular(lightv, lightData, material) * pointLight.lightcolor;
    }
    return sumColor;
}

float3 LightGroup::ComputeSpotLight(LightData lightData, Material material)
{
    float3 sumColor;
    for (uint i = 0; i < SPOTLIGHT_NUM; i++)
    {
        SpotLight spotLight = spotLights[i];
        if (!spotLight.active)
        {
            continue;
        }
		// ライトへの方向ベクトル
        float3 lightv = spotLight.lightpos - lightData.worldpos;
        float d = length(lightv);
        lightv = normalize(lightv);
		// 距離減衰係数
        float atten = saturate(1.0f / (spotLight.lightatten.x + spotLight.lightatten.y * d + spotLight.lightatten.z * d * d));
		// 角度減衰
        float cos = dot(lightv, spotLight.lightv);
		// 減衰開始角度から、減衰終了角度にかけて減衰
		// 減衰開始角度の内側は1倍、減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(spotLight.lightfactoranglecos.y, spotLight.lightfactoranglecos.x, cos);
		// 角度減衰を乗算
        atten *= angleatten;
		// 全て加算する
        sumColor += atten * ComputeDiffuseSpecular(lightv, lightData, material) * spotLight.lightcolor;
    }
    return sumColor;
}

float3 LightGroup::ComputeCircleShadow(float3 worldpos, Material material)
{
    float3 sumColor = float3(0, 0, 0);
    for (uint i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        CircleShadow circleShadow = circleShadows[i];
        if (!circleShadow.active)
        {
            continue;
        }
		// オブジェクト表面からキャスターへのベクトル
        float3 casterv = circleShadow.casterPos - worldpos;
		// 投影方向での距離
        float d = dot(casterv, circleShadow.dir);
		// 距離減衰係数
        float atten = saturate(1.0f / (circleShadow.atten.x + circleShadow.atten.y * d + circleShadow.atten.z * d * d));
		// 距離がマイナスなら0にする
        atten *= step(0, d);
		// 仮想ライトの座標
        float3 lightpos = circleShadow.casterPos + circleShadow.dir * circleShadow.distanceCasterLight;
		// オブジェクト表面からライトへのベクトル(単位ベクトル)
        float3 lightv = normalize(lightpos - worldpos);
		// 角度減衰
        float cos = dot(lightv, circleShadow.dir);
		// 減衰開始角度から、減衰終了角度にかけて減衰
		// 減衰開始角度の内側は1倍、減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(circleShadow.factorAngleCos.y, circleShadow.factorAngleCos.x, cos);
		// 角度減衰を乗算
        atten *= angleatten;
		// 全て減算する
        sumColor -= atten;
    }
    return sumColor;
}

// ライト計算
float3 LightGroup::ComputeLightEffect(LightData lightData, Material material)
{
	// 平行光源
    float3 sumColor = ComputeDirLight(lightData, material);
    // 点光源
    sumColor += ComputePointLight(lightData, material);
	// スポットライト
    sumColor += ComputeSpotLight(lightData, material);
	// 丸影
    sumColor += ComputeCircleShadow(lightData.worldpos, material);
    return sumColor;
}