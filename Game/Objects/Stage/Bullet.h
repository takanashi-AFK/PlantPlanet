#pragma once

// インクルード
#include "StageObject.h"

// effekseer: インクルード
#include "../../../EffekseeLib/EffekseerVFX.h"
#include "../../../Engine/Global.h"

/// <summary>
/// 発射する弾を管理するクラス
/// </summary>

class Bullet :public StageObject
{
private:
	bool isActive_;			// 弾が動作中かどうか
	int frame_;				// 経過フレーム	
	float speed_;			// 移動速度
	XMVECTOR direction_;	// 移動方向
	StageObject* shooter_;	// 発射したオブジェクト
	int power_;

	// effekseer: 変形行列
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform_;/*★★★*/
	EffectData data_;

	float lifeTime_;	// 生存時間
	float colliderRadius_;	// 当たり判定の半径
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">親オブジェクト</param>
	Bullet(GameObject* _parent);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision(GameObject* _target, Collider* _collider) override;

	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="_speed">速度</param>
	void SetSpeed(float _speed) { speed_ = _speed; }
	
	/// <summary>
	/// 移動方向を設定
	/// </summary>
	/// <param name="_direction">方向</param>
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }

	/// <summary>
	/// 実行
	/// </summary>
	void Execute() { isActive_ = true; }

	/// <summary>
	/// なんのオブジェクトが発射したかを設定
	/// </summary>
	/// <param name="_shooter"></param>
	void SetShooter(StageObject* _shooter) { shooter_ = _shooter; }

	void SetEffectData(EffectData _data) { data_ = _data; }

	void SetLifeTime(float _time) { lifeTime_ = _time; }

	void SetPower(int _power) { power_ = _power; }

	void SetColliderRadius(float _radius) { colliderRadius_ = _radius; }
private:
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="_dir">方向</param>
	/// <param name="_speed">速度</param>
	void Move(XMVECTOR _dir, float _speed);

	/// <summary>
	/// 一定時間後に自身を削除
	/// </summary>
	/// <param name="_sec">消えるまでの時間（秒）</param>
	void AutoDelete(float _sec);
};

Bullet* CreateBullet(GameObject* _parent, EffectData _data, float _colderRadius); 
Bullet* CreateBullet(GameObject* _parent,EffectData _data);