#include "Component_Teleporter.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../../StageObject.h"
#include "../../../../../Engine/GameObject/GameObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../../../../../Engine/Global.h"

Component_Teleporter::Component_Teleporter(string _name, StageObject* _holder, Component* _parent)
:Component(_holder, _name, Teleporter, _parent)
{
}

void Component_Teleporter::Initialize()
{
	// �q�R���|�[�l���g�̒ǉ�
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
}

void Component_Teleporter::Update()
{
	if (!isActive_)return;

	auto detector = dynamic_cast<Component_CircleRangeDetector*>(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	// PlayerBehavior�����I�u�W�F�N�g������
	auto playerList = ((Stage*)holder_->GetParent())->GetStageObjects();
	for (auto a : playerList) {

		StageObject* pl = a;
		if (pl != nullptr) {

			if (pl->FindComponent("PlayerBehavior")) {
				target_ = pl;
				break;
			}
		}
	}

	// ���o�Ώۂ̐ݒ�
	detector->SetTarget(target_);

	if (detector->IsContains()) {
		Teleport();
	}
}

void Component_Teleporter::Release()
{
}

void Component_Teleporter::Save(json& _saveObj)
{
}

void Component_Teleporter::Load(json& _loadObj)
{
}

void Component_Teleporter::DrawData()
{
	ImGui::Checkbox("isActive", &isActive_);
	ImGui::Combo("ChangeType", (int*)&changeType_, "CHANGE_TELEPORT\0CHANGE_SCENE\0CHANGE_JSON\0");
	if (changeType_ == CHANGE_TELEPORT) {
		ImGui::DragFloat3("TeleportPos", &teleportPos_.x, 0.1f);
	}
	else if (changeType_ == CHANGE_SCENE) {
		ImGui::InputInt("ChangeSceneID", (int*)&changeSceneID_);
	}
}

void Component_Teleporter::Teleport()
{
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (timer == nullptr)return;
	static bool isEffectEnd = false;
	static bool isEffectNow = false;

	if (isEffectEnd == false) {

		// �^�C�}�[�J�n
		timer->SetTime(3.f);
		timer->Start();

		// �^�[�Q�b�g��WASD��T��(2�ȏ���Ă��Ȃ��z��)
		if (target_ != nullptr) {
			auto inputMove = target_->FindComponent("InputMove");
			inputMove->Stop();
		}

		if (timer->GetIsEnd()) {
			isEffectNow = true;
			timer->Reset();
		}
	}
	
	// �^�C�}�[�I��
	if (isEffectNow == true) {

		// ���o
		if (changeType_ == CHANGE_TELEPORT) {
			// teleportPos_�ւ̍��W�ύX
			target_->SetPosition(teleportPos_);
		}
		else if (changeType_ == CHANGE_SCENE) {
			// ChangeSceneName_�ւ̃V�[���J��
			SceneManager* sceneManager = (SceneManager*)holder_->GetParent()->FindObject("SceneManager");
			sceneManager->ChangeScene(changeSceneID_, TID_BLACKOUT);
		}
		else if (changeType_ == CHANGE_JSON) {
			// JSON�t�@�C���̓Ǎ�
			json loadData;
			if (JsonReader::Load(changeJsonPath_, loadData)) {
				// �X�e�[�W���쐬
				pStage_ = Instantiate<Stage>(holder_->GetParent());
				pStage_->Load(loadData);
			}
		}
		isEffectNow = false;
		isEffectEnd = true;
	}

	if (isEffectEnd == true) {
		// �^�C�}�[�J�n
		timer->SetTime(0.5f);
		timer->Start();

		// �^�C�}�[�I��
		if (timer->GetIsEnd()) {
			// �^�[�Q�b�g��WASD��T��(2�ȏ���Ă��Ȃ��z��)
			if (target_ != nullptr) {
				auto inputMove = target_->FindComponent("InputMove");
				inputMove->Stop();
				timer->Reset();
				isEffectEnd = false;
				isEffectNow = false;
				
			}
		}


	}
}

void Component_Teleporter::Execute(ChangeType _ch)
{
	isActive_ = true;
	changeType_ = _ch;
}
