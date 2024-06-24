#include "Knuckle.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/GameObject/Camera.h"

namespace
{
	const int FPS = 60;
}

Knuckle::Knuckle(GameObject* _parent) :
	StageObject("Knuckle", "Models/DebugCollision/SphereCollider.fbx", _parent), frame_(), isEnd_(true)
{
}

void Knuckle::Initialize()
{
    // �R���C�_�[�̒��S��ݒ� 
    XMFLOAT3 colliderCenter = XMFLOAT3(0, 0, 0);

    // �R���C�_�[�̃T�C�Y��ݒ�
    XMFLOAT3 colliderSize = XMFLOAT3(3, 1, 3);

	// 

    // BoxCollider�̃R���X�g���N�^���Ăяo���ăR���C�_�[��ǉ�
    BoxCollider* collider = new BoxCollider(colliderCenter, colliderSize);
    AddCollider(collider);

    // ���f���̓ǂݍ���
    modelHandle_ = Model::Load(modelFilePath_);
    assert(modelHandle_ >= 0);
}

void Knuckle::Update()
{
	AutoDelete(0.1f);
}

void Knuckle::Release()
{
}

void Knuckle::Draw()
{
}

void Knuckle::AutoDelete(float _sec)
{
	// 0�ȉ��Ȃ�I��
	if (_sec <= 0)return;

	// �o�߃t���[�����w��b���𒴂�����폜
	if (frame_ > FPS * _sec) {
		KillMe();
		isEnd_ = true;
	}
	else frame_++;
}


