#include "Component_ShootAttack.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Bullet.h"
#include "../../Stage.h"
#include "../../../../../Engine/Global.h"
using namespace FileManager;

Component_ShootAttack::Component_ShootAttack(string _name, StageObject* _holder, Component* _parent):
	Component_Attack(_holder, _name, ShootAttack,_parent),
	shootingSpeed_(0),
	shootingDirection_({0,0,0,0}),
	shootingPosition_({0,0,0}),
	isShootPositionSet_(false),
	bulletLifeTime_(0),
	effectData({}),
	bulletColliderRadius_(0)
{
}

void Component_ShootAttack::Initialize()
{
}

void Component_ShootAttack::Update()
{	
	if (isActive_ == false)return;

	// �������v���n�u�𐶐�
	Bullet* bulletPrefab = CreateBullet(holder_->GetParent(), effectData,bulletColliderRadius_); {

		// �e�������������I�u�W�F�N�g��ݒ�
		bulletPrefab->SetShooter(holder_);

		// ���������x��ݒ�
		bulletPrefab->SetSpeed(shootingSpeed_);

		// ������������ݒ�
		bulletPrefab->SetDirection(shootingDirection_);

		// �e�̐������Ԃ�ݒ�
		bulletPrefab->SetLifeTime(bulletLifeTime_);

		// �U���͂�ݒ�
		bulletPrefab->SetPower(power_);

		// �������ʒu��ݒ�
		if (isShootPositionSet_ == true)bulletPrefab->SetPosition(shootingPosition_);
		else bulletPrefab->SetPosition(holder_->GetPosition());
	}
	
	// ��������
	bulletPrefab->Execute();
	
	// �U�����~
	Stop();
}

void Component_ShootAttack::Release()
{
}

void Component_ShootAttack::Save(json& _saveObj)
{
	// �U���͂̕ۑ�
	_saveObj["power_"] = power_;

	// ���x�ƕ����̕ۑ�
	_saveObj["shootingSpeed_"] = shootingSpeed_;
	_saveObj["shootingDirection_"] = { REFERENCE_XMVECTOR3(shootingDirection_)};

	// �e�̐������Ԃ̕ۑ�
	_saveObj["bulletLifeTime_"] = bulletLifeTime_;

	// �e�̑傫���̕ۑ�
	_saveObj["bulletColliderRadius_"] = bulletColliderRadius_;

	// �G�t�F�N�g�f�[�^�̕ۑ�
	_saveObj["EffectData"] = {
		{"name",effectData.name},
		{"path",effectData.path},
	};

	// �G�t�F�N�g�̃X�P�[���̕ۑ�
	_saveObj["EffectData"]["scale"] = { REFERENCE_XMFLOAT3(effectData.scale)};
}

void Component_ShootAttack::Load(json& _loadObj)
{
	// �U���͂̓ǂݍ���
	if (_loadObj.contains("power_"))power_ = _loadObj["power_"];

	// ���x�ƕ����̓ǂݍ���
	if (_loadObj.contains("shootingSpeed_"))shootingSpeed_ = _loadObj["shootingSpeed_"];
	if (_loadObj.contains("shootingDirection_"))shootingDirection_ = XMVectorSet(_loadObj["shootingDirection_"][0], _loadObj["shootingDirection_"][1],_loadObj["shootingDirection_"][2],0);

	// �e�̑傫���̓ǂݍ���
	if (_loadObj.contains("bulletColliderRadius_"))bulletColliderRadius_ = _loadObj["bulletColliderRadius_"];

	// �e�̐������Ԃ̓ǂݍ���
	if (_loadObj.contains("bulletLifeTime_"))bulletLifeTime_ = _loadObj["bulletLifeTime_"];

	// �G�t�F�N�g�f�[�^�̓ǂݍ���
	if (_loadObj.contains("EffectData")) {
		if(_loadObj["EffectData"].contains("name")) effectData.name = _loadObj["EffectData"]["name"];
		if(_loadObj["EffectData"].contains("path")) effectData.path = _loadObj["EffectData"]["path"];
		if(_loadObj["EffectData"].contains("scale"))effectData.scale = { _loadObj["EffectData"]["scale"][0].get<float>(),_loadObj["EffectData"]["scale"][1].get<float>(), _loadObj["EffectData"]["scale"][2].get<float>() };
	}
}

void Component_ShootAttack::DrawData()
{
	// �U���͂̐ݒ�
	ImGui::DragInt("Power", &power_);

	// ���x�̐ݒ�
	ImGui::DragFloat("Speed", &shootingSpeed_, 0.1f,0.f);
	
	// �����̐ݒ�
	ImGui::DragFloat3("Direction", (float*)&shootingDirection_, 0.1f);

	// �e�𐶑����Ԃ�ݒ�
	ImGui::DragFloat("bulletLifeTime_", &bulletLifeTime_, 0.1f, 0.f);

	// �e�̑傫����ݒ�
	ImGui::DragFloat("bulletColliderRadius_", &bulletColliderRadius_, 0.1f, 0.f);

	// �U���{�^��
	if (ImGui::Button("Execute"))this->Execute();
	
	// �G�t�F�N�g�f�[�^�̕\��
	if(ImGui::TreeNode("Effect Data")) {

		// �G�t�F�N�g�̖��O
		ImGui::Text("Name: %s", effectData.name.c_str());
		// �G�t�F�N�g�̃p�X
		ImGui::Text("Path: %s", effectData.path.c_str());
		ImGui::SameLine();

		// �G�t�F�N�g�̐ݒ�
		if (ImGui::Button("Set Effect")){
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �G�t�F�N�g�̃t�@�C���p�X��ݒ�
			{
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.efk)\0*.efk\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iefk�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					effectData.path = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					effectData.path = GetAssetsRelativePath(effectData.path);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(effectData.path);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					return;
				}
			}

			// �G�t�F�N�g�̖��O��ݒ�
			effectData.name = GetFileNameWithoutExtension(effectData.path);
		}

		// �G�t�F�N�g�̃X�P�[��
		ImGui::DragFloat3("Scale", (float*)&effectData.scale, 0.1f);

		ImGui::TreePop();
	}
}
