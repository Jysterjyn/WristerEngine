#pragma once
#include <D3D12Common.h>
#include <Color.h>

namespace WristerEngine::_3D
{
	class PrimitiveDrawer
	{
	private:
		// �����̍ő吔
		static const UINT MAX_LINE_COUNT = 256;
		
		struct ConstBufferData
		{
			Matrix4 matCamera;
		};

		struct VertexData
		{
			Vector3 pos;
			ColorRGBA color;
		};

		std::vector<VertexData> vertices;	// ���_�f�[�^�z��
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	// ���_�o�b�t�@
		VertexData* vertMap = nullptr;	// ���_�o�b�t�@�̃}�b�v
		D3D12_VERTEX_BUFFER_VIEW vbView{};	// ���_�o�b�t�@�r���[
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;	// �萔�o�b�t�@
		ConstBufferData* constMap = nullptr;

		PrimitiveDrawer() = default;
		~PrimitiveDrawer() = default;
		PrimitiveDrawer(const PrimitiveDrawer&) = delete;
		PrimitiveDrawer& operator=(const PrimitiveDrawer&) = delete;

	public:
		// �V���O���g���C���X�^���X�̎擾
		static PrimitiveDrawer* GetInstance();

		/// <summary>
		/// 3D�����̕`��
		/// </summary>
		/// <param name="p1">�n�_���W</param>
		/// <param name="p2">�I�_���W</param>
		/// <param name="color">�F(RGBA)</param>
		void DrawLine3d(const Vector3& p1, const Vector3& p2, const ColorRGBA& color);

		// ���_�f�[�^�̓]��
		void TransferVertices();

		// ������
		void Initialize();
		void Update();
		void Draw();
	};
}