#pragma once
#include "Vector.h"
#include "Color.h"
#include <cmath>

namespace WristerEngine
{
	// ライト基底クラス
	class BaseLight
	{
	private:
		ColorRGB color; // ライト色
		bool active = false; // 有効フラグ

	public:
		// setter
		void SetColor(const ColorRGB& color_) { color = color_; }
		void SetActive(bool active_) { active = active_; }
		// getter
		const ColorRGB& GetColor()  { return color; }
		bool IsActive() const { return active; }
	};

	// 平行光源
	class DirectionalLight : public BaseLight
	{
	private:
		Vector3 dir = { 1,0,0 }; // ライト光線方向

	public:
		struct ConstBufferData
		{
			Vector3 lightv; // ライトへの方向を表すベクトル
			float pad;
			ColorRGB lightcolor; // ライトの色
			unsigned int active;
		};

		// ライト光線方向を変更
		void SetDir(const Vector3& lightdir_) { dir = Normalize(lightdir_); }
		// ライト光線方向を取得
		const Vector3& GetDir()  { return dir; }
	};

	// スポットライト
	class SpotLight : public BaseLight
	{
	private:
		Vector3 dir;
		Vector3 pos;
		Vector3 atten = { 1,1,1 };
		Vector2 factorAngleCos = { 0.5f,0.2f };

	public:
		struct ConstBufferData
		{
			Vector3 lightv;
			float pad1;
			Vector3 lightpos;
			float pad2;
			ColorRGB lightcolor;
			float pad3;
			Vector3 lightatten;
			float pad4;
			Vector2 lightfactoranglecos;
			unsigned int active;
			float pad5;
		};

		// setter
		void SetDir(const Vector3& dir_) { dir = Normalize(dir_); }
		void SetPos(const Vector3& pos_) { pos = pos_; }
		void SetAtten(const Vector3& atten_) { atten = atten_; }
		void SetFactorAngle(const Vector2& factorAngle)
		{
			factorAngleCos.x = std::cos(factorAngle.x);
			factorAngleCos.y = std::cos(factorAngle.y);
		}
		// getter
		const Vector3& GetDir() { return dir; }
		const Vector3& GetPos() { return pos; }
		const Vector3& GetAtten() { return atten; }
		const Vector2& GetFactorAngleCos() { return factorAngleCos; }
	};

	// 点光源
	class PointLight : public BaseLight
	{
	private:
		Vector3 pos;
		Vector3 atten = { 1,1,1 }; // ライト距離減衰係数

	public:
		struct ConstBufferData
		{
			Vector3 lightpos;
			float pad1;
			ColorRGB lightcolor;
			float pad2;
			Vector3 lightAtten;
			unsigned int active;
		};

		// setter
		void SetPos(const Vector3& pos_) { pos = pos_; }
		void SetAtten(const Vector3& atten_) { atten = atten_; }
		// getter
		const Vector3& GetPos() { return pos; }
		const Vector3& GetAtten() { return atten; }
	};
}