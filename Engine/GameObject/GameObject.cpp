#include "gameObject.h"
#include <assert.h>
#include "../global.h"
#include "../SceneManager.h"
#include "../../Game/Objects/UI/UIInventory.h"
//コンストラクタ（親も名前もなし）
GameObject::GameObject(void) :
	GameObject(nullptr, "")
{

}

//コンストラクタ（名前なし）
GameObject::GameObject(GameObject * parent) :
	GameObject(parent, "")
{
}

//コンストラクタ（標準）
GameObject::GameObject(GameObject * parent, const std::string& name)
	: pParent_(parent),



	objectName_(name)
{
	childList_.clear();
	state_ = { 0, 1, 1, 0 };

	if(parent)
		transform_.pParent_ = &parent->transform_;

}

//デストラクタ
GameObject::~GameObject()
{
	for (auto it = colliderList_.begin(); it != colliderList_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	colliderList_.clear();
}

// 削除するかどうか
bool GameObject::IsDead()
{
	return (state_.dead != 0);
}

// 自分を削除する
void GameObject::KillMe()
{
	state_.dead = 1;
}

// Updateを許可
void GameObject::Enter()
{
	state_.entered = 1;
}

void GameObject::EnterOtherObject(GameObject* _target)
{
	for (auto it = childList_.begin(); it != childList_.end(); it++) {
		if ((*it) != _target) {
			(*it)->Enter();
		}
	}
}

// Updateを拒否
void GameObject::Leave()
{
	state_.entered = 0;
}

void GameObject::LeaveOtherObject(GameObject *_target)
{
	for (auto it = childList_.begin(); it != childList_.end(); it++){

		if ((*it) != _target){
			(*it)->Leave();
		}

	}
}

// Drawを許可
void GameObject::Visible()
{
	state_.visible = 1;
}

// Drawを拒否
void GameObject::Invisible()
{
	state_.visible = 0;
}

// 初期化済みかどうか
bool GameObject::IsInitialized()
{
	return (state_.initialized != 0);
}

// 初期化済みにする
void GameObject::SetInitialized()
{
	state_.initialized = 1;
}

// Update実行していいか
bool GameObject::IsEntered()
{
	return (state_.entered != 0);
}

// Draw実行していいか
bool GameObject::IsVisibled()
{
	return (state_.visible != 0);
}

//子オブジェクトリストを取得
std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

//親オブジェクトを取得
GameObject * GameObject::GetParent(void)
{
	return pParent_;
}

//名前でオブジェクトを検索（対象は自分の子供以下）
GameObject * GameObject::FindChildObject(const std::string & name)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return nullptr;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトから探す
	while (it != end) {
		//同じ名前のオブジェクトを見つけたらそれを返す
		if ((*it)->GetObjectName() == name)
			return *it;

		//その子供（孫）以降にいないか探す
		GameObject* obj = (*it)->FindChildObject(name);
		if (obj != nullptr)
		{
			return obj;
		}

		//次の子へ
		it++;
	}

	//見つからなかった
	return nullptr;
}

//オブジェクトの名前を取得
const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}

//子オブジェクトを追加（リストの最後へ）
void GameObject::PushBackChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_back(obj);
}

//子オブジェクトを追加（リストの先頭へ）
void GameObject::PushFrontChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_front(obj);
}

//子オブジェクトを全て削除
void GameObject::KillAllChildren(void)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトを1個ずつ削除
	while (it != end)
	{
		KillObjectSub(*it);
		delete *it;
		it = childList_.erase(it);
	}

	//リストをクリア
	childList_.clear();
}

std::list<Collider*> GameObject::GetColliderList()
{
	return std::list<Collider*>();
}

Collider* GameObject::GetCollider(int index)
{
	// 要素番号がリストの範囲内かどうかをチェック
	if (index < 0 || index >= colliderList_.size()) return nullptr;

	// イテレータを使って指定した要素番号まで進む
	auto it = colliderList_.begin();
	std::advance(it, index);

	// 指定した要素番号の要素を返す
	return *it;
}

//オブジェクト削除（再帰）
void GameObject::KillObjectSub(GameObject * obj)
{
	if (!childList_.empty())
	{
		auto list = obj->GetChildList();
		auto it = list->begin();
		auto end = list->end();
		while (it != end)
		{
			KillObjectSub(*it);
			delete *it;
			it = list->erase(it);
		}
		list->clear();
	}
	obj->Release();
}



//コライダー（衝突判定）を追加する
void GameObject::AddCollider(Collider* collider)
{
	collider->SetGameObject(this);
	colliderList_.push_back(collider);
}

//コライダー（衝突判定）を削除
void GameObject::ClearCollider()
{
	for (auto it = colliderList_.begin(); it != colliderList_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	colliderList_.clear();
}

//衝突判定
void GameObject::Collision(GameObject * pTarget)
{
	//自分同士の当たり判定はしない
	if (pTarget == nullptr || this == pTarget)
	{
		return;
	}

	//自分とpTargetのコリジョン情報を使って当たり判定
	//1つのオブジェクトが複数のコリジョン情報を持ってる場合もあるので二重ループ
	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{
		for (auto j = pTarget->colliderList_.begin(); j != pTarget->colliderList_.end(); j++)
		{
			if ((*i)->IsHit(*j))
			{
				//当たった
				this->OnCollision(pTarget,*i);
			}
		}
	}

	//子供がいないなら終わり
	if (pTarget->childList_.empty())
		return;

	//子供も当たり判定
	for (auto i = pTarget->childList_.begin(); i != pTarget->childList_.end(); i++)
	{
		Collision(*i);
	}
}


//テスト用の衝突判定枠を表示
void GameObject::CollisionDraw()
{
	Direct3D::SetShader(Direct3D::SHADER_UNLIT);

	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{
		(*i)->Draw(GetWorldPosition());
	}

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

//RootJobを取得
GameObject * GameObject::GetRootJob()
{
	if (GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}




void GameObject::UpdateSub()
{
	Update();
	//Transform();
	SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");

	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		if((*it)->IsEntered() == true)
		(*it)->UpdateSub();
	}

	for (auto it = childList_.begin(); it != childList_.end();)
	{
		if ((*it)->IsDead() == true)
		{
			(*it)->ReleaseSub();
			SAFE_DELETE(*it);
			it = childList_.erase(it);
		}
		else
		{
			//当たり判定
			(*it)->Collision(GetParent());
			it++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();


	//リリース時は削除
#ifdef _DEBUG
		//コリジョンの描画
	if (Direct3D::isDrawCollision_)
	{
		CollisionDraw();
	}
#endif

	//その子オブジェクトの描画処理
	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	//コライダーを削除
	ClearCollider();
	
	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->ReleaseSub();
		SAFE_DELETE(*it);
	}

	Release();
}




////ローカル行列の取得（このオブジェクトの行列）
//XMMATRIX GameObject::GetLocalMatrix(void)
//{
//	return transform_.GetWorldMatrix();
//}

//ワールド行列の取得（親の影響を受けた最終的な行列）
XMMATRIX GameObject::GetWorldMatrix(void)
{
	return transform_.GetWorldMatrix();
}


