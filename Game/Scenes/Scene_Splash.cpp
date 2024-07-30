#include "Scene_Splash.h"

// �C���N���[�h
#include "../../Engine/Global.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"

// �萔
namespace {
	const float LIMIT_TIME = 1.f;				// �V�[����؂�ւ���܂łɊ|���鎞�ԁi�b�j
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

}

void Scene_Splash::Draw()
{
}

void Scene_Splash::Release()
{
}
