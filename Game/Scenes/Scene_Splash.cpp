#include "Scene_Splash.h"

// �C���N���[�h
#include "../../Engine/Global.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"

// debug
#include "../../Engine/ImGui/imgui.h"

// �萔
namespace {
	const float LIMIT_TIME = 1.f;				// �V�[����؂�ւ���܂łɊ|���鎞�ԁi�b�j
	const float IMAGE_REDUCTION_RATE = 0.67f;	// �X�v���b�V���摜�̏k���{��
}

Scene_Splash::Scene_Splash(GameObject* _parent)
	:GameObject(_parent,"Scene_Splash"),count_()
{
}

void Scene_Splash::Initialize()
{
	// json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load("Datas/UILayouts/splashScene_layout.json", loadData)) {

		// UI�p�l�����擾
		UIPanel* panel = UIPanel::GetInstance();

		// �X�v���b�V���V�[���̃p�l��layout��ݒ�
		panel->Load(loadData);
	}
}

void Scene_Splash::Update()
{
	// �V�[���ؑ֏���
	{
		// ��莞�Ԍo�߂�����V�[����؂�ւ���
		if (count_ >= LIMIT_TIME * FPS) {

			// �V�[����؂�ւ���
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_TITLE,TID_BLACKOUT);

		}

		// �o�ߎ��Ԃ����Z
		count_++;
	}

	// debug
	ImGui::Text("Scene_Splash");
}

void Scene_Splash::Draw()
{
}

void Scene_Splash::Release()
{
}
