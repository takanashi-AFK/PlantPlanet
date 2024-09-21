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
	UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
	input->SetInputString(ScoreManager::userName);
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

		// �v���C�V�[���ւ̑J��
		UIButton* startButton = (UIButton*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_START_BUTTON_NAME);
		if (startButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

			// �����Ń��[�U�[�����擾����
			UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
			ScoreManager::userName = input->GetInputString();

			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_DIFFICULTYSELECT, TID_BLACKOUT);
		}
	}
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
