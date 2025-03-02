#pragma once
#include <string>
#include <wrl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfapi.h>
#include <dshow.h>
#include <memory>
#include <wrl.h>

namespace WristerEngine
{
	class AudioManager;

	// オーディオ
	class Audio
	{
	private:
		Microsoft::WRL::ComPtr<IGraphBuilder> graphBuilder;
		Microsoft::WRL::ComPtr<IMediaControl> mediaControl;
		Microsoft::WRL::ComPtr<IMediaPosition> mediaPosition;
		Microsoft::WRL::ComPtr<IBasicAudio> basicAudio;
		// デフォルトディレクトリ
		static std::string DEFAULT_DIRECTORY_PATH;

		// 再生が終わったか
		bool IsFinished();

	protected:
		// 初期化
		void Initialize(const std::string& fileName);

	public:
		friend AudioManager;
		// 仮想デストラクタ
		virtual ~Audio() = default;
		// 再生
		void Play() { mediaControl->Run(); SetPlayPosition(0); }
		// 中断
		void Stop() { mediaControl->Stop(); }
		// スピード変更
		void SetSpeed(double playSpd) { mediaPosition->put_Rate(playSpd); }
		// 再生位置変更(秒単位)
		void SetPlayPosition(double playPosition) { mediaPosition->put_CurrentPosition(playPosition); }
		// -10000(無音)~0(最大音量) : 単位 デシベル
		void SetVolume(long volume) { basicAudio->put_Volume(volume); }
		// -10000(左)～10000(右)
		void SetBalance(long balance) { basicAudio->put_Balance(balance); }
	};
}