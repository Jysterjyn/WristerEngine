#pragma once
#include "Mesh.h"
#include "Transform.h"

namespace WristerEngine::_3D
{
	// オブジェクトクラス
	class Object3d
	{
	public:
		Material material;
		Transform transform;
		bool isInvisible = false;

		void Destroy() { isDestroy = true; }
		const Mesh* GetMesh() const { return mesh; }

	private:
		friend class ModelManager;
		Mesh* mesh = nullptr;
		bool isDestroy = false;

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