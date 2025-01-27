#include "Scene_End.h"

// �C���N���[�h
#include "../../Engine/SceneManager.h"
#include "../Constants.h"
#include "../Objects/UI/UIPanel.h"

using namespace Constants;

// �萔
namespace {
	const float LIMIT_TIME = 3.f;				// �V�[����؂�ւ���܂łɊ|���鎞�ԁi�b�j
}


Scene_End::Scene_End(GameObject* parent_)
	: GameObject(parent_, "Scene_End"), count_()
{
}

void Scene_End::Initialize()
{
	// UI�p�l�� & ���C�A�E�g�̓ǂݍ���
	json loadData;
	if(JsonReader::Load(END_SCENE_UI_LAYOUT_JSON,loadData))UIPanel::GetInstance()->Load(loadData);
}

void Scene_End::Update()
{
	// �V�[���ؑ֏���
	{
		// ��莞�Ԍo�߂�����V�[����؂�ւ���
		if (count_ >= LIMIT_TIME * FPS) {

			//// �V�[����؂�ւ���
			//SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			//sceneManager->ChangeScene(SCENE_ID_TITLE, TID_BLACKOUT);
			PostQuitMessage(0);
		}

		// �o�ߎ��Ԃ����Z
		count_++;
	}
}

void Scene_End::Draw()
{
}

void Scene_End::Release()
{
}
