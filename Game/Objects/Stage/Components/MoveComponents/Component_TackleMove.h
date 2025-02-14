#pragma once

//インクルード
#include"../Component.h"
#include <DirectXMath.h>
using namespace DirectX;


/// <summary>
/// ターゲットに対して追従するコンポーネントのクラス
/// </summary>
class Component_TackleMove :public Component
{
private:
	XMVECTOR direction_;		// 移動方向
	float distance_;			// 移動距離
	float speed_;				// 移動速度
	string easingType_;			// イージングの種類

	float rate_;				// イージングの割合
	bool isFirstMove_;			// 初回移動かどうか
	XMFLOAT3 startPosition_;	// 動作開始の位置

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_holder">保有者</param>
	Component_TackleMove(string _name, StageObject* _holder, Component* _parent);

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
	/// <param name="_saveobj">データの保存情報</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// 読込
	/// </summary>
	/// <param name="_loadobj">データの読込情報</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// 移動方向を設定
	/// </summary>
	/// <param name="_direction">方向</param>
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }

	/// <summary>
	/// 移動方向を設定
	/// </summary>
	/// <param name="_direction">方向</param>
	void SetDistance(float _distance) { distance_ = _distance; }

	void SetSpeed(float _speed) { speed_ = _speed; }

	XMVECTOR GetDirection() { return direction_; }

	/// <summary>移動方向を取得</summary>
	float GetDistance() { return distance_; }
	/// <summary>
	/// ImGuiパネル表示
	/// </summary>
	void DrawData()override;

	/// <summary>
	/// 実行されているかどうか
	/// </summary>
	bool IsActived() { return isActive_; }
};