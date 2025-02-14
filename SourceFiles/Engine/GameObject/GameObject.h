#pragma once
#include "Input.h"
#include "SceneManager.h"
#include "Constant.h"
#include <LevelLoader.h>

namespace WristerEngine
{
	namespace _2D
	{
		// ゲームに表示されている2Dスプライトの基底クラス
		class GameObject
		{
		protected:
			Input* input = Input::GetInstance();
			Constant* constant = Constant::GetInstance();
			std::unique_ptr<Sprite> sprite;

		public:
			// 仮想デストラクタ
			virtual ~GameObject() = default;
			// 初期化(純粋仮想関数)
			virtual void Initialize(const ObjectData& objData) = 0;
			// 更新(純粋仮想関数)
			virtual void Update() = 0;
			// 描画(純粋仮想関数)
			virtual void Draw() = 0;
			// 現在のシーンを取得
			const std::string& GetNowScene() const { return SceneManager::GetInstance()->GetNowScene(); }
		};
	}

	namespace _3D
	{
		// ゲームに表示されている3Dオブジェクトの基底クラス
		class GameObject
		{
		protected:
			Input* input = Input::GetInstance();
			Constant* constant = Constant::GetInstance();

		public:
			// 仮想デストラクタ
			virtual ~GameObject() = default;
			/// <summary>
			/// 初期化(純粋仮想関数)
			/// </summary>
			/// <param name="objectData">Jsonファイルから読み込んだデータ</param>
			virtual void Initialize(const ObjectData& objectData) = 0;
			// 更新(純粋仮想関数)
			virtual void Update() = 0;
			// 現在のシーンを取得
			const std::string& GetNowScene() const { return SceneManager::GetInstance()->GetNowScene(); }
		};
	}
}