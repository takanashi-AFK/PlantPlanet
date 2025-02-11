#include "Scene_Splash.h"

// �C���N���[�h
#include "../../Engine/Global.h"
#include "../../Engine/Json/JsonReader.h"
#include "../../Engine/SceneManager.h"
#include "../Constants.h"
#include "../Objects/UI/UIPanel.h"
#include "../Plants/PlantCollection.h"

using namespace Constants;

// �萔
namespace {
	const float LIMIT_TIME = 3.f;				// �V�[����؂�ւ���܂łɊ|���鎞�ԁi�b�j
}

Scene_Splash::Scene_Splash(GameObject* _parent)
	:GameObject(_parent,"Scene_Splash"),count_()
{
}

void Scene_Splash::Initialize()
{
	// UI�p�l�� & ���C�A�E�g�̓ǂݍ���
	json loadData;
	if (JsonReader::Load(SPLASH_SCENE_UI_LAYOUT_JSON, loadData))UIPanel::GetInstance()->Load(loadData);

	// �A���f�[�^�̓ǂݍ���
	if(JsonReader::Load(PLANT_DATA_JSON, loadData))PlantCollection::Load(loadData);
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
