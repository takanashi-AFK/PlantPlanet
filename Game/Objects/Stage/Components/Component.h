#pragma once

// インクルード
#include "../../../../Engine/Json/JsonReader.h"
#include "../../../../Engine/Collider/Collider.h"

// 前方宣言
class StageObject;
class GameObject;


// using宣言
using std::vector;

// コンポーネントタイプ
enum ComponentType 
{
	BossBehavior,
	CactanBehavior,
	Chase,
	CircleRangeDetector,
	Fall,
	FanRangeDetector,
	HealthGauge,
	HelingoBehavior,
	HelingoFall,
	MeleeAttack,
	MoveX,
	PlayerBehavior,
	Rise,
	Rotation,
	RotationX,
	RotationY,
	RotationZ,
	ShootAttack,
	TackleMove,
	Timer,
	WASDInputMove,
	PlayerMotion,
	Teleporter,
	PlantGenerator,
	Plant,
	StaminaGauge, 
	Accessory,
	WeakRangeEnemy,
	MeleeEnemyBehavior,
	ReturnGate,
	RectangleDetector,
	BreakableWall,
	RunningawayEnemy,
	RunningawayEnemyMotion,
	// コンポーネント追加時に識別番号を追加
	Max
	
};



// コンポ―ネント基底クラス
class Component
{
protected:
	string name_;                           // コンポーネント名
	ComponentType type_;                    // コンポーネントタイプ
	StageObject* holder_;                   // コンポ―ネント保有者のポインタ
	vector<Component*> childComponents_;    // 子コンポーネント群
	Component* parent_;                     // 親コンポーネント
	bool isActive_;                         // アクティブ状態かどうか
	bool killMe_;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_holder">保有者</param>
	/// <param name="_name">名前</param>
	Component(StageObject* _holder, string _name,ComponentType  _type);
	Component(StageObject* _holder, string _name,ComponentType _type,Component* _parent);

	virtual ~Component();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 開放
	/// </summary>
	virtual void Release() = 0;

	/// <summary>
	/// 衝突処理
	/// </summary>
	virtual void OnCollision(GameObject* _target, Collider* _collider) {};

	/// <summary>
	/// 保存
	/// </summary>
	/// <param name="_saveObj">保存情報</param>
	virtual void Save(json& _saveObj) {};

	/// <summary>
	/// 読込
	/// </summary>
	/// <param name="_saveObj">読込情報</param>
	virtual void Load(json& _loadObj) {};

	/// <summary>
	/// ImGuiパネル表示
	/// </summary>
	virtual void DrawData() {};

	/// <summary>
	/// 自身＆子コンポーネントを初期化
	/// </summary>
	void ChildIntialize();

	/// <summary>
	/// 自身＆子コンポーネントを更新
	/// </summary>
	void ChildUpdate();

	/// <summary>
	/// 自身＆子コンポーネントを解放
	/// </summary>
	void ChildRelease();

	/// <summary>
	/// 衝突処理
	/// </summary>
	void ChildOnCollision(GameObject* _target, Collider* _collider);

	/// <summary>
	/// 自身＆子コンポーネントを描画
	/// </summary>
	void ChildDrawData();

	/// <summary>
	/// 自身＆子コンポーネントを保存
	/// </summary>
	void ChildSave(json& _saveObj);

	/// <summary>
	/// 自身＆子コンポーネントを読込
	/// </summary>
	void ChildLoad(json& _loadObj);

	/// <summary>
	/// 子コンポーネントをリストに追加
	/// </summary>
	/// <param name="comp">コンポーネントタイプ</param>
	bool AddChildComponent(Component* _comp);

	/// <summary>
	/// 子コンポーネントをリストから削除
	/// </summary>
	/// <param name="comp">コンポーネントタイプ</param>
	bool DeleteChildComponent(string _name);

	/// <summary>
	/// コンポーネントをリスト内あるかに探す
	/// </summary>
	/// <param name="_name">コンポーネント名</param>
	/// <returns>コンポーネントがあるかどうか</returns>
	bool FindChildComponent(string _name);

	/// <returns>
	/// 子コンポーネントを取得
	/// </returns>
	/// <param name="_type">コンポーネントタイプ</param>
	Component* GetChildComponent(string _name);
	
	/// <returns>
	/// 子コンポーネントを取得
	/// </returns>
	vector<Component*> GetChildComponent(ComponentType _type);

	/// <returns>
	/// コンポーネントタイプを取得
	/// </returns>
	ComponentType GetType() const { return type_; }

	StageObject* GetHolder() const{ return holder_; }

	/// <returns>
	/// コンポーネント名を取得
	/// </returns>
	string GetName() { return name_; }

	void Execute() { isActive_ = true; }
	void Stop() { isActive_ = false; }

	void KillMe();
	bool isKillMe()const;

	bool IsActive() const { return isActive_; }
};

/// <summary>
/// コンポーネントを作成する
/// </summary>
/// <param name="_type">コンポーネントタイプ</param>
/// <param name="_holder">保有者</param>
/// <returns>作成したコンポーネント</returns>
Component* CreateComponent(string _name,ComponentType _type,StageObject* _holder, Component* _parent);
Component* CreateComponent(string _name,ComponentType _type,StageObject* _holder);

/// <summary>
/// コンポーネントタイプを文字列に変換する 
/// </summary>
string ComponentTypeToString(ComponentType _type);