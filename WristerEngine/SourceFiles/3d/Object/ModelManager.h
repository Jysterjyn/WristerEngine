#pragma once
#include "CameraManager.h"
#include "LightGroup.h"
#include "Object3d.h"

namespace WristerEngine::_3D
{
	enum class RootParamNum { MatWorld = (int)TexType::Num, Material, Light, Camera };

	// 3Dオブジェクトの管理
	class ModelManager
	{
	private:
		// ライト
		std::unique_ptr<LightGroup> lightGroup;
		// 読み込んだモデル情報
		std::unordered_map<std::string, std::array<std::unique_ptr<Mesh>, 2>> meshes;
		// 生成したオブジェクト
		std::list<std::unique_ptr<Object3d>> objects;
		// カメラマネージャー
		CameraManager* cameraManager = CameraManager::GetInstance();

		ModelManager() = default;
		~ModelManager() = default;

	public:
		static ModelManager* GetInstance();

		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 描画処理
		void Draw();
		// モデル作成
		Object3d* Create(const std::string& modelName, bool smoothing = false, const std::string& directoryName = "");
		// モデルグループ作成
		std::unordered_map<std::string, Object3d*> CreateGroup(const std::string& directoryName, bool smoothing = false);
		// オブジェクトの解放
		void ClearObjects() { objects.clear(); }
		// getter
		LightGroup* GetLightGroup() const { return lightGroup.get(); }
	};
}