#pragma once
#include <Audio.h>
#include <string>
#include <WristerEngineUtility.h>

namespace WristerEngine
{
	// オーディオ管理
	class AudioManager
	{
	private:
		uList audios;

		AudioManager() = default;
		~AudioManager() = default;
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;

	public:
		static AudioManager* GetInstance();

		/// <summary>
		/// オーディオ生成
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <param name="isLoop">ループ再生フラグ</param>
		/// <returns>オーディオインスタンスのポインタ</returns>
		Audio* Create(const std::string& fileName, bool isLoop = false);

		void PlaySE(const std::string& fileName);

		// 全オーディオインスタンスの更新
		void Update();
	};
}