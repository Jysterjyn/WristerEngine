#pragma once
#include "Camera.h"
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
		// カメラのポインタ
		std::unordered_map<std::string, Camera*> cameras;
		// 現在使っているカメラの名前
		std::string cameraName = "default";

		ModelManager() = default;
		~ModelManager() = default;

	public:
		static ModelManager* GetInstance();

		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 描画処理
		void DrawObjects();
		// モデル作成
		Object3d* Create(const std::string& modelName, bool smoothing = false);
		// オブジェクトの解放
		void ClearObjects() { objects.clear(); }
		// カメラの追加
		void AddCamera(const std::string& cameraName_, Camera* camera) { cameras[cameraName_] = camera; }
		// setter
		void SetCameraName(const std::string& cameraName);
		// getter
		LightGroup* GetLightGroup() const { return lightGroup.get(); }
		Camera* GetCamera() { return cameras[cameraName]; }
	};
}