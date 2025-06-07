#pragma once
#include <vector>
#include <stdint.h>

class ContactRecord
{
private:
	std::vector<uint32_t> records;

public:
	void AddRecord(uint32_t number) { records.push_back(number); }
	bool CheckRecord(uint32_t number);
	void Clear() { records.clear(); }
};
