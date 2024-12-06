#pragma once

//インクルード
#include"../Component.h"
#include <DirectXMath.h>
using namespace DirectX;


/// <summary>
/// ターゲットに対して追従するコンポーネントのクラス
/// </summary>
class Component_Chase:public Component
{
private:
	StageObject* target_;	//追従の対象オブジェクト
	string targetName_;		//追従の対象オブジェクト名

	float speed_;			//速度
	float limitDistance_;	//追従の限界距離
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_holder">保有者</param>
	Component_Chase(string _name, StageObject* _holder, Component* _parent);

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
	/// ImGuiパネル表示
	/// </summary>
	void DrawData()override;

	/// <summary>
	/// ターゲットの設定
	/// </summary>
	void SetTarget(StageObject* _target) { target_ = _target; }

	/// <summary>
	/// 追跡をやめる距離の設定
	/// </summary>
	/// <returns></returns>
	float GetLimitDistance() { return limitDistance_; }

private:

	/// <summary>
	/// 方向の計算
	/// </summary>
	XMVECTOR CalcDirection(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);

	/// <summary>
	/// 距離を求める
	/// </summary>
	float CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);
	
	/// <summary>
	/// ホルダーの向きを回転させる
	/// </summary>
	float CalcRotateAngle(XMVECTOR _direction);
	
	/// <summary>
	/// 移動
	/// </summary>
	void Move(XMVECTOR _direction);
};

