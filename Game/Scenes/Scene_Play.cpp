#include "Scene_Play.h"

#include"../Objects/Stage/SkySphere.h"
#include"../Objects/Stage/Stage.h"
#include"../Otheres/GameEditor.h"
#include"../../Engine/ImGui/imgui.h"
#include"../Objects/Stage/Components/HealthManagerComponents/Component_HealthManager.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include"../../Engine/SceneManager.h"
Scene_Play::Scene_Play(GameObject* parent)
{
}

void Scene_Play::Initialize()
{
	Instantiate<SkySphere>(this);

	Stage* pStage = Instantiate<Stage>(this);

	// �X�e�[�W��ǂݍ���
	json stageData;
	JsonReader::Load("Datas/stage_Test00_sugawara.json", stageData);
	pStage->Load(stageData);
	
	//�͈�for���ŃI�u�W�F�N�g���X�g�̎擾
	for (auto pList : pStage->GetStageObjects()){
		//���X�g���łƂ��낭����Ă���PlayerBehavior����������R���|�[�l���g���L���X�g���đ��
		if (pList->FindComponent("PlayerBehavior")) {
			Component_PlayerBehavior* playerBeha = dynamic_cast<Component_PlayerBehavior*>(pList->FindComponent("PlayerBehavior"));
			if (playerBeha != nullptr) {
				//playerBeha����q�R���|�[�l���g���擾����
				health_ =dynamic_cast<Component_HealthManager*>(playerBeha->GetChildComponent("HealthManager"));
			}
		}
	}

#ifdef _DEBUG

	// �X�e�[�W�G�f�B�^�[�����s
	GameEditor* pSe = Instantiate<GameEditor>(this);
	
	// �ҏW�Ώۂ�ݒ�
	pSe->SetEditStage(pStage);

#endif // _DEBUG

}

void Scene_Play::Update()
{
	//HP��0�ɂȂ�����G���h�V�[���Ɉڍs
	if (health_->GetHP() == 0) {
		SceneManager* pChangeScene = (SceneManager*)FindObject("SceneManager");
		pChangeScene->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
	}
	
	
}

void Scene_Play::Draw()
{
}

void Scene_Play::Release()
{
}
