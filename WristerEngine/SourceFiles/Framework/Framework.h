#pragma once
#include <SceneManager.h>
#include <SpriteManager.h>
#include <DirectXCommon.h>
#include <Constant.h>
#include <CollisionManager2D.h>
#include <GlobalVariables.h>
#include <ImGuiManager.h>
#include <AudioManager.h>

namespace WristerEngine
{
	// ゲーム処理の枠組みクラス
	class Framework : public _2D::SpriteUtility
	{
	private:
		GlobalVariables* globalVariables = GlobalVariables::GetInstance();
		Constant* constant = Constant::GetInstance();
		std::wstring windowName;

	protected:
		AudioManager* audioManager = AudioManager::GetInstance();
		WindowsAPI* wAPI = WindowsAPI::GetInstance();
		Input* input = Input::GetInstance();
		DirectXCommon* dxCommon = DirectXCommon::GetInstance();
		SceneManager* sceneManager = SceneManager::GetInstance();
		_2D::CollisionManager* collisionManager = _2D::CollisionManager::GetInstance();
		ImGuiManager* imGuiManager = ImGuiManager::GetInstance();
		FPS* fps = FPS::GetInstance();

		// 初期化
		virtual void Initialize();
		// 更新
		virtual void Update();
		// 描画
		virtual void Draw() = 0;
		// 終了
		virtual void Finalize();

	public:
		Framework(CR<std::wstring> windowName_){ windowName = windowName_; }
		// 終了チェック
		virtual bool IsEndRequest() { return false; }
		virtual ~Framework() = default;
		// 実行
		void Run();
	};
}