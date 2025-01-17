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
	if (JsonReader::Load("Datas/SceneLayout/title.json", loadData)) UIPanel::GetInstance()->Load(loadData);

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
				uiPanel->SetButtonArrayIndex(0, 0);
				isFirstSelectButton_ = false;
			}
			else
			uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		}
		else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			if (isFirstSelectButton_ == true) {
				uiPanel->SetButtonArrayIndex(0, 0);
				isFirstSelectButton_ = false;
			}
			else
			uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		}

		for (auto button : uiPanel->GetUIObject(UI_BUTTON)) {
			UIButton* uiButton = static_cast<UIButton*>(button);
			 string buttonName;
			// �{�^�������擾
			if (uiButton != nullptr) {
				 buttonName = uiButton->GetObjectName();
			}

			// �}�E�X�ŃN���b�N���ꂽ�ꍇ�̏���
			if (uiButton->OnClick()) {
				if (buttonName == "startButton") {
					SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
					sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
				}
				else if (buttonName == "EndButton") {
					PostQuitMessage(0);
				}
				continue; // ���̏������`�F�b�N���Ȃ�
			}

			UIButton* buttons = uiPanel->GetSelectingButton();
				
			// �p�b�h�ł̑I����Ԃ�A�{�^���̏���
			if (buttons != nullptr )
				if(uiPanel->GetSelectingButton()->GetObjectName() == buttonName &&
					Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {
				if (buttonName == "startButton") {
					SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
					sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
				}
				else if (buttonName == "EndButton") {
					PostQuitMessage(0);
				}
			}
		}


	}

}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
