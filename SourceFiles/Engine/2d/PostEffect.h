#pragma once
#include <array>
#include "DirectXCommon.h"

namespace WristerEngine::_2D
{
	class PostEffect
	{
	public:
		enum class Type
		{
			None, // �Ȃ�
			ColorFlip, // �F���]
			GrayScale, // �O���[�X�P�[��
			GetHighLumi, // ���P�x���o
			Blur, // ���ςڂ���
			GaussianBlur, // �K�E�V�A���u���[
			GaussianBlurLinear, // �P�����̃K�E�V�A���u���[
			CreateDotFilter, // �N���X�t�B���^
			Bloom, // �u���[��
			Dark
		};

	private:
		struct ConstBufferData
		{
			UINT32 effectType = 0;
			float angle = 0;
			float brightness = 1;
			Vector2 uvOffset;
			float pad;
			Vector2 spotlightCenterUV;
		};

		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		static ID3D12Device* device;
		static std::vector<std::unique_ptr<PostEffect>> postEffects;

		ComPtr<ID3D12Resource> texBuff;
		ComPtr<ID3D12DescriptorHeap> rtvHeap, dsvHeap;
		std::array<Vertex, 4> vertices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap = nullptr;
		SRVHandle srvHandle;

		void CreateBuffers();
		void CreateRTV();

	public:
		static PostEffect* Create(Type effectType = Type::None);
		static PostEffect* GetPostEffect(size_t index) { return postEffects[index].get(); }
		void SetEffectType(Type effectType) { constMap->effectType = (UINT32)effectType; }
		void SetAngle(float angle) { constMap->angle = angle; }
		void SetLightPos(Vector2 lightPos) { constMap->spotlightCenterUV = lightPos; }
		void Draw();
		void PreDrawScene();
		void PostDrawScene();
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return srvHandle.gpu; }
	};
}