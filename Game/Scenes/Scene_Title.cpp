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
	if (JsonReader::Load("Datas/Test/Prottype_titleScene_Layout.json", loadData)) UIPanel::GetInstance()->Load(loadData);
}

void Scene_Title::Update()
{
	// �V�[���ؑ֏���
	{
		// �{�^���������ꂽ��
		if (((UIButton*)UIPanel::GetInstance()->GetUIObject("startButton"))->OnClick() || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
		}

		// �{�^���������ꂽ��
		if (((UIButton*)UIPanel::GetInstance()->GetUIObject("EndButton"))->OnClick() || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {
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
