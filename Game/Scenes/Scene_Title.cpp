#include "Scene_Title.h"

// �C���N���[�h
#include"../../Engine/SceneManager.h"
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title")
{
}

void Scene_Title::Initialize()
{
	// json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load("Datas/UILayouts/titleScene_layout.json", loadData)) {

		// UI�p�l�����擾
		UIPanel* panel = UIPanel::GetInstance();

		// �X�v���b�V���V�[���̃p�l��layout��ݒ�
		panel->Load(loadData);
	}
}
//
void Scene_Title::Update()
{
	// �V�[���ؑ֏���
	{
		// �{�^�����擾
		UIButton* button = (UIButton*)UIPanel::GetInstance()->GetUIObject("startButton");

		// �{�^�����擾�ł�����
		if (button != nullptr) {

			// �{�^���������ꂽ��V�[����؂�ւ���
			if (button->OnClick() == true) {

				// �V�[����؂�ւ���
				SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
				sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
			}
		}
	}

	// debug
	ImGui::Text("Scene_Title");
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
