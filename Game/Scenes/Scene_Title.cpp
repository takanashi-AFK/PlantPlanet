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
	: GameObject(parent, "Scene_Title")
{
}

void Scene_Title::Initialize()
{
	// UI�p�l�� & ���C�A�E�g�̓ǂݍ���
	json loadData;
	if (JsonReader::Load(TITLE_SCENE_UI_LAYOUT_JSON, loadData)) UIPanel::GetInstance()->Load(loadData);

	// ���[�U�[���̏�����
	/*UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
	input->SetInputString(ScoreManager::userName);*/
}

void Scene_Title::Update()
{

	// �V�[���ؑ֏���
	{
		// �����L���O�V�[���ւ̑J��
		UIButton* rankingButton = (UIButton*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_RANKING_BUTTON_NAME);
		if (rankingButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_X)) {
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_RANKING, TID_BLACKOUT);
		}

		// �A�h�x���`���[�փV�[���J��
		UIButton* adventureStartButton = (UIButton*)UIPanel::GetInstance()->GetUIObject("ADV_StartButton");
		if (adventureStartButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

			// �����Ń��[�U�[�����擾����
			UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
			ScoreManager::userName = input->GetInputString();

			g_selectedGameMode = 1;

			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
		}

		// �X�R�A�A�^�b�N�փV�[���J��
		UIButton* scoreAttackStartButton = (UIButton*)UIPanel::GetInstance()->GetUIObject("SCA_StartButton");
		if (scoreAttackStartButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

			// �����Ń��[�U�[�����擾����
			UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
			ScoreManager::userName = input->GetInputString();

			g_selectedGameMode = 2;

			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
		}
	}
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
