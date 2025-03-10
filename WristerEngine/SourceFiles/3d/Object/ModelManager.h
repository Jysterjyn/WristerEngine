#pragma once
#include "Camera.h"
#include "LightGroup.h"
#include "Object3d.h"

namespace WristerEngine::_3D
{
	enum class RootParamNum { MatWorld = (int)TexType::Num, Material, Light, Camera };

	// 3D�I�u�W�F�N�g�̊Ǘ�
	class ModelManager
	{
	private:
		// ���C�g
		std::unique_ptr<LightGroup> lightGroup;
		// �ǂݍ��񂾃��f�����
		std::unordered_map<std::string, std::array<std::unique_ptr<Mesh>, 2>> meshes;
		// ���������I�u�W�F�N�g
		std::list<std::unique_ptr<Object3d>> objects;
		// �J�����̃|�C���^
		std::vector<Camera*> cameras;
		// ���ݎg���Ă���J�����̃C���f�b�N�X
		size_t cameraIndex = 0;

		ModelManager() = default;
		~ModelManager() = default;

	public:
		static ModelManager* GetInstance();

		// ������
		void Initialize();
		// �X�V
		void Update();
		// �`�揈��
		void DrawObjects();
		// ���f���쐬
		Object3d* Create(const std::string& modelName, bool smoothing = false);
		// �I�u�W�F�N�g�̉��
		void ClearObjects() { objects.clear(); }
		// �J�����̒ǉ�
		void AddCamera(Camera* camera) { cameras.push_back(camera); }
		// setter
		void SetCameraIndex(size_t cameraIndex);
		// getter
		LightGroup* GetLightGroup() const { return lightGroup.get(); }
		Camera* GetCamera() const { return cameras[cameraIndex]; }
	};
}