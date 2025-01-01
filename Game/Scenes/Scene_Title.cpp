#include "Scene_Title.h"

// �C���N���[�h
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"

#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIInputString.h"
using namespace Constants;

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title"), isFirstSelectButton_(true)
{
}

void Scene_Title::Initialize()
{
	// UI�p�l�� & ���C�A�E�g�̓ǂݍ���
	json loadData;
	if (JsonReader::Load("Datas/Test/Prottype_titleScene_Layout.json", loadData)) UIPanel::GetInstance()->Load(loadData);

	UIPanel* uiPanel = UIPanel::GetInstance();
	isFirstSelectButton_ = true;
}

void Scene_Title::Update()
{
	// �V�[���ؑ֏���
	{
		UIPanel* uiPanel = UIPanel::GetInstance();

		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
			// �ŏ��̓��͂�������
			if (isFirstSelectButton_ == true) {
				uiPanel->SetSelectingButton(0, 0);
				isFirstSelectButton_ = false;
			}
			else
			uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		}
		else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			if (isFirstSelectButton_ == true) {
				uiPanel->SetSelectingButton(0, 0);
				isFirstSelectButton_ = false;
			}
			else
			uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		}

		for (auto button : uiPanel->GetUIObjects()) {
			// ���O��startButton�̃{�^�����������炩�A�I�𒆂̃{�^�����Ȃɂ���������
			if (((UIButton*)button)->GetObjectName() == "startButton" && uiPanel->GetSelectingButton() != nullptr) {
				//selectingButton_��startButton�ŁA����A�{�^���������ꂽ�� or �{�^�����}�E�X�ɂ���ĉ����ꂽ��
				if (uiPanel->GetSelectingButton()->GetObjectName() == ((UIButton*)button)->GetObjectName() && Input::IsPadButtonDown(XINPUT_GAMEPAD_A) ||
					((UIButton*)button)->OnClick()) {
					// �V�[���}�l�[�W���[���擾���ăV�[����ύX
					SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
					sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
				}
			}
			else if (((UIButton*)button)->GetObjectName() == "EndButton" && uiPanel->GetSelectingButton() != nullptr) {
				//selectingButton_��EndButton�ŁA����A�{�^���������ꂽ�� or �{�^�����}�E�X�ɂ���ĉ����ꂽ��
				if (uiPanel->GetSelectingButton()->GetObjectName() == ((UIButton*)button)->GetObjectName() && Input::IsPadButtonDown(XINPUT_GAMEPAD_A) ||
					((UIButton*)button)->OnClick()) {
					PostQuitMessage(0);

				}
			}
		}

		// �{�^���������ꂽ��
		if (((UIButton*)UIPanel::GetInstance()->GetUIObject("EndButton"))->OnClick()) {
			PostQuitMessage(0);
		}
	}

}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
