#pragma once
#include "SceneManager.h"
#include "SpriteManager.h"
#include "DirectXCommon.h"
#include "Constant.h"
#include <CollisionManager.h>
#include <GlobalVariables.h>

namespace WristerEngine
{
	// ゲーム処理の枠組みクラス
	class Framework : public _2D::SpriteUtility
	{
	private:
		GlobalVariables* globalVariables = GlobalVariables::GetInstance();
		WindowsAPI* wAPI = WindowsAPI::GetInstance();
		Constant* constant = Constant::GetInstance();

	protected:
		std::wstring windowName;
		Input* input = Input::GetInstance();
		DirectXCommon* dxCommon = DirectXCommon::GetInstance();
		SceneManager* sceneManager = SceneManager::GetInstance();
		CollisionManager* collisionManager = CollisionManager::GetInstance();

		// 初期化
		virtual void Initialize();
		// 更新
		virtual void Update();
		// 描画
		virtual void Draw() = 0;
		// 終了チェック
		virtual bool IsEndRequest() { return false; }
		// 終了
		virtual void Finalize();

	public:
		virtual ~Framework() = default;
		// 実行
		void Run();
	};
}