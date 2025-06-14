#pragma once
#pragma warning(push)
#pragma warning(disable:26800)
#pragma warning(disable:26819)
#include <json.hpp>
#pragma warning(pop)

namespace WristerEngine
{
	class JsonLoader
	{
	public:
		const static std::string DEFAULT_BASE_DIRECTORY;

	protected:
		static nlohmann::json LoadJson(const std::string& fileName);
	};
}