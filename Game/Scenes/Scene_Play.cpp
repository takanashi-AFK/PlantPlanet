#include "Scene_Play.h"

// �C���N���[�h
#include"../Objects/Stage/SkySphere.h"
#include"../Objects/Stage/Stage.h"
#include"../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIButton.h"
#include "../Objects/Camera/TPSCamera.h"
#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/Global.h"
#include "../../Engine/ResourceManager/Image.h"
#include "../Objects/UI/CountDown.h"
#include "../../Engine/DirectX/Input.h"


Scene_Play::Scene_Play(GameObject* parent)
{
}

void Scene_Play::Initialize()
{
	UIPanel* panel_ = UIPanel::GetInstance();
	json panelData;
	if (JsonReader::Load("test_playerHPBar.json", panelData)) {
		panel_->Load(panelData);
	}

	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	// stageLayout_json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load("Stage01.json", loadData)) {

		// �X�e�[�W���쐬
		pStage_ = Instantiate<Stage>(this);
		pStage_->Load(loadData);
	}

	// �J�������̓ǂݍ���
	json camData;
	if (JsonReader::Load("Datas/CameraLayouts/TPSCamera.json", camData)) {

		tpsCamera_ = Instantiate<TPSCamera>(this);
		tpsCamera_->Load(camData);
		tpsCamera_->SetActive(false);
	}

	countDown_ = Instantiate<CountDown>(this);
}

void Scene_Play::Update()
{
	TPSCamera* tpsCamera_ = (TPSCamera*)FindObject("TPSCamera");
	if (tpsCamera_ == nullptr)return;


	// �J�[�\���Œ艻����
	static bool fixedCursorPos = false; {

		// �Œ艻�̐؂�ւ�
		if (Input::IsKeyDown(DIK_F3))fixedCursorPos = !fixedCursorPos;
		
		// �J�[�\���̈ʒu�𒆉��ɌŒ�
		if (fixedCursorPos) {
			SetCursorPos(Direct3D::screenWidth_ / 2, Direct3D::screenHeight_ / 2);
		}
	}
	
	if (countDown_->IsFinished() && isGameStart_ == false) {

		// �J�[�\���̈ʒu�𒆉��ɌŒ�
		fixedCursorPos = true;
		
		// �J�����̃A�N�e�B�u��
		tpsCamera_->SetActive(true);

		isGameStart_ = true;
	}

	// �V�[���ؑ֏���
	//{
	//	bool isSceneChange = false;
	//	StageObject* player = nullptr;
	//	static Component* playerHealthGauge = nullptr;
	//	static Component* bossHealthGauge = nullptr;
	//	// �v���C���[�����񂾂�V�[����؂�ւ���
	//	{
	//		// �X�e�[�W���Ƀv���C���[�R���|�[�l���g�������Ă���L�����N�^�[�����݂��邩�ǂ����𔻒肵�擾
	//		vector<Component*> comp_playerBehaviors = pStage_->FindComponents(ComponentType::PlayerBehavior);

	//		// �X�e�[�W���Ƀ{�X�R���|�[�l���g�������Ă���L�����N�^�[�����݂��邩�ǂ����𔻒肵�擾
	//		vector<Component*> comp_bossBehaviors = pStage_->FindComponents(ComponentType::BossBehavior);

	//		// �͈�for���Ń{�X�R���|�[�l���g�̐����t���O���m�F
	//		for (auto comp : comp_bossBehaviors) {

	//			vector<Component*> bHealthGaugeList = comp->GetChildComponent(ComponentType::HealthGauge);

	//			for (auto bhg : bHealthGaugeList) {
	//				bossHealthGauge = bhg;
	//			}
	//		}


	//		// �͈�for���Ńv���C���[�R���|�[�l���g�̐����t���O���m�F
	//		for (auto comp : comp_playerBehaviors) {

	//			vector<Component*> pHealthGaugeList = comp->GetChildComponent(ComponentType::HealthGauge);

	//			if (comp != nullptr && player == nullptr) {
	//				player = comp->GetHolder();
	//			}

	//			for (auto hg : pHealthGaugeList) {
	//				playerHealthGauge = hg;
	//			}
	//		}

	//		if (((Component_HealthGauge*)playerHealthGauge)->IsDead() || ((Component_HealthGauge*)bossHealthGauge)->IsDead()) {
	//			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
	//			// �V�[����؂�ւ���
	//			sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
	//			player = nullptr;
	//			isGameStart_ = false;
	//		}


	//		if (player != nullptr) {
	//			tpsCamera_->SetTarget(player);
	//		}
	//	}
	//	//// debug
	//	//{
	//	//	if (ImGui::Button("end")) {
	//	//		g_score = 100;
	//	//		isSceneChange = true;
	//	//	}
	//	//}

	//	//// �V�[���ؑ֏���
	//	//if (isSceneChange) {
	//	//	// �V�[����؂�ւ���
	//	//	SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
	//	//	sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
	//	//}
	//}

}

void Scene_Play::Draw()
{
}
void Scene_Play::Release()
{
}

// ���ƃ{�X�����񂾍ۂ̃V�[���؂�ւ��ł��ĂȂ�����m�F����
// ���������v���C���[�Ɠ���ւ���Ƃ��Ȃ񂩂���Ȋ�����