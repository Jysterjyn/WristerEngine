#pragma once
#include <string>
#include <wrl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfapi.h>
#include <dshow.h>
#include <memory>
#include <mfmediaengine.h>
#include <WristerEngineUtility.h>

namespace WristerEngine
{
	class AudioManager;

	// オーディオ
	class Audio : public ListObject
	{
	private:
		Microsoft::WRL::ComPtr<IGraphBuilder> graphBuilder;
		Microsoft::WRL::ComPtr<IMediaControl> mediaControl;
		Microsoft::WRL::ComPtr<IMediaPosition> mediaPosition;
		Microsoft::WRL::ComPtr<IBasicAudio> basicAudio;
		const std::string name;
		bool isLoop = false;
		bool isSE = false;

		// デフォルトディレクトリ
		static std::string DIRECTORY_PATH;

		// 再生が終わったか
		bool IsFinished() const;

	protected:
		// 初期化
		void Initialize() override;
		// 更新
		virtual void Update() override;
		bool Remove() const override { return isSE && IsFinished(); }

	public:
		friend AudioManager;

		Audio(CR<std::string> fileName, bool isLoop) : name(fileName), isLoop(isLoop) {}
		// 仮想デストラクタ
		virtual ~Audio() = default;
		// 再生
		void Play() { PlayResume(); SetPlayPosition(0); }
		void PlayResume() { mediaControl->Run(); }
		// 中断
		void Stop() { mediaControl->Stop(); }
		// スピード変更
		void SetSpeed(double playSpd) { mediaPosition->put_Rate(playSpd); }
		// 再生位置変更(秒単位)
		void SetPlayPosition(double playPosition) { mediaPosition->put_CurrentPosition(playPosition); }
		// -10000(無音)~0(最大音量) : 単位 デシベル
		void SetVolume(long volume) { basicAudio->put_Volume(volume); }
		// -10000(左)〜10000(右)
		void SetBalance(long balance) { basicAudio->put_Balance(balance); }
		REFTIME GetReftime();
	};
}