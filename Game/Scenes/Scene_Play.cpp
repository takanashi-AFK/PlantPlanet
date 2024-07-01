#include "Scene_Play.h"

// �C���N���[�h
#include"../Objects/Stage/SkySphere.h"
#include"../Objects/Stage/Stage.h"
#include "../Objects/UI/UIPanel.h"
#include"../Objects/Stage/Components/HealthManagerComponents/Component_HealthManager.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIButton.h"
#include "../Objects/Camera/TPSCamera.h"

Scene_Play::Scene_Play(GameObject* parent)
{
}

void Scene_Play::Initialize()
{
	// �X�J�C�X�t�B�A�𐶐�
	Instantiate<SkySphere>(this);

	// �X�e�[�W�𐶐�
	Stage* pStage = Instantiate<Stage>(this); {
		// �X�e�[�W��ǂݍ���
		json stageData;
		JsonReader::Load("Datas/StageLayouts/stage_Test00_sugawara.json", stageData);
		pStage->Load(stageData);
	}
	//	//�͈�for���ŃI�u�W�F�N�g���X�g�̎擾
	//	for (auto pList : pStage->GetStageObjects()) {
	//		//���X�g���łƂ��낭����Ă���PlayerBehavior����������R���|�[�l���g���L���X�g���đ��
	//		if (pList->FindComponent("PlayerBehavior")) {
	//			Component_PlayerBehavior* playerBeha = dynamic_cast<Component_PlayerBehavior*>(pList->FindComponent("PlayerBehavior"));
	//			if (playerBeha != nullptr) {
	//				//playerBeha����q�R���|�[�l���g���擾����
	//				playerHealth_ = dynamic_cast<Component_HealthManager*>(playerBeha->GetChildComponent("HealthManager"));
	//			}
	//		}
	//	}
	//}

	//TPSCamera* tpsCamera = Instantiate<TPSCamera>(this); {
	//	json camData;
	//	if (JsonReader::Load("Datas/CameraLayouts/TPSCamera.json", camData)) {
	//		tpsCamera->Load(camData);
	//	}
	//}
}

void Scene_Play::Update()
{
	////HP��0�ɂȂ�����G���h�V�[���Ɉڍs
	//if (playerHealth_->GetHP() == 0) {
	//	SceneManager* pChangeScene = (SceneManager*)FindObject("SceneManager");
	//	pChangeScene->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
	//}
}

void Scene_Play::Draw()
{
}

void Scene_Play::Release()
{
}
