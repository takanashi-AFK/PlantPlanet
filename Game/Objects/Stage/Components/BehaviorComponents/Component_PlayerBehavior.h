#pragma once
#include "../Component.h"
#include <DirectXMath.h>
#include "../../StageObject.h"
#include <vector>
#include "../../../../../Engine/ResourceManager/Model.h"
#include "../../../EffekseeLib/EffekseerVFX.h"/*★★★*/
#include "../../../../../Engine/Global.h"


// 前方宣言
class CountDown;
class Component_BossBehavior;
// プレイヤーの状態
enum PlayerState {
	PLAYER_STATE_IDLE = 0,	/* 待機 */
	PLAYER_STATE_WALK,		/* 歩行 */
	PLAYER_STATE_SHOOT,		/* 射撃 */
	PLAYER_STATE_DODGE,		/* 回避 */
	PLAYER_STATE_DEAD,		/* 死亡 */
	PLAYER_STATE_MAX
};

class Component_PlayerBehavior : public Component
{
private:
	PlayerState nowState_, prevState_;	// 現在の状態、前の状態
	float shootHeight_;					// 射撃の高さ
	bool isGameStart_;					// ゲーム開始フラグ
	int invincibilityFrame_;			// 無敵フレーム
	Component_BossBehavior* bossBehavior;

	// effekseer: 変形行列
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform;/*★★★*/
	EffectData effectData_;

	bool isShootStart_;					// 射撃開始フラグ
	bool isDodgeStart_;					// 回避開始フラグ

	float stamina_;						// スタミナ
	bool isUseStamina_;					// スタミナを使用したか
public:
	/// <summary> コンストラクタ </summary>
	Component_PlayerBehavior(string _name,StageObject* _holder,Component* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 解放 </summary>
	void Release() override;
	
	/// <summary> 保存 </summary>
	void Save(json& _saveObj) override;

	/// <summary> 読込 </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGuiパネル表示 </summary>
	void DrawData() override;

/*
setter :*/
	/// <param name="_state"> プレイヤーの状態 </param>
	void SetState(PlayerState _state) { prevState_ = nowState_; nowState_ = _state; }

	/// <param name="_height"> 射撃の高さ </param>
	void SetShootHeight(float _height) { shootHeight_ = _height; }

	/// <param name="_flag"> ゲーム開始フラグ </param>
	void SetGameStart(bool _flag) { isGameStart_ = _flag; }

/*
getter :*/
	/// <returns> プレイヤーの状態 </returns>
	PlayerState GetState() const { return nowState_; }

	/// <returns> 前のプレイヤーの状態 </returns>
	PlayerState GetPrevState() const { return prevState_; }

	/// <returns> 射撃の高さ </returns>
	float GetShootHeight() const { return shootHeight_; }

	/// <returns> ゲーム開始フラグ </returns>
	bool GetGameStart() const { return isGameStart_; }
/*
predicate :*/
	/// <returns> プレイヤーが死んでいるか </returns>
	bool IsDead();

	/// <returns> 射撃を開始したか </returns>
	bool IsShootStart() const { return isShootStart_; }

	/// <returns> 回避を開始したか </returns>
	bool IsDodgeStart() const { return isDodgeStart_; }

	/// <returns> 現在の状態が指定した状態か </returns>
	bool IsState(PlayerState _state) const { return nowState_ == _state; }
private:
	/// <summary> 射撃方向の計算 </summary>
	XMVECTOR CalcShootDirection();

	/// <summary> 待機状態時の処理 </summary>
	void Idle();
	
	/// <summary> 歩行状態時の処理 </summary>
	void Walk();
	
	/// <summary> 射撃状態時の処理 </summary>
	void Shoot();

	/// <summary> 回避状態時の処理 </summary>
	void Dodge();

	/// <summary> 死亡状態時の処理 </summary>
	void Dead();
};

