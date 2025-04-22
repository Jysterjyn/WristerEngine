#pragma once
#include <BaseCamera.h>
#include <DebugCamera.h>
#include <RailCamera.h>

namespace WristerEngine::_3D
{
	enum class CameraType
	{
		Normal, // 通常カメラ
		Debug, // デバッグカメラ
		Rail // レールカメラ
	};

	struct CameraProp
	{
		DebugCamera::Prop debugProp;
		RailCamera::Prop railProp;
	};

	class CameraManager
	{
		// 現在使っているカメラの名前
		std::string name = "default";
		// カメラのポインタ
		static std::unordered_map<std::string, std::unique_ptr<BaseCamera>> cameras;

		CameraManager() = default;
		~CameraManager() = default;

	public:
		static CameraManager* GetInstance();
		// カメラ作成
		BaseCamera* Create(const std::string& name, CameraType type = CameraType::Normal, CameraProp* prop = nullptr);
		// 更新
		void Update() { Get()->Update(); }
		// カメラの解放
		void Clear() { cameras.clear(); }
		// 使用カメラの変更
		void SetName(const std::string& name);
		// 使用カメラの取得
		BaseCamera* Get() const { return cameras[name].get(); };
	};
}