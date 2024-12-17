#include "SkySphere.h"
#include"../../../Engine/Global.h"

// �C���N���[�h
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/ResourceManager/Model.h"

SkySphere::SkySphere(GameObject* _parent)
	:StageObject("SkySphere", "Models/SkySphere/SkySphere.fbx", _parent), timeZone_{ TIME_ZONE::MORNING}
{
}

void SkySphere::Initialize()
{
	// ���f���̓ǂݍ���
	transform_.pParent_ = nullptr;

	string modelNames[static_cast<int>(TIME_ZONE::AMOUNT)] =
	{
		"Models/SkySphere/SkySphere.fbx",
		"Models/SkySphere/SkySphere.fbx",
	};

	for (auto i = 0u; i < static_cast<int>(TIME_ZONE::AMOUNT); ++i) {

		modelHandles_[i] = Model::Load(modelNames[i]);
		assert(modelHandles_[i] >= 0);
	}

	// �ۗL����R���|�[�l���g�̏���������
	for (auto comp : myComponents_)comp->ChildIntialize();

	SetLights();
}

void SkySphere::Draw()
{
	// �X�J�C�X�t�B�A�p�̃V�F�[�_�[��ݒ�
	SetLights();
	Direct3D::SetShader(Direct3D::SHADER_SKY);

	modelHandle_ = modelHandles_[static_cast<int>(timeZone_)];

	// ���f���̕`��

	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);

	// �V�F�[�_�[�����ɖ߂�
	Direct3D::SetShader(Direct3D::SHADER_3D);
}

void SkySphere::SetTimeZone(TIME_ZONE tz)
{
	timeZone_ = tz;

	SetLights();
}

SkySphere::TIME_ZONE SkySphere::GetTimeZone()
{
	return timeZone_;
}

void SkySphere::SetLights()
{
	int index = static_cast<int>(timeZone_);

	Light::sunLight = globalLightColors_[index];
	Light::ambientLight = AmbientLightColors_[index];
}
