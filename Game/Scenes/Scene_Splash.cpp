#include "Scene_Splash.h"

// �C���N���[�h
#include "../../Engine/Global.h"
#include "../../Engine/ResourceManager/Image.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"

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
	////// UI�p�l���̐���
	//UIPanel* pUIPanel = Instantiate<UIPanel>(this); {
	//	json uiData;
	//	if (JsonReader::Load("Datas/UILayouts/SplashScene.json", uiData))pUIPanel->Load(uiData);
	//}
}

void Scene_Splash::Update()
{
	//// n�b�o�߂�����V�[�����ړ�
	//if (count_ >= LIMIT_TIME * FPS)((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID_TITLE, TID_BLACKOUT);
	//
	//// �J�E���g�A�b�v
	//count_++;
}

void Scene_Splash::Draw()
{
}

void Scene_Splash::Release()
{
}
