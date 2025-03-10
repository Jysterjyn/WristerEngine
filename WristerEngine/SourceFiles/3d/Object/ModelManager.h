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
		std::vector<Camera*> cameras;
		// 現在使っているカメラのインデックス
		size_t cameraIndex = 0;

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
		void AddCamera(Camera* camera) { cameras.push_back(camera); }
		// setter
		void SetCameraIndex(size_t cameraIndex);
		// getter
		LightGroup* GetLightGroup() const { return lightGroup.get(); }
		Camera* GetCamera() const { return cameras[cameraIndex]; }
	};
}