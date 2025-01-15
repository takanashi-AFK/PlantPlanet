#include "Scene_Title.h"

// �C���N���[�h
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"

#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIInputString.h"

#include "../Otheres/UserManager.h"

using namespace Constants;

namespace {
	// UI���萔
	const string BUTTON_NAME_START = "startButton";
	const string BUTTON_NAME_CONTINUE = "continueButton";
	const string BUTTON_NAME_END = "EndButton";
	const string BUTTON_NAME_OK = "okButton";
	const string BUTTON_NAME_NO = "noButton";
	const string IMAGE_POPUP = "pop-upWindowBackground";

	const string IMAGE_TEXT0 = "text0";
	const string IMAGE_TEXT1 = "text1";
	const string IMAGE_TEXT2 = "text2";
	const string IMAGE_TEXT4 = "text4";
}

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
	// UI�p�l���̎擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UI���͏���
	HandleUIInput(uiPanel, isFirstSelectButton_);

	// ���[�U�[�����̓t�H�[�����當������擾
	UIInputString* uiInputString = (UIInputString*)uiPanel->GetUIObject(UIType::UI_INPUTSTRING)[0];
	if (uiInputString == nullptr) return;

	// debug code
	ImGui::Begin("userData"); {
		UserManager& um = UserManager::GetInstance();

		for (auto& userData : um.GetAllUsers()) {
			ImGui::Text(userData.first.c_str());
			ImGui::SameLine();
			ImGui::Text(std::to_string(userData.second->bestScore).c_str());
		}

		ImGui::Separator();
		ImGui::Text("Logged in user:");
		ImGui::Text(um.isUserLoggedIn() ? um.GetLoggedInUser()->userName.c_str() : "none");

		ImGui::Separator();
		if (ImGui::Button("+")) um.RegisterUser(uiInputString->GetInputString()); ImGui::SameLine();
		if (ImGui::Button("-")) um.DeleteUser(uiInputString->GetInputString());
		if (ImGui::Button("login")) um.LoginUser(uiInputString->GetInputString()); ImGui::SameLine();
		if (ImGui::Button("logout")) um.LogoutUser();
		if (ImGui::Button("save")) um.SaveUser("Datas/userData.json"); ImGui::SameLine();
		if (ImGui::Button("load")) um.LoadUser("Datas/userData.json");

	} ImGui::End();
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}

void Scene_Title::HandleUIInput(UIPanel* _uiPanel, bool& _isFirstSelectButton)
{
	// ���[�U�[�����̓t�H�[�����當������擾
	UIInputString* uiInputString = (UIInputString*)_uiPanel->GetUIObject(UIType::UI_INPUTSTRING)[0];
	if (uiInputString == nullptr) return;

	// �R���g���[���[���͂ɂ�艟������{�^����I��
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {

			// �V�[�����ŏ��̓��͂̏ꍇ...
			if (_isFirstSelectButton == true) {

				// �{�^���̑I���ʒu��������
				_uiPanel->SetButtonArrayIndex(0, 0);

				// �V�[�����ŏ��̓��̓t���O��OFF
				_isFirstSelectButton = false;
			}

			// �{�^���̑I���ʒu�����Ɉړ�
			else _uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
			
		}
		else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {

			// �V�[�����ŏ��̓��͂̏ꍇ...
			if (_isFirstSelectButton == true) {

				// �{�^���̑I���ʒu��������
				_uiPanel->SetButtonArrayIndex(0, 0);

				// �V�[�����ŏ��̓��̓t���O��OFF
				_isFirstSelectButton = false;
			}

			// �{�^���̑I���ʒu���E�Ɉړ�
			else _uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		}
	}

	// �{�^�����������ꂽ���𔻒肵�A�{�^���̃A�N�V���������s
	for (auto uiObject : _uiPanel->GetUIObject(UIType::UI_BUTTON)) {

		// �{�^�����������ꂽ�ꍇ...
		UIButton* uiButton = static_cast<UIButton*>(uiObject);
		if (uiButton->OnClick()) 
			ProcessButtonAction(_uiPanel,uiButton->GetObjectName(), uiInputString->GetInputString());

		// �p�b�h��A�{�^�����������ꂽ�ꍇ...
		if(Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) 
			ProcessButtonAction(_uiPanel,_uiPanel->GetSelectingButton()->GetObjectName(), uiInputString->GetInputString());
	}
}

void Scene_Title::ProcessButtonAction(UIPanel* _uiPanel,string _buttonName, string _inputUserName)
{
	// `_status`�̒�`
	// 0: ���[�U�[�������͂���Ă��Ȃ�
	// 1: ���[�U�[�������ɓo�^����Ă���
	// 2: �V�K�f�[�^���쐬����
	// 3: ���ɓo�^����Ă��郆�[�U�[���ŃQ�[�����J�n����
	// 4: �����f�[�^�����݂��Ȃ�

	// ���[�U�[�}�l�[�W���[�̃C���X�^���X���擾
	UserManager& um = UserManager::GetInstance();

	// �{�^�����ɂ���ď����𕪊�
	if (_buttonName == BUTTON_NAME_START) {

		// ���[�U�[�������͂���Ă��Ȃ��ꍇ...
		if (_inputUserName.empty()) {

			// ���[�U�[�������͂���Ă��Ȃ��|��\��
			SetUIVisible(_uiPanel, { IMAGE_TEXT0, IMAGE_POPUP, BUTTON_NAME_OK }, true);
			status_ = 0;
		}

		// ���[�U�[�������͂���Ă���ꍇ...
		else {

			// �������[�U�[���Ƃ��ēo�^����Ă���ꍇ...
			if (um.isUserRegistered(_inputUserName) == true) {

				// ���[�U�[�������ɓo�^����Ă���|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT1, IMAGE_POPUP, BUTTON_NAME_OK }, true);
				status_ = 1;
			}

			// �������[�U�[���Ƃ��ēo�^����Ă��Ȃ��ꍇ...
			else {

				// �V�K�f�[�^���쐬����|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT2, IMAGE_POPUP, BUTTON_NAME_OK, BUTTON_NAME_NO }, true);
				status_ = 2;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_CONTINUE) {

		// ���[�U�[�������͂���Ă��Ȃ��ꍇ...
		if (_inputUserName.empty()) {

			// ���[�U�[�������͂���Ă��Ȃ��|��\��
			SetUIVisible(_uiPanel, { IMAGE_TEXT0, IMAGE_POPUP, BUTTON_NAME_OK }, true);
			status_ = 0;
		}

		// ���[�U�[�������͂���Ă���ꍇ...
		else {

			// �������[�U�[���Ƃ��ēo�^����Ă���ꍇ...
			if (um.isUserRegistered(_inputUserName) == true) {

				// ���ɓo�^����Ă��郆�[�U�[���ŃQ�[�����J�n����|��\��
				status_ = 3;
			}

			// �������[�U�[���Ƃ��ēo�^����Ă��Ȃ��ꍇ...
			else {

				// �����f�[�^�����݂��Ȃ��|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT4, IMAGE_POPUP, BUTTON_NAME_OK }, true);
				status_ = 4;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_OK) {
		
		switch (status_) 
		{
		case 0: ClosePopup(_uiPanel); break;
		case 1: ClosePopup(_uiPanel); break;
		case 2: {
			um.RegisterUser(_inputUserName);
			um.LoginUser(_inputUserName);
			SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
			sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
		}break;
		case 3: {
			um.LoginUser(_inputUserName);
			SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
			sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
		}break;
		case 4: ClosePopup(_uiPanel); break;
		}
	}

	else if (_buttonName == BUTTON_NAME_NO) {

		// �|�b�v�A�b�v�����
		ClosePopup(_uiPanel);
	}

	else if (_buttonName == BUTTON_NAME_END) {

		// �Q�[�����I������
		PostQuitMessage(0);
	}
}

void Scene_Title::SetUIVisible(UIPanel* _uiPanel, vector<string> _uiObjectNames, bool _visible)
{
	for (auto& uiObjectName : _uiObjectNames) {
		_uiPanel->GetUIObject(uiObjectName)->SetVisible(_visible);
	}
}

void Scene_Title::ClosePopup(UIPanel* _uiPanel)
{
	SetUIVisible(_uiPanel, { 
		IMAGE_TEXT0, 
		IMAGE_TEXT1, 
		IMAGE_TEXT2, 
		IMAGE_POPUP, 
		BUTTON_NAME_OK, 
		BUTTON_NAME_NO }, false
	);
}
