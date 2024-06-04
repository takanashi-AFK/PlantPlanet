#include "Component_HelingoBehavior.h"

#include "../../StageObject.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../RotationComponents/Component_RotationY.h"
#include "../TimerComponent/Component_Timer.h"
#include "../MoveComponents/Component_Fall.h"
#include "../MoveComponents/Component_Chase.h"
#include "../../Stage.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../HealthManagerComponents/Component_HealthManager.h"

Component_HelingoBehavior::Component_HelingoBehavior(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, HelingoBehavior,_parent)
{
}

void Component_HelingoBehavior::Initialize()
{
	// �R���C�_�[�̒ǉ�
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));

	// �q�R���|�[�l���g�̒ǉ�
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	if (FindChildComponent("Fall") == false)AddChildComponent(CreateComponent("Fall", Fall, holder_, this));
	if (FindChildComponent("Chase") == false)AddChildComponent(CreateComponent("Chase", Chase, holder_, this));
}

void Component_HelingoBehavior::Update()
{
	if (target_ == nullptr)target_ = (StageObject*)holder_->FindObject(targetName_);
	if (target_ == nullptr)return;

	auto detector = dynamic_cast<Component_CircleRangeDetector*>(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	auto fall = dynamic_cast<Component_Fall*>(GetChildComponent("Fall"));

	// �͈͓��Ƀv���C���[������ꍇ
	detector->SetRadius(discoveryrange_);
	if (fall->IsActived() == false && detector->IsContains()) {

		// �Ǐ]���J�n
		auto chase = dynamic_cast<Component_Chase*>(GetChildComponent("Chase"));
		if (chase == nullptr)return;

		chase->SetTarget(target_);
		chase->Start();

		detector->SetRadius(fallrange_);
		if (detector->IsContains() == true) {

			// ��������
			
			if (fall == nullptr)return;
			chase->Stop();
			fall->Start();
		}
	}
	else{
		// �Ǐ]���~
		auto chase = dynamic_cast<Component_Chase*>(GetChildComponent("Chase"));
		if (chase == nullptr)return;
		chase->Stop();
	}

}

void Component_HelingoBehavior::Release()
{
}

void Component_HelingoBehavior::OnCollision(GameObject* _target)
{
	// �v���C���[�ƏՓ˂����ꍇ
	if (_target->GetObjectName() == "Char_Player") {

		// �v���C���[��HP�}�l�[�W���[�R���|�[�l���g���擾
        Component* hm = ((StageObject*)_target)->FindComponent("HealthManager");
		if (hm == nullptr)return;

		// �v���C���[��HP�����炷
		auto fall = dynamic_cast<Component_Fall*>(GetChildComponent("Fall"));
		if(fall->IsFalling())((Component_HealthManager*)hm)->TakeDamage(5);

		// �v���C���[��HP��0�ȉ��̏ꍇ
		if (((Component_HealthManager*)hm)->GetHP() <= 0) {

			// �v���C���[������
			((Stage*)holder_->FindObject("Stage"))->DeleteStageObject((StageObject*)_target);
		}
	}
}

void Component_HelingoBehavior::Save(json& _saveObj)
{
	if (target_ != nullptr)_saveObj["target_"] = target_->GetObjectName();
	_saveObj["fallrange_"] = fallrange_;
	_saveObj["discoveryrange_"] = discoveryrange_;
}

void Component_HelingoBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("target_"))targetName_ = _loadObj["target_"];
	if (_loadObj.contains("fallrange_"))fallrange_ = _loadObj["fallrange_"];
	if (_loadObj.contains("discoveryrange_"))discoveryrange_ = _loadObj["discoveryrange_"];
}

void Component_HelingoBehavior::DrawData()
{
	ImGui::DragFloat("fallrange_", &fallrange_);
	ImGui::DragFloat("discoveryrange_", &discoveryrange_);
#ifdef _DEBUG

	//�Ώۂ̑I��
	vector<string> objNames;
	objNames.push_back("null");

	for (auto obj : ((Stage*)holder_->GetParent())->GetStageObjects())objNames.push_back(obj->GetObjectName());

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
	else {
		target_ = (StageObject*)holder_->FindObject(objNames[select]);

		auto detector = dynamic_cast<Component_CircleRangeDetector*>(GetChildComponent("CircleRangeDetector"));
		detector->SetTarget(target_);
	}
#endif // _DEBUG
}
