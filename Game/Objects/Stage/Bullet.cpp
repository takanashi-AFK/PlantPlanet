#include "Bullet.h"

// �C���N���[�h
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "Components/GaugeComponents/Component_HealthGauge.h"
#include "Stage.h"

Bullet::Bullet(GameObject* _parent)
	:StageObject("Bullet", "Models/DebugCollision/SphereCollider.fbx", _parent), isActive_(false), frame_(), speed_(), direction_(), shooter_(), mt(), data_(), lifeTime_(2.f), power_(20)
{
}

void Bullet::Initialize()
{
	// �R���C�_�[��ǉ�
	AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), colliderRadius_));

	// ���f���̓ǂݍ���
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// effekseer: :Effect�̓ǂݍ���
	EFFEKSEERLIB::gEfk->AddEffect(data_.name, data_.path);/*������*/
	EFFEKSEERLIB::gEfk->AddEffect("Hit", "Effects/Attack_Impact.efk");/*������*/

	// effekseer: :Effect�̍Đ����̐ݒ�
	EFFEKSEERLIB::EFKTransform t;/*������*/
	DirectX::XMStoreFloat4x4(&(t.matrix), transform_.GetWorldMatrix());/*������*/
	t.isLoop = false;/*������*/
	t.maxFrame = 180;/*������*/
	t.speed = 1.f;/*������*/

	// effekseer: :Effect�̍Đ�
	mt = EFFEKSEERLIB::gEfk->Play(data_.name, t);/*������*/
}

void Bullet::Update()
{
	// ���쒆�łȂ���ΏI��
	if (isActive_ == false)return;

	// �ړ�
	Move(direction_,speed_);

	// �����폜
	//if (EFFEKSEERLIB::gEfk->IsEffectPlaying("Sylph10") == false)KillMe();
	AutoDelete(lifeTime_);

	// effekseer: :Effect�̍Đ����̍X�V
	Transform effectTrans = transform_;
	effectTrans.scale_ = data_.scale;
	DirectX::XMStoreFloat4x4(&(mt->matrix), effectTrans.GetWorldMatrix());/*������*/
}

void Bullet::Draw()
{
	// ���f���̕`��
	//Model::SetTransform(modelHandle_, transform_);
	//Model::Draw(modelHandle_);
}

void Bullet::OnCollision(GameObject* _target, Collider* _collider)
{
	if (isActive_ == false)return;

	// �^�[�Q�b�g��StageObject�łȂ��ꍇ�͏������s��Ȃ�
	StageObject* target = dynamic_cast<StageObject*>(_target);
	if (!target) return;

	if (target->GetObjectName() == shooter_->GetObjectName())return;
	auto list = target->FindComponent(HealthGauge);
	if (list.empty()) return;
	// �_���[�W����
	for (auto hm : list) {

		((Component_HealthGauge*)hm)->TakeDamage(power_);
		this->KillMe();

		/*if (((Component_HealthGauge*)hm)->IsDead()) {
			((Stage*)FindObject("Stage"))->DeleteStageObject((StageObject*)_target);
		}*/
	}

}

void Bullet::Move(XMVECTOR _dir, float _speed)
{
	// �ړ�
	XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_) + (XMVector3Normalize(_dir) * _speed));
}

void Bullet::AutoDelete(float _sec)
{
	// 0�ȉ��Ȃ�I��
	if (_sec <= 0)return;

	// �o�߃t���[�����w��b���𒴂�����폜
	if (frame_ > FPS * _sec) KillMe();
	else frame_++;
}

Bullet* CreateBullet(GameObject* _parent, EffectData _data,float _colderRadius)
{
	// �C���X�^���X�̐���
	Bullet* bullet = new Bullet(_parent);

	// �e�I�u�W�F�N�g�����݂���ꍇ�A�q�I�u�W�F�N�g�Ƃ��ēo�^
	if(_parent != nullptr) _parent->PushBackChild(bullet);

	// �ݒ�
	{
		// �e�̃G�t�F�N�g�f�[�^��ݒ�
		bullet->SetEffectData(_data);

		// �R���C�_�[�̔��a��ݒ�
		bullet->SetColliderRadius(_colderRadius);

		// ������
		bullet->Initialize();
	}

	// ���������C���X�^���X�̃A�h���X��Ԃ�
	return bullet;
}

Bullet* CreateBullet(GameObject* _parent, EffectData _data)
{
	return CreateBullet(_parent,_data,0.5f);
}
