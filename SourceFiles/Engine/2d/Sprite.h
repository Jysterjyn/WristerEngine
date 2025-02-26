#pragma once
#include <array>
#include <memory>
#include "Color.h"
#include "DirectXCommon.h"

namespace WristerEngine::_2D
{
	// �e�N�X�`��1�����̃f�[�^
	struct TextureData : DXCommonGetter
	{
		// �f�t�H���g�f�B���N�g��
		static std::string DEFAULT_TEXTURE_DIRECTORY_PATH;

		static std::list<std::unique_ptr<TextureData>> textures;
		std::string fileName;
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
		SRVHandle srvHandle;

		// �e�N�X�`���ǂݍ���
		static TextureData* Load(const std::string& fileName);
	};

	// �X�v���C�g
	struct Sprite : DXCommonGetter
	{
		Vector2 position;
		float rotation = 0;
		ColorRGBA color;
		Vector2 size; // �\���T�C�Y
		Vector2 anchorPoint;
		bool isFlipX = false;
		bool isFlipY = false;
		bool isInvisible = false;
		Vector2 textureLeftTop; // �؂���̈�̍�����W
		Vector2 textureSize; // �؂���̈�̃T�C�Y
		Vector2 posOffset; // �\���ʒu�̒���

	private:
		class Animation
		{
		private:
			Sprite* sprite = nullptr;
			float width = 0;
			FrameTimer interval;
			size_t animeNum = 0;
			size_t animeNumMax = 0;

		public:
			/// <summary>
			/// ������
			/// </summary>
			/// <param name="sprite">sprite�|�C���^</param>
			/// <param name="spriteNum">�A�j���[�V��������</param>
			/// <param name="animationIntervel">�A�j���[�V�������x</param>
			void Initialize(Sprite* sprite, size_t spriteNum, int animationIntervel);
			// �X�V
			void Update();
		};

		struct ConstBufferData
		{
			Matrix4 mat;
			ColorRGBA color;
		};

		enum class VertexNumber
		{
			LB, // ����
			LT, // ����
			RB, // �E��
			RT  // �E��
		};

		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		const static Matrix4 matProj;
		static std::list<std::unique_ptr<Sprite>> sprites;
		std::array<Vertex, 4> vertices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap = nullptr;
		Vertex* vertMap = nullptr;
		Matrix4 matWorld;
		UINT16 texIndex = 0;
		std::vector<TextureData*> textures{};
		std::unique_ptr<Animation> animation;

		// �e�N�X�`���T�C�Y���C���[�W�ɍ��킹��
		void AdjustTextureSize();
		// ������
		void Initialize();

	public:
		static void UpdateAll();

		void SetAnimation(size_t spriteNum, int animationIntervel);
		// �X�V
		void Update();
		// �`��
		void Draw();
		// �ʒu����ʒ����ɂ���
		void SetCenterPos() { position = Half(WIN_SIZE); }
		// anchorPoint = { 0.5f,0.5f } �ɂ���
		void SetCenterAnchor() { anchorPoint = { 0.5f,0.5f }; }
		// �X�v���C�g����
		static Sprite* Create(std::initializer_list<const std::string> fileNames,
			const Vector2& pos = {}, const Vector2& anchorPoint = {},
			const Vector2& textureSize = {}, const Vector2& textureLeftTop = {});
		// �`��O����
		static void PreDraw();

		// ���݂̃C���f�b�N�X�������e�N�X�`����GPU�n���h�����擾
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return textures[texIndex]->srvHandle.gpu; }

		/// <summary>
		/// �؂���̈���w��(�`��T�C�Y���ς���)
		/// </summary>
		/// <param name="textureSize">�؂���̈�̃T�C�Y</param>
		/// <param name="textureLeftTop">�؂���̈�̍�����W</param>
		void SetRect(const Vector2& textureSize, const Vector2& textureLeftTop = {});

		/// <summary>
		/// �e�N�X�`������
		/// </summary>
		/// <param name="spritNum">������</param>
		void Split(const Vector2& spritNum);

		void SetTextureIndex(UINT16 texIndex);
	};
}