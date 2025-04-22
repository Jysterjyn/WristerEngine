#pragma once
#include <BaseCamera.h>
#include <DebugCamera.h>
#include <RailCamera.h>

namespace WristerEngine::_3D
{
	enum class CameraType
	{
		Normal, // �ʏ�J����
		Debug, // �f�o�b�O�J����
		Rail // ���[���J����
	};

	struct CameraProp
	{
		DebugCamera::Prop debugProp;
		RailCamera::Prop railProp;
	};

	class CameraManager
	{
		// ���ݎg���Ă���J�����̖��O
		std::string name = "default";
		// �J�����̃|�C���^
		static std::unordered_map<std::string, std::unique_ptr<BaseCamera>> cameras;

		CameraManager() = default;
		~CameraManager() = default;

	public:
		static CameraManager* GetInstance();
		// �J�����쐬
		BaseCamera* Create(const std::string& name, CameraType type = CameraType::Normal, CameraProp* prop = nullptr);
		// �X�V
		void Update() { Get()->Update(); }
		// �J�����̉��
		void Clear() { cameras.clear(); }
		// �g�p�J�����̕ύX
		void SetName(const std::string& name);
		// �g�p�J�����̎擾
		BaseCamera* Get() const { return cameras[name].get(); };
	};
}