#pragma once
#include <DirectXMath.h>
#include <list>
#include <string>
#include <assert.h>
#include "../Collider/SphereCollider.h"
#include "../Collider/BoxCollider.h"
#include "Transform.h"




using namespace DirectX;


//-----------------------------------------------------------
//全てのゲームオブジェクト（シーンも含めて）が継承するインターフェース
// ゲームオブジェクトは、親子構造になっていて、
// マトリクスの影響を受けることになる
//-----------------------------------------------------------
class GameObject
{
protected:
	
	//位置や向きなどを管理するオブジェクト
	Transform				transform_;

	//オブジェクトの名前
	std::string				objectName_;

	//衝突判定リスト
	std::list<Collider*>	colliderList_;	

public:
	//コンストラクタ
	GameObject();
	GameObject(GameObject* parent);
	GameObject(GameObject* parent, const std::string& name);

	//デストラクタ
	virtual ~GameObject();

	//各オブジェクトで必ず作る関数
	virtual void Initialize(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw() = 0;
	virtual void Release(void) = 0;

	//自分の該当関数を読んだ後、子供の関数も呼ぶ
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();


	////ローカル行列の取得（このオブジェクトの行列）
	////戻値：ローカル行列
	//XMMATRIX GetLocalMatrix();

	//ワールド行列の取得（親の影響を受けた最終的な行列）
	//戻値：ワールド行列
	XMMATRIX GetWorldMatrix();



	//各フラグの制御
	bool IsDead();			// 削除するかどうか
	void KillMe();			// 自分を削除する
	void Enter();			// Updateを許可
	void EnterOtherObject(GameObject* _target);	// 自分以外のオブジェクトのUpdateを許可
	void Leave();			// Updateを拒否
	void LeaveOtherObject(GameObject *_target);	// 自分以外のオブジェクトのUpdateを拒否
	void Visible();			// Drawを許可
	void Invisible();		// Drawを拒否
	bool IsInitialized();	// 初期化済みかどうか
	void SetInitialized();	// 初期化済みにする
	bool IsEntered();		// Update実行していいか
	bool IsVisibled();		// Draw実行していいか


	//子オブジェクトリストを取得
	//戻値：子オブジェクトリスト
	std::list<GameObject*>* GetChildList();

	//親オブジェクトを取得
	//戻値：親オブジェクトのアドレス
	GameObject* GetParent();

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：name	検索する名前
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	GameObject* FindChildObject(const std::string& name);

	//名前でオブジェクトを検索（対象は全体）
	//引数：検索する名前
	//戻値：見つけたオブジェクトのアドレス
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//オブジェクトの名前を取得
	//戻値：名前
	const std::string& GetObjectName(void) const;

	//子オブジェクトを追加（リストの最後へ）
	//引数：追加するオブジェクト
	void PushBackChild(GameObject* obj);

	//子オブジェクトを追加（リストの先頭へ）
	//引数：obj 追加するオブジェクト
	void PushFrontChild(GameObject* obj);

	//子オブジェクトを全て削除
	void KillAllChildren();

	// コライダーを取得
	std::list<Collider*> GetColliderList();

	// コライダーを取得
	Collider* GetCollider(int index);

	//コライダー（衝突判定）を追加する
	void AddCollider(Collider * collider);

	//何かと衝突した場合に呼ばれる（オーバーライド用）
	//引数：pTarget	衝突した相手
	virtual void OnCollision(GameObject* pTarget, Collider* _collider) {};

	//コライダー（衝突判定）を削除
	void ClearCollider();

	//衝突判定
	//引数：pTarget	衝突してるか調べる相手
	void Collision(GameObject* pTarget);

	//テスト用の衝突判定枠を表示
	void CollisionDraw();

	//RootJobを取得
	GameObject* GetRootJob();

	inline Transform GetTransform() { return this->transform_; };

	//各アクセス関数
	XMFLOAT3 GetPosition() { return transform_.position_; }
	XMFLOAT3 GetRotate() { return transform_.rotate_; }
	XMFLOAT3 GetScale() { return transform_.scale_; }
	XMFLOAT3 GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_ , transform_.position_); }
	XMFLOAT3 GetWorldRotate() { return Transform::Float3Add(GetParent()->transform_.rotate_, transform_.rotate_); }
	XMFLOAT3 GetWorldScale() { return Transform::Float3Add(GetParent()->transform_.scale_, transform_.scale_); }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3( x, y, z )); }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { SetRotate(XMFLOAT3( x, y, z )); }
	void SetRotateX(float x) { SetRotate(x, transform_.rotate_.y, transform_.rotate_.z); }
	void SetRotateY(float y) { SetRotate(transform_.rotate_.x, y, transform_.rotate_.z); }
	void SetRotateZ(float z) { SetRotate(transform_.rotate_.x, transform_.rotate_.y, z); }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	void SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x, y, z)); }
	void SetScale(float scale) { SetScale(scale, scale, scale); }
	void SetScale2D(float scale) { SetScale(scale, scale, 1); }

private:

	//オブジェクト削除（再帰）
	//引数：obj　削除するオブジェクト
	void KillObjectSub(GameObject* obj);


private:
	//フラグ
	struct OBJECT_STATE
	{
		bool initialized : 1;	//初期化済みか
		bool entered : 1;		//更新するか
		bool visible : 1;		//描画するか
		bool dead : 1;			//削除するか
	};
	OBJECT_STATE state_;

	//親オブジェクト
	GameObject* pParent_;

	//子オブジェクトリスト
	std::list<GameObject*> childList_;
};


//オブジェクトを作成するテンプレート
template <class T>
T* Instantiate(GameObject* pParent)
{
	T* pNewObject = new T(pParent);
	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();
	return pNewObject;
}

