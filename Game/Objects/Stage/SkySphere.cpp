#include "SkySphere.h"

// �C���N���[�h
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/DirectX/Direct3D.h"

SkySphere::SkySphere(GameObject* _parent)
	:StageObject("SkySphere", "Models/SkySphere/SkySphere.fbx", _parent)
{
}

void SkySphere::Initialize()
{
	// ���f���̓ǂݍ���
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// �ۗL����R���|�[�l���g�̏���������
	for (auto comp : myComponents_)comp->ChildIntialize();
}

void SkySphere::Draw()
{
	// �X�J�C�X�t�B�A�p�̃V�F�[�_�[��ݒ�
	Direct3D::SetShader(Direct3D::SHADER_SKY);

	// ���f���̕`��
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);

	// �V�F�[�_�[�����ɖ߂�
	Direct3D::SetShader(Direct3D::SHADER_3D);
}
