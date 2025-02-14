#pragma once
#include "Mesh.h"
#include "Transform.h"

namespace WristerEngine::_3D
{
	// オブジェクトクラス
	struct Object3d
	{
		Material material;
		std::unique_ptr<Transform> worldTransform;
		bool isDestroy = false;
		bool isInvisible = false;

	private:
		friend class ModelManager;
		Mesh* mesh;
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mesh">メッシュデータ</param>
		void Initialize(Mesh* mesh);
		// 更新
		void Update();
		// 描画
		void Draw();
	};
}