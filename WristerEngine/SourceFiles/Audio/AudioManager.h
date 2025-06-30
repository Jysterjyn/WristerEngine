#pragma once
#include "PointAudio.h"
#include <list>
#include <memory>
#include <string>
#include <map>

namespace WristerEngine
{
	// オーディオ管理
	class AudioManager
	{
	private:
		static std::list<std::unique_ptr<Audio>> audios;

	public:
		/// <summary>
		/// オーディオ生成
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <param name="isLoop">ループ再生フラグ</param>
		/// <returns>オーディオインスタンスのポインタ</returns>
		static Audio* Create(const std::string& fileName, bool isLoop = false);

		/// <summary>
		/// 点音源生成
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <param name="isLoop">ループ再生フラグ</param>
		/// <param name="audioPos">音源のワールド座標</param>
		/// <param name="useCamera">trueにするとカメラの位置がマイク座標になる</param>
		/// <param name="usePan">trueにすると音源が左右移動する</param>
		/// <returns>点音源インスタンスのポインタ</returns>
		static PointAudio* CreatePointAudio(const std::string& fileName,
			bool isLoop = false, const Vector3& audioPos = {}, bool useCamera = true, bool usePan = true);

		static void PlaySE(const std::string& fileName);

		// 全オーディオインスタンスの更新
		static void Update();
	};
}