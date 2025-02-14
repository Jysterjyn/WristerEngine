#pragma once
#include "BaseScene.h"
#include <memory>
#include <string>

namespace WristerEngine
{
	// �V�[���t�@�N�g���[�̒��ۃN���X
	class AbstractSceneFactory
	{
	public:
		// ���z�f�X�g���N�^
		virtual ~AbstractSceneFactory() = default;
		// �V�[���쐬
		virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
	};
}