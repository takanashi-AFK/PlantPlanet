#include "Component_Teleporter.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../../StageObject.h"
#include "../../../../../Engine/GameObject/GameObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../../../../../Engine/Global.h"
#include "../MoveComponents/Component_WASDInputMove.h"
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
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (timer == nullptr)return;
	//// PlayerBehavior�����I�u�W�F�N�g������
	//auto playerList = ((Stage*)holder_->GetParent())->GetStageObjects();
	//for (auto a : playerList) {

	//	StageObject* pl = a;
	//	if (pl != nullptr) {

	//		if (pl->FindComponent("PlayerBehavior")) {
	//			playerBehavior_ = pl->FindComponent("PlayerBehavior");
	//			target_ = pl;
	//			break;
	//		}
	//	}
	//}

	// ���o�Ώۂ̐ݒ�
	detector->SetTarget(target_);

	if (detector->IsContains()) {
		Teleport();
	}

	if (isTeleport_ == true) {
		// �^�C�}�[�J�n
		timer->SetTime(0.5f);
		timer->Start();

		// �^�C�}�[�I��
		if (timer->GetIsEnd()) {
			// �^�[�Q�b�g��WASD��T��(2�ȏ���Ă��Ȃ��z��)
			if (target_ != nullptr) {
				auto inputMove = target_->FindComponent(WASDInputMove);
				if (inputMove.empty())return;
				for (auto iMove : inputMove) {
					iMove->Execute();
					break;
				}
			}
			timer->Reset();
			isTeleport_ = false;
		}
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
	else if (changeType_ == CHANGE_JSON) {
		if (ImGui::Button("...")) {

			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
			{
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter = TEXT("json�t�@�C��(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					changeJsonPath_ = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					changeJsonPath_ = FileManager::GetAssetsRelativePath(changeJsonPath_);

					// ���������"\\"��"/"�ɒu��
					FileManager::ReplaceBackslashes(changeJsonPath_);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					return;
				}
			}ImGui::SameLine();
		}
			ImGui::Text(changeJsonPath_.c_str());

	}

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
	else target_ = (StageObject*)holder_->FindObject(objNames[select]);
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

		// �^�[�Q�b�g��WASD��T��
		if (target_ != nullptr) {
			auto inputMove = target_->FindComponent(WASDInputMove);
			if (inputMove.empty())return;
			for (auto iMove : inputMove) {
				iMove->Stop();
				break;
			}
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
		//else if (changeType_ == CHANGE_SCENE) {
		//	// ChangeSceneName_�ւ̃V�[���J��
		//	SceneManager* sceneManager = (SceneManager*)holder_->GetParent()->FindObject("SceneManager");
		//	sceneManager->ChangeScene(changeSceneID_, TID_BLACKOUT);
		//}
		else if (changeType_ == CHANGE_JSON) {
			// JSON�t�@�C���̓Ǎ�

			json loadData;
			if (JsonReader::Load(changeJsonPath_, loadData)) {
				// �X�e�[�W���쐬
				json loadObj;
				if (JsonReader::Load(changeJsonPath_, loadObj) == false) MessageBox(NULL, "�Ǎ��Ɏ��s���܂����B", 0, 0);
				((Stage*)holder_->GetParent())->Load(loadObj);
			}
		}
		isEffectNow = false;
		isTeleport_ = true;
	}


}

void Component_Teleporter::Execute(ChangeType _ch)
{
	isActive_ = true;
	changeType_ = _ch;
}
