#include "Random.h"
#include <numeric>
using namespace WristerEngine;

std::random_device RandomDevice::rnd;
std::mt19937 RandomDevice::rnddev(rnd());

Random_Int::Random_Int(int min, int max)
{
	random = std::make_unique<std::uniform_int_distribution<int>>(min, max);
}

Random_Float::Random_Float(float min, float max)
{
	random = std::make_unique<std::uniform_real_distribution<float>>(min, max);
}

void Roulette::Initialize(const std::vector<uint32_t>& rateTable_)
{
	rateTable = rateTable_;
	int max = std::accumulate(rateTable.begin(), rateTable.end(), 0);
	random = std::make_unique<Random_Int>(1, max);
}

size_t Roulette::operator()() const
{
	size_t randNum = (*random)();
	for (size_t i = 0; i < rateTable.size(); i++)
	{
		if (randNum <= std::accumulate(rateTable.begin(), rateTable.begin() + i + 1, 0)) { return i; }
	}
	// ƒGƒ‰[
	return (size_t)-1;
}