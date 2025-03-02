#pragma once
#include <random>
#include <memory>

namespace WristerEngine
{
	// ランダム基底クラス
	class RandomDevice
	{
	protected:
		static std::random_device rnd;
		static std::mt19937 rnddev;
	};

	// int型ランダム
	class Random_Int : private RandomDevice
	{
	private:
		std::unique_ptr<std::uniform_int_distribution<int>> random;

	public:
		// コンストラクタ
		Random_Int(int min, int max);
		int operator()() { return (*random)(rnddev); }
	};

	// float型ランダム
	class Random_Float : private RandomDevice
	{
	private:
		std::unique_ptr<std::uniform_real_distribution<float>> random;

	public:
		// コンストラクタ
		Random_Float(float min = 0, float max = 0);
		float operator()() { return (*random)(rnddev); }
	};

	// 確率指定ランダムルーレット
	class Roulette
	{
	private:
		std::unique_ptr<Random_Int> random;
		std::vector<uint32_t> rateTable;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="rateTable">確率リスト</param>
		void Initialize(const std::vector<uint32_t>& rateTable);

		// 事象の出力
		size_t operator()() const;
	};
}