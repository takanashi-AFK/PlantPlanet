#pragma once

#include "../Component.h"
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// 扇形範囲に入った対象を検出するコンポーネント
/// </summary>
class Component_FanRangeDetector : public Component
{
private:
	StageObject* target_;	// 検出対象
	float length_;			// 検出範囲の長さ
	float angle_;			// 検出範囲の角度
	XMVECTOR direction_;	// 検出範囲の方向
	bool isContains_;		// 検出フラグ

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_holder">保有者</param>
	Component_FanRangeDetector(string _name, StageObject* _holder, Component* _parent);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Release() override;

	/// <summary>
	/// 保存
	/// </summary>
	/// <param name="_saveObj">保存情報</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// 読込
	/// </summary>
	/// <param name="_loadObj">読込情報</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// ImGuiパネル表示
	/// </summary>
	void DrawData() override;

	/// <returns>
	/// 検出範囲内に入っているかどうか
	/// </returns>
	bool IsContains();

	void SetTarget(StageObject* _target) { target_ = _target; }

	void SetRange(float _length) { length_ = _length; }

	void SetAngle(float _angle) { angle_ = _angle; }

	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }


};

