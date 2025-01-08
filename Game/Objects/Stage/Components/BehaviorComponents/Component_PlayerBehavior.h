#pragma once
#include "../Component.h"
#include <DirectXMath.h>
#include "../../StageObject.h"
#include <vector>
#include "../../../../../Engine/ResourceManager/Model.h"
#include "../../../EffekseeLib/EffekseerVFX.h"
#include "../../../../../Engine/Global.h"
#include "../../../../Plants/Plant.h"
#include "../../../UI/UIImage.h"
#include "../../../UI/UIText.h"

// 前方宣言
class CountDown;
class Component_BossBehavior;
class Salad;

enum PlayerState {
	PLAYER_STATE_IDLE = 0,
	PLAYER_STATE_WALK,
	PLAYER_STATE_SHOOT,
	PLAYER_STATE_DODGE,
	PLAYER_STATE_DEAD,
	PLAYER_STATE_SHOOT_WALK_LEFT,
	PLAYER_STATE_SHOOT_WALK_RIGHT,
	PLAYER_STATE_SHOOT_WALK_BACK,
	PLAYER_STATE_SHOOT_WALK_FORWARD,
	PLAYER_STATE_SHOOT_IDLE,
	PLAYER_STATE_INTRACT,
	PLAYER_STATE_MELEE,	
	PLAYER_STATE_MADESALAD,
	PLAYER_STATE_MAX
};

class Component_PlayerBehavior : public Component
{
public :
	static constexpr float defaultSpeed_Walk = .1f;
	static constexpr float defaultTime_CollectPlant = 1;
	static constexpr int defaultMax_HP = 100;
	static constexpr int defaultPow_Range = 10.f;
	static constexpr int defaultPow_Melee = 20.f;

private:
	PlayerState nowState_, prevState_;	// 現在の状態、前の状態
	float shootHeight_;					// 射撃の高さ
	XMFLOAT3 prevAngles_;				// 前フレームの回転
	Component_BossBehavior* bossBehavior;
	int invincibilityFrame_;			// 無敵フレーム
	int walkingFrame_;					// 連続して歩いている時間
	int lockRotateFrame_;				// 回転を固定する時間
	int lockRotateFrameLeft_;			// 回転を固定してから経過した時間
	int researchPoint_;
	vector<PlantData> myPlants_;
	std::list<std::function<PlantData::FuncValue(Component_PlayerBehavior*)>> saladEffects_;

	// effekseer: 変形行列
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform;
	EffectData effectData_;

	bool isShootStart_ : 1;					// 射撃開始フラグ
	bool isDodgeStart_ : 1;					// 回避開始フラグ
	bool isGameStart_  : 1;					// ゲーム開始フラグ
	bool isUseStamina_ : 1;					// スタミナを使用したか
	bool isShootAttack_: 1;					
  
	bool isMeleeStart_	 : 1;
	bool isRenewalPopUp_ : 1;

	bool isEatSaladEnd_  : 1;
	bool isBreakableWall_: 1;

	float stamina_decrease_dodge_;
	float stamina_decrease_melee_;
	float stamina_decrease_shoot_;

	float timeCollectPlant;
	struct PopUpInfo
	{
		int time;
		
		UIText*  info_;
		UIText*	 texts_[3];

		UIImage* backGround_;
		UIImage* images_[3];

	}popUpInfo_;

	struct SaladEffectLogo
	{
		UIText* texts_[3];
		UIImage* images_[3];
	}saladEffectLogo_;

public:
	/// <summary> コンストラクタ </summary>
	Component_PlayerBehavior(string _name, StageObject* _holder, Component* _parent);

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

	void EatSalad(Salad salad);

	/*
	setter :*/
	/// <param name="_state"> プレイヤーの状態 </param>
	void SetState(PlayerState _state) { prevState_ = nowState_; nowState_ = _state; }

	/// <param name="_height"> 射撃の高さ </param>
	void SetShootHeight(float _height) { shootHeight_ = _height; }

	/// <param name="_flag"> ゲーム開始フラグ </param>
	void SetGameStart(bool _flag) { isGameStart_ = _flag; }

	/// <param name="_point"> リサーチポイント </param>
	void SetResearchPoint(int _point) { researchPoint_ = _point; }

	void SetStaminaDecrease_Shoot(int _stamina_decrease_shoot) { stamina_decrease_shoot_ = _stamina_decrease_shoot; }
	void SetStaminaDecrease_Dodge(int _stamina_decrease_dodge) { stamina_decrease_dodge_ = _stamina_decrease_dodge; }
	void SetStaminaDecrease_Melee(int _stamina_decrease_melee) { stamina_decrease_melee_ = _stamina_decrease_melee; }

	void SetTimeCollectPlant(float time);
	void SetMyPlants(vector<PlantData> _plants) { myPlants_.clear(); myPlants_ = _plants; }

	void SetBreakableWall(bool _flag) { isBreakableWall_ = _flag; }

	/// <returns> プレイヤーの状態 </returns>
	PlayerState GetState() const { return nowState_; }

	/// <returns> 前のプレイヤーの状態 </returns>
	PlayerState GetPrevState() const { return prevState_; }

	/// <returns> 射撃の高さ </returns>
	float GetShootHeight() const { return shootHeight_; }

	/// <returns> ゲーム開始フラグ </returns>
	bool GetGameStart() const { return isGameStart_; }

	int GetLockRotateTime()const { return lockRotateFrame_; };
	int GetLockRotateTimeLeft()const { return lockRotateFrameLeft_; };

	/// <returns> レアリティごとにリサーチポイントを判別して取得 </returns>
	int GetResearchPointByRarity(PlantData _plantData);

	/// <returns> プレイヤーのリサーチポイント </returns>
	int GetResearchPoint() const { return researchPoint_; }

	/// <returns> プレイヤーの植物データ </returns>
	vector<PlantData> GetMyPlants() { return myPlants_; }

	float GetStaminaDecrease_Shoot() { return stamina_decrease_shoot_; }
	float GetStaminaDecrease_Dodge() { return stamina_decrease_dodge_; }
	float GetStaminaDecrease_Melee() { return stamina_decrease_melee_; }

	float GetTimeCollectPlant();
/*
predicate :*/
	/// <returns> プレイヤーが死んでいるか </returns>
	bool IsDead();

	/// <returns> 付近にインタラクト可能なオブジェクトがあるかどうか </returns>
	bool IsInteractable();

	/// <returns> 射撃を開始したか </returns>
	bool IsShootStart() const { return isShootStart_; }

	/// <returns> 回避を開始したか </returns>
	bool IsDodgeStart() const { return isDodgeStart_; }

	/// <returns> 現在の状態が指定した状態か </returns>
	bool IsState(PlayerState _state) const { return nowState_ == _state; }

	bool IsBreakableWall() const { return isBreakableWall_; }

private:
	/// <summary> 射撃方向の計算 </summary>
	XMVECTOR CalcShootDirection();

	/// <summary> 付近の植物を取得 </summary>
	StageObject* GetNearestPlant(PlantData& _plantData);
	/*
	state :*/
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

	/// <summary> インタラクト状態時の処理 </summary>
	void Interact();

	/// <summary> 近接攻撃時の処理 </summary>
	void Melee();

	void MadeSalad();

	/// <summary> アイテム(サラダ)によるバフ、デバフの適用</summary>
	void ApplyEffects();

	void ResetSaladEffectLogo();

	void DrawPopUp();

	void AdjustCameraDistance();
};