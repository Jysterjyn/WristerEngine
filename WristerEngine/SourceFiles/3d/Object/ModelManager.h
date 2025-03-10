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
		std::unordered_map<std::string, Camera*> cameras;
		// ���ݎg���Ă���J�����̖��O
		std::string cameraName = "default";

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
		void AddCamera(const std::string& cameraName_, Camera* camera) { cameras[cameraName_] = camera; }
		// setter
		void SetCameraName(const std::string& cameraName);
		// getter
		LightGroup* GetLightGroup() const { return lightGroup.get(); }
		Camera* GetCamera() { return cameras[cameraName]; }
	};
}