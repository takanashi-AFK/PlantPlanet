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
#include "../Objects/UI/UIText.h"

using namespace Constants;

namespace {
	// UI���萔
	const string BUTTON_NAME_START		= "startButton";
	const string BUTTON_NAME_CONTINUE	= "continueButton";
	const string BUTTON_NAME_END		= "EndButton";

	const string BUTTON_NAME_OK			= "okButton";
	const string BUTTON_NAME_NO			= "noButton";
	const string IMAGE_POPUP			= "pop-upWindowBackground";

	const string IMAGE_TEXT0			= "text0";
	const string IMAGE_TEXT1			= "text1";
	const string IMAGE_TEXT2			= "text2";
	const string IMAGE_TEXT3			= "text3";
	const string IMAGE_TEXT4			= "text4";

	const string TEXT_USER_NAME			= "text-userName";
	const string TEXT_LIBRARY_STATUS	= "text-libraryStatus";
	const string TEXT_PLAY_TOTAL_TIME	= "text-playTotalTime";
}

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title"), isFirstSelectButton_(true), status_(0)
{
}

void Scene_Title::Initialize()
{
	UserManager::GetInstance().LoadUser("Datas/userData.json");

	// UI�p�l�� & ���C�A�E�g�̓Ǎ�
	json loadData;
	if (JsonReader::Load("Datas/SceneLayout/title.json", loadData)) UIPanel::GetInstance()->Load(loadData);

	UIPanel* uiPanel = UIPanel::GetInstance(); {
		// �{�^���z��̏�����
		uiPanel->ResetArrayOfButton();

		// ����̃{�^����z��ɒǉ�
		uiPanel->PushButtonToArray((UIButton*)uiPanel->GetUIObject(BUTTON_NAME_START));
		uiPanel->PushButtonToArray((UIButton*)uiPanel->GetUIObject(BUTTON_NAME_CONTINUE));
	}

	// �A������Ǎ�
	json plantData;
	if (JsonReader::Load("Datas/PlantData/TentativeFlowers.json", plantData))PlantCollection::Load(plantData);
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

	// ���݁A�I�� ���� ���C���[����ԏ�̃{�^�����擾
	UIButton* selectingButton = UIButton::GetTopSelectingUI(_uiPanel->GetUIObject(UIType::UI_BUTTON));

	// UI�{�^�����������ꂽ�ꍇ...
	if (selectingButton != nullptr && selectingButton->OnClick())
		ProcessButtonAction(_uiPanel, selectingButton->GetObjectName(), uiInputString->GetInputString());
	
	// �p�b�h��A�{�^�����������ꂽ�ꍇ... ( ���C���[�Ɋ֌W�Ȃ� )
	selectingButton = _uiPanel->GetSelectingButton();
	if (selectingButton == nullptr) return;
	
	// �{�^���̃V�F�[�_�[��ύX
	selectingButton->SetShader(Direct3D::SHADER_BUTTON_SELECT);
	
	// �p�b�h��A�{�^�����������ꂽ�ꍇ...
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		ProcessButtonAction(_uiPanel, selectingButton->GetObjectName(), uiInputString->GetInputString());
	
}

void Scene_Title::ProcessButtonAction(UIPanel* _uiPanel,string _buttonName, string _inputUserName)
{
	// ���[�U�[�}�l�[�W���[�̃C���X�^���X���擾
	UserManager& um = UserManager::GetInstance();

	//�{�^�����I���ł��邩�ǂ����ݒ肷��
	auto SetSelectableButton_ContinueAndStart = [&](bool flag) 
		{
			UIButton* startButton = static_cast<UIButton*>(_uiPanel->GetUIObject(BUTTON_NAME_START));
			UIButton* continueButton = static_cast<UIButton*>(_uiPanel->GetUIObject(BUTTON_NAME_CONTINUE));

			if (!(startButton && continueButton)) return;

			startButton->SetSelctable(flag);
			continueButton->SetSelctable(flag);
		};

	//�Q�[���p�b�h�̃{�^���Ŏ��s�����Ƃ��̔z��ꏊ
	int failerButtonX = 0;
	int failerButtonY = 1;

	//�ʏ펞�̏ꏊ
	int defaultButtonX = 0;
	int defaultButtonY = 0;

	// `_status`�̒�`
	// 0: ���[�U�[�������͂���Ă��Ȃ�
	// 1: ���[�U�[�������ɓo�^����Ă���
	// 2: �V�K�f�[�^���쐬����
	// 3: ���ɓo�^����Ă��郆�[�U�[���ŃQ�[�����J�n����
	// 4: �����f�[�^�����݂��Ȃ�

	// �{�^�����ɂ���ď����𕪊�
	if (_buttonName == BUTTON_NAME_START) {

		// ���[�U�[�������͂���Ă��Ȃ��ꍇ...
		if (_inputUserName.empty()) {

			// ���[�U�[�������͂���Ă��Ȃ��|��\��
			SetUIVisible(_uiPanel, { IMAGE_TEXT0, IMAGE_POPUP, BUTTON_NAME_OK }, true);
			_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
			_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

			SetSelectableButton_ContinueAndStart(false);

			status_ = 0;
		}

		// ���[�U�[�������͂���Ă���ꍇ...
		else {

			// �������[�U�[���Ƃ��ēo�^����Ă���ꍇ...
			if (um.isUserRegistered(_inputUserName) == true) {

				// ���[�U�[�������ɓo�^����Ă���|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT1, IMAGE_POPUP, BUTTON_NAME_OK }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

				SetSelectableButton_ContinueAndStart(false);

				status_ = 1;
			}

			// �������[�U�[���Ƃ��ēo�^����Ă��Ȃ��ꍇ...
			else {

				// �V�K�f�[�^���쐬����|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT2, IMAGE_POPUP, BUTTON_NAME_OK, BUTTON_NAME_NO }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

				SetSelectableButton_ContinueAndStart(false);

				status_ = 2;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_CONTINUE) {

		// ���[�U�[�������͂���Ă��Ȃ��ꍇ...
		if (_inputUserName.empty()) {

			// ���[�U�[�������͂���Ă��Ȃ��|��\��
			SetUIVisible(_uiPanel, { IMAGE_TEXT0, IMAGE_POPUP, BUTTON_NAME_OK }, true);
			_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
			_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

			SetSelectableButton_ContinueAndStart(false);

			status_ = 0;
		}

		// ���[�U�[�������͂���Ă���ꍇ...
		else {

			// �������[�U�[���Ƃ��ēo�^����Ă���ꍇ...
			if (um.isUserRegistered(_inputUserName) == true) {

				// ���ɓo�^����Ă��郆�[�U�[���ŃQ�[�����J�n����|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT3, TEXT_USER_NAME, TEXT_LIBRARY_STATUS, TEXT_PLAY_TOTAL_TIME, IMAGE_POPUP, BUTTON_NAME_OK,BUTTON_NAME_NO }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
				SetSelectableButton_ContinueAndStart(false);
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

				// ���[�U�[����K��
				{
					// ���[�U�[����K��
					((UIText*)_uiPanel->GetUIObject(TEXT_USER_NAME))->SetText(_inputUserName);

					// �}�ӂ̊�������K��
					string completenessRateStr = std::to_string(um.GetLibraryCompletenessRate(_inputUserName)) + "%";
					((UIText*)_uiPanel->GetUIObject(TEXT_LIBRARY_STATUS))->SetText(completenessRateStr);

					// �v���C���Ԃ�K��
					string playTotalTimeStr; {
						int totalSec = um.GetPlayTotalTime(_inputUserName) ;

						int hour = totalSec / 3600; totalSec %= 3600;
						int min = (totalSec) / 60; totalSec %= 60;
						int sec = totalSec;

						std::ostringstream oss;
						oss << std::setw(2) << std::setfill('0') << hour << ":"
							<< std::setw(2) << std::setfill('0') << min << ":"
							<< std::setw(2) << std::setfill('0') << sec;

						playTotalTimeStr = oss.str();
					}
					((UIText*)_uiPanel->GetUIObject(TEXT_PLAY_TOTAL_TIME))->SetText(playTotalTimeStr);
				}

				// OK�{�^��,NO�{�^����z��ɒǉ�

				status_ = 3;
			}

			// �������[�U�[���Ƃ��ēo�^����Ă��Ȃ��ꍇ...
			else {

				// �����f�[�^�����݂��Ȃ��|��\��
				SetUIVisible(_uiPanel, { IMAGE_TEXT4, IMAGE_POPUP, BUTTON_NAME_OK }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);
				SetSelectableButton_ContinueAndStart(false);
				status_ = 4;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_OK) {
		
		_uiPanel->SetButtonArrayIndex(defaultButtonX, defaultButtonY);

		// ��Ԃɂ���ď����𕪊�
		switch (status_) 
		{
		case 0: ClosePopup(_uiPanel);					break;
		case 1: ClosePopup(_uiPanel);					break;
		case 2: GameStart(&um,_inputUserName,true);		break;
		case 3: GameStart(&um, _inputUserName, false);	break;
		case 4: ClosePopup(_uiPanel);					break;
		}

		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
		SetSelectableButton_ContinueAndStart(true);
	}

	else if (_buttonName == BUTTON_NAME_NO) {

		_uiPanel->SetButtonArrayIndex(defaultButtonX, defaultButtonY);
		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
		SetSelectableButton_ContinueAndStart(true);
		ClosePopup(_uiPanel);
	}

	else if (_buttonName == BUTTON_NAME_END) {

		// �Q�[�����I������
		PostQuitMessage(0);
	}
}

void Scene_Title::GameStart(UserManager* _userManager, string _userName, bool _isNewUser)
{
	// �V�K���[�U�[�̏ꍇ�̓��[�U�[��o�^
	if(_isNewUser)_userManager->RegisterUser(_userName);

	// ���[�U�[�����O�C��
	_userManager->LoginUser(_userName);

	// �V�[����؂�ւ���
	SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
	sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
}

void Scene_Title::SetUIVisible(UIPanel* _uiPanel, vector<string> _uiObjectNames, bool _visible)
{
	// �w�肳�ꂽUI�I�u�W�F�N�g��\��/��\���ɂ���
	for (auto& uiObjectName : _uiObjectNames) {
		_uiPanel->GetUIObject(uiObjectName)->SetVisible(_visible);
	}
}

void Scene_Title::ClosePopup(UIPanel* _uiPanel)
{
	SetUIVisible(_uiPanel, {
		BUTTON_NAME_OK,
		BUTTON_NAME_NO, 
		IMAGE_POPUP,
		IMAGE_TEXT0,
		IMAGE_TEXT1,
		IMAGE_TEXT2,
		IMAGE_TEXT3,
		IMAGE_TEXT4,
		TEXT_USER_NAME,
		TEXT_LIBRARY_STATUS,
		TEXT_PLAY_TOTAL_TIME
	}, false);
}
