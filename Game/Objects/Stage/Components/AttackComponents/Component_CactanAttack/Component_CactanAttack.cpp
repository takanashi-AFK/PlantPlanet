#include "Component_CactanAttack.h"

// �C���N���[�h
#include "../../../../../../Engine/ImGui/imgui.h"
#include "../../../Bullet.h"
#include "../../../Stage.h"

Component_CactanAttack::Component_CactanAttack(string _name, StageObject* _holder, Component* _parent):
	Component_Attack(_holder, _name, CactanAttack,_parent),
	bulletSpeed_(0.5f),
	target_{}
{
}

void Component_CactanAttack::Initialize()
{
}

void Component_CactanAttack::Update()
{	
}

void Component_CactanAttack::Release()
{
}

void Component_CactanAttack::Save(json& _saveObj)
{
	// �ۑ�
	_saveObj["bulletSpeed_"] = bulletSpeed_;
	_saveObj["target_"] = target_->GetObjectName();
}

void Component_CactanAttack::Load(json& _loadObj)
{
	// �Ǎ�
	if (_loadObj.contains("bulletSpeed_"))bulletSpeed_ = _loadObj["bulletSpeed_"];
	if (_loadObj.contains("target_"))target_ = (StageObject*)holder_->FindObject(_loadObj["target_"]);
}

void Component_CactanAttack::DrawData()
{
	ImGui::DragFloat("Speed", &bulletSpeed_, 0.1f, 0, 2.f);

	//�^�[�Q�b�g�w��
	SetTarget();
	if (target_ == nullptr)return;

	//�^�[�Q�b�g�̈ʒu�ƕێ��҂̈ʒu���擾���A�x�N�g����
	XMFLOAT3 t = target_->GetPosition();
	XMFLOAT3 h = holder_->GetPosition();
	XMVECTOR dir = XMLoadFloat3(&t) - XMLoadFloat3(&h);

	//�x�N�g���𐳋K��
	dir = XMVector3Normalize(dir);

	if (ImGui::Button("Attack")) {
		//�e�𐶐�
		Bullet* pBullet = Instantiate<Bullet>(holder_->GetParent());

		//�e�̕����ƃX�s�[�h�A�����n�_��ݒ�
		pBullet->SetDirection(dir);
		pBullet->SetSpeed(bulletSpeed_);
		pBullet->SetPosition(holder_->GetPosition());

		//����
		pBullet->Execute();
	}
}

void Component_CactanAttack::Attack()
{

}

void Component_CactanAttack::SetTarget()
{
	vector<string> objNames;
	objNames.push_back("null");

	//StageObject�̖��O���擾
	for (auto obj : ((Stage*)holder_->GetParent())->GetStageObjects()) {

		//�������g�ȊO�̃I�u�W�F�N�g�����������ꍇ�A���X�g�ɒǉ�
		if(obj->GetObjectName() != holder_->GetObjectName())
		objNames.push_back(obj->GetObjectName());
	}
	static int select = 0;
	if (ImGui::BeginCombo("target_", objNames[select].c_str())) {
		for (int i = 0; i < objNames.size(); i++) {
			bool is_selected = (select == i);
			if (ImGui::Selectable(objNames[i].c_str(), is_selected))select = i;
			if (is_selected)ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	if (select == 0)target_ = nullptr;
	else target_ = (StageObject*)holder_->FindObject(objNames[select]);

}
