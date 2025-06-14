#pragma once
#include <memory>
#include <cassert>
#include <array>
#include "DirectXCommon.h"

namespace WristerEngine
{
	// result値がエラーなら代入時に警告が出る
	struct Result
	{
	private:
		HRESULT result = S_OK;

	public:
		// コンストラクタ
		Result() {}
		Result(const HRESULT& result) { operator=(result); }

		void operator=(HRESULT result_)
		{
			result = result_;
			assert(SUCCEEDED(result));
		}
	};

	// バッファ生成
	template<class T> void CreateBuffer(ID3D12Resource** buff, T** map, UINT64 width)
	{
		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(width);
		// バッファの生成
		Result result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE,
			&resDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(buff));

		// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
		result = (*buff)->Map(0, nullptr, (void**)map);
		// 繋がりを解除
		(*buff)->Unmap(0, nullptr);
	}

	// インプットレイアウトの追加
	D3D12_INPUT_ELEMENT_DESC SetInputLayout(LPCSTR semanticName, DXGI_FORMAT format);
	// シェーダー読み込み
	void LoadShader(ID3DBlob** shaderBlob, std::wstring shaderName, LPCSTR target);
	// 深度バッファの生成
	void CreateDepthBuffer(ID3D12DescriptorHeap** dsvHeap);
}