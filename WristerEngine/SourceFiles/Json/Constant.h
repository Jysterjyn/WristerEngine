#pragma once
#include <unordered_map>
#include "JsonLoader.h"
#include <cassert>
#include "Vector.h"

#define Const(type, str) constant->GetConstant<type>(str)
#define WEConst(type, str) WristerEngine::Constant::GetInstance()->GetConstant<type>(str)

namespace WristerEngine
{
	// 定数の情報を"constant.json"から読み取る
	class Constant final : public JsonLoader
	{
	private:
		std::unordered_map<std::string, const void*> constants;
		nlohmann::json deserialized;

		Constant() = default;
		~Constant() = default;
		Constant(const Constant&) = delete;
		Constant& operator=(const Constant&) = delete;

		// 型別読み込み
		void LoadInt();
		void LoadFloat();
		void LoadDouble();
		void LoadVector();
		void LoadColor();
		void LoadStruct();

	public:
		static Constant* GetInstance();

		// 定数を読み込む
		void LoadConstants();

		template<class T>
		T& GetConstant(const std::string& name)
		{
			T* num = (T*)constants[name];
			assert(num);
			return *num;
		}

		int operator*() { return 0; }

		// 終了処理
		void Finalize();
	};
}