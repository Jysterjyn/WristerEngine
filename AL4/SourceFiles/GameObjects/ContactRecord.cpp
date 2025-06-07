#include "ContactRecord.h"
#include <algorithm>

bool ContactRecord::CheckRecord(uint32_t number)
{
	return std::any_of(records.begin(), records.end(), [number](uint32_t num) { return num == number; });
}
