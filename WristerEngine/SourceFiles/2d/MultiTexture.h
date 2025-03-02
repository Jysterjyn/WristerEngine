#pragma once
#include "PostEffect.h"

namespace WristerEngine::_2D
{
	class MultiTexture : DXCommonGetter
	{
	private:
		struct ConstBufferData
		{
			UINT32 effectType = 0;
		};

		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::array<Vertex, 4> vertices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipelineState;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap = nullptr;

		void CreateBuffers();
	public:
		void Initialize();
		// 2�̃e�N�X�`������������
		void Draw(std::array<PostEffect, 2>& postEffects);
		void SetEffectType(UINT32 effectType) { constMap->effectType = effectType; }
	};
}