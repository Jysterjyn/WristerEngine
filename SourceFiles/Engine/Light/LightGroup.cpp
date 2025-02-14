#include "LightGroup.h"
#include "D3D12Common.h"
using namespace WristerEngine;

void LightGroup::TransferConstBuffer()
{
	constMap->ambientColor = ambientColor;
	// 平行光源
	for (int i = 0; i < DIR_LIGHT_NUM; i++)
	{
		// ライトが有効なら設定を転送	
		if (dirLights[i].IsActive())
		{
			constMap->dirLights[i].active = true;
			constMap->dirLights[i].lightv = -dirLights[i].GetDir();
			constMap->dirLights[i].lightcolor = dirLights[i].GetColor();
		}
		// ライトが無効なら転送しない
		else { constMap->dirLights[i].active = false; }
	}
	// 点光源
	for (int i = 0; i < POINT_LIGHT_NUM; i++)
	{
		if (pointLights[i].IsActive())
		{
			constMap->pointLights[i].active = true;
			constMap->pointLights[i].lightpos = pointLights[i].GetPos();
			constMap->pointLights[i].lightcolor = pointLights[i].GetColor();
			constMap->pointLights[i].lightAtten = pointLights[i].GetAtten();
		}
		else { constMap->pointLights[i].active = false; }
	}
	// スポットライト
	for (int i = 0; i < SPOT_LIGHT_NUM; i++)
	{
		if (spotLights[i].IsActive())
		{
			constMap->spotLights[i].active = true;
			constMap->spotLights[i].lightv = -spotLights[i].GetDir();
			constMap->spotLights[i].lightpos = spotLights[i].GetPos();
			constMap->spotLights[i].lightcolor = spotLights[i].GetColor();
			constMap->spotLights[i].lightatten = spotLights[i].GetAtten();
			constMap->spotLights[i].lightfactoranglecos = spotLights[i].GetFactorAngleCos();
		}
		else { constMap->spotLights[i].active = false; }
	}
	// 丸影
	for (int i = 0; i < CIRCLE_SHADOW_NUM; i++)
	{
		if (circleShadows[i].IsActive())
		{
			constMap->circleShadows[i].active = true;
			constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
			constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
			constMap->circleShadows[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
			constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
			constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
		}
		else { constMap->circleShadows[i].active = false; }
	}
}

std::unique_ptr<LightGroup> LightGroup::Create()
{
	std::unique_ptr<LightGroup> instance = std::make_unique<LightGroup>();
	instance->Initialize();
	return instance;
}

void LightGroup::Initialize()
{
	DefaultLightSetting();
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

DirectionalLight* LightGroup::GetDirectionalLight(uint32_t index)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	return &dirLights[index];
}

PointLight* LightGroup::GetPointLight(uint32_t index)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);
	return &pointLights[index];
}

SpotLight* LightGroup::GetSpotLight(uint32_t index)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);
	return &spotLights[index];
}

CircleShadow* LightGroup::GetCircleShadow(uint32_t index)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);
	return &circleShadows[index];
}

void LightGroup::DefaultLightSetting()
{
	for (DirectionalLight& dirLight : dirLights)
	{
		dirLight.SetActive(true);
		dirLight.SetColor({ 1,1,1 });
	}
	dirLights[0].SetDir({ 0,-1,0 });
	dirLights[1].SetDir({ 0.5f,0.1f,0.2f });
	dirLights[2].SetDir({ -0.5f,0.1f,-0.2f });
}