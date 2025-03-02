#pragma once
#include <unordered_map>
#include "JsonLoader.h"
#include <cassert>
#include "Vector.h"

#define Const(type, str) constant->GetConstant<type>(str)
#define WEConst(type, str) WristerEngine::Constant::GetInstance()->GetConstant<type>(str)

namespace WristerEngine
{
	// ’è”‚Ìî•ñ‚ğ"constant.json"‚©‚ç“Ç‚İæ‚é
	class Constant : public WristerEngine::JsonLoader
	{
	private:
		std::unordered_map<std::string, const void*> constants;
		nlohmann::json deserialized;

		Constant() = default;
		~Constant() = default;

		// Œ^•Ê“Ç‚İ‚İ
		void LoadInt();
		void LoadFloat();
		void LoadDouble();
		void LoadVector();
		void LoadColor();
		void LoadStruct();

	public:
		Constant(const Constant& obj) = delete;
		Constant& operator=(const Constant& obj) = delete;

		static Constant* GetInstance();

		// ’è”‚ğ“Ç‚İ‚Ş
		void LoadConstants();

		template<class T>
		T& GetConstant(const std::string& name)
		{
			T* num = (T*)constants[name];
			assert(num);
			return *num;
		}

		int operator*() { return 0; }

		// I—¹ˆ—
		void Finalize();
	};
}