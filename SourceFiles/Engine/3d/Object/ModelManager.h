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
		static std::unique_ptr<LightGroup> lightGroup;
		// 読み込んだモデル情報
		static std::unordered_map<std::string, std::array<std::unique_ptr<Mesh>, 2>> meshes;
		// 生成したオブジェクト
		static std::list<std::unique_ptr<Object3d>> objects;
		// カメラのポインタ
		static Camera* camera;

	public:
		// 初期化
		static void Initialize();
		// 更新
		static void Update();
		// 描画処理
		static void DrawObjects();
		// モデル作成
		static Object3d* Create(const std::string& modelName, bool smoothing = false);
		// オブジェクトの解放
		static void ClearObjects() { objects.clear(); }
		// setter
		static void SetCamera(Camera* camera_) { camera = camera_; }
		// getter
		static LightGroup* GetLightGroup() { return lightGroup.get(); }
		static Camera* GetCamera() { return camera; }
	};
}