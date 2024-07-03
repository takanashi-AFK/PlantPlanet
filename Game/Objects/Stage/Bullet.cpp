#include "Bullet.h"

// �C���N���[�h
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "Components/GaugeComponents/Component_HealthGauge.h"
#include "Stage.h"

Bullet::Bullet(GameObject* _parent) 
:StageObject("Bullet","Models/DebugCollision/SphereCollider.fbx", _parent),isActive_(false), frame_(), speed_(), direction_()
{
}

void Bullet::Initialize()
{
	// �R���C�_�[��ǉ�
	AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f));

	// ���f���̓ǂݍ���
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	SetScale(0.2f);
}

void Bullet::Update()
{
	// ���쒆�łȂ���ΏI��
	if (isActive_ == false)return;

	// �ړ�
	Move(direction_,speed_);

	// �����폜
	AutoDelete(2.f);
}

void Bullet::Draw()
{
	// ���f���̕`��
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void Bullet::OnCollision(GameObject* _target)
{
	// �v���C���[�ƏՓ˂����ꍇ
	if (_target->GetObjectName() == "Char_Player") {

		// �v���C���[��HP�}�l�[�W���[�R���|�[�l���g���擾
		Component* hm = ((StageObject*)_target)->FindComponent("HealthManager");
		if (hm == nullptr)return;

		// �v���C���[��HP�����炷
		((Component_HealthGauge*)hm)->TakeDamage(20);

		// �v���C���[��HP��0�ȉ��̏ꍇ
		if (((Component_HealthGauge*)hm)->GetHP() <= 0) {

			// �v���C���[������
			((Stage*)FindObject("Stage"))->DeleteStageObject((StageObject*)_target);
		}
		
		this->KillMe();
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

