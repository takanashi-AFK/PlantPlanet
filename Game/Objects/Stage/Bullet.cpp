#include "Bullet.h"

// インクルード
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "Components/GaugeComponents/Component_HealthGauge.h"
#include "Stage.h"

Bullet::Bullet(GameObject* _parent)
	:StageObject("Bullet", "Models/DebugCollision/SphereCollider.fbx", _parent), isActive_(false), frame_(), speed_(), direction_(), shooter_(), effectModelTransform_(), data_(), lifeTime_(2.f), power_(20)
{
}

void Bullet::Initialize()
{
	// コライダーを追加
	AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), colliderRadius_));

	// モデルの読み込み
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// effekseer: :Effectの読み込み
	EFFEKSEERLIB::gEfk->AddEffect(data_.name, data_.path);
	EFFEKSEERLIB::gEfk->AddEffect("hit", "Effects/Attack_Impact.efk");

	// effekseer: :Effectの再生情報の設定
	EFFEKSEERLIB::EFKTransform effectTransform;
	DirectX::XMStoreFloat4x4(&(effectTransform.matrix), transform_.GetWorldMatrix());
	effectTransform.isLoop = false;
	effectTransform.maxFrame = 180;
	effectTransform.speed = 1.f;

	// effekseer: :Effectの再生
	effectModelTransform_ = EFFEKSEERLIB::gEfk->Play(data_.name, effectTransform);
}

void Bullet::Update()
{
	// 動作中でなければ終了
	if (isActive_ == false)return;

	// 移動
	Move(direction_,speed_);

	// 自動削除
	//if (EFFEKSEERLIB::gEfk->IsEffectPlaying("Sylph10") == false)KillMe();
	AutoDelete(lifeTime_);

	// effekseer: :Effectの再生情報の更新
	Transform effectTrans = transform_;
	effectTrans.scale_ = data_.scale;
	DirectX::XMStoreFloat4x4(&(effectModelTransform_->matrix), effectTrans.GetWorldMatrix());
}

void Bullet::Draw()
{
	// モデルの描画
	//Model::SetTransform(modelHandle_, transform_);
	//Model::Draw(modelHandle_);
}

void Bullet::OnCollision(GameObject* _target, Collider* _collider)
{
	if (isActive_ == false)return;

	// ターゲットがStageObjectでない場合は処理を行わない
	StageObject* target = (StageObject*)(_target);
	if (!target) return;

	if (target->GetObjectName() == shooter_->GetObjectName())return;
	std::vector<Component*> list = target->FindComponent(HealthGauge);
	if (list.empty()) return;
	// ダメージ処理
	for (auto hm : list) {

		// エフェクト再生
		EFFEKSEERLIB::EFKTransform t;
		DirectX::XMStoreFloat4x4(&(t.matrix), target->GetWorldMatrix());
		t.isLoop = false;
		t.maxFrame = 60;
		t.speed = 1.0f;

		effectModelTransform_ = EFFEKSEERLIB::gEfk->Play("hit", t);
		((Component_HealthGauge*)hm)->TakeDamage(power_);
		this->KillMe();
	}

}

void Bullet::Move(XMVECTOR _dir, float _speed)
{
	// 移動
	XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_) + (XMVector3Normalize(_dir) * _speed));
}

void Bullet::AutoDelete(float _sec)
{
	// 0以下なら終了
	if (_sec <= 0)return;

	// 経過フレームが指定秒数を超えたら削除
	if (frame_ > FPS * _sec) KillMe();
	else frame_++;
}

Bullet* CreateBullet(GameObject* _parent, EffectData _data,float _colderRadius)
{
	// インスタンスの生成
	Bullet* bullet = new Bullet(_parent);

	// 親オブジェクトが存在する場合、子オブジェクトとして登録
	if(_parent != nullptr) _parent->PushBackChild(bullet);

	// 設定
	{
		// 弾のエフェクトデータを設定
		bullet->SetEffectData(_data);

		// コライダーの半径を設定
		bullet->SetColliderRadius(_colderRadius);

		// 初期化
		bullet->Initialize();
	}

	// 生成したインスタンスのアドレスを返す
	return bullet;
}

Bullet* CreateBullet(GameObject* _parent, EffectData _data)
{
	return CreateBullet(_parent,_data,0.5f);
}
