#pragma once
#include <memory>
#include <cassert>
#include <array>
#include "DirectXCommon.h"

namespace WristerEngine
{
	// result�l���G���[�Ȃ������Ɍx�����o��
	struct Result
	{
	private:
		HRESULT result = S_OK;

	public:
		// �R���X�g���N�^
		Result() {}
		Result(const HRESULT& result) { operator=(result); }

		void operator=(HRESULT result_)
		{
			result = result_;
			assert(SUCCEEDED(result));
		}
	};

	// �o�b�t�@����
	template<class T> void CreateBuffer(ID3D12Resource** buff, T** map, UINT64 width)
	{
		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(width);
		// �o�b�t�@�̐���
		Result result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE,
			&resDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(buff));

		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		result = (*buff)->Map(0, nullptr, (void**)map);
		// �q���������
		(*buff)->Unmap(0, nullptr);
	}

	// �C���v�b�g���C�A�E�g�̒ǉ�
	D3D12_INPUT_ELEMENT_DESC SetInputLayout(LPCSTR semanticName, DXGI_FORMAT format);
	// �V�F�[�_�[�ǂݍ���
	void LoadShader(ID3DBlob** shaderBlob, std::wstring shaderName, LPCSTR target);
	// �[�x�o�b�t�@�̐���
	void CreateDepthBuffer(ID3D12DescriptorHeap** dsvHeap);
}