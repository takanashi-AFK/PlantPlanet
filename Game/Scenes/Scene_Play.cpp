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
#include "../Objects/Stage/Components/BehaviorComponents/Component_BossBehavior.h"
#include "../Objects/UI/UITimer.h"


Scene_Play::Scene_Play(GameObject* parent)
{
}

void Scene_Play::Initialize()
{
	// �p�l���̐���
	UIPanel* panel_ = UIPanel::GetInstance();
	json panelData;
	if (JsonReader::Load("test_playerHPBar.json", panelData)) {
		panel_->Load(panelData);
	}

	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	// stageLayout_json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load("Stage02.json", loadData)) {

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
	UITimer* uiTimer =  (UITimer*)UIPanel::GetInstance()->FindObject("Timer");

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

		// �Q�[���J�n�t���O�𗧂Ă�
		isGameStart_ = true;

		// �^�C�}�[�̊J�n
		if (uiTimer != nullptr)uiTimer->StartTimer();
	}


	// �V�[���ؑ֏���
	{
		// �V�[���ؑփt���O��p��
		bool isSceneChange = false;
		
		// �v���C���[�����񂾏ꍇ�A�ؑփt���O�𗧂Ă�
		for (auto playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))if (((Component_PlayerBehavior*)playerBehavior)->IsDead()) isSceneChange = true;
		
		// �{�X�����񂾏ꍇ�A�ؑփt���O�𗧂Ă�
		for (auto bossBehavior : pStage_->FindComponents(ComponentType::BossBehavior)) if (((Component_BossBehavior*)bossBehavior)->IsDead()){ScoreManager::isClear = true;isSceneChange = true;}
		
		// �^�C�}�[���I�������ꍇ�A�ؑփt���O�𗧂Ă�
		
		if(uiTimer != nullptr)if (uiTimer->IsEnd())isSceneChange = true;

		// �V�[���ؑփt���O�������Ă���ꍇ
		if (isSceneChange == true) {
			
			// �^�C�}�[�̍ŏI�l���擾
			if (uiTimer != nullptr)ScoreManager::time = uiTimer->GetSeconds();
			
			// �V�[����؂�ւ���
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT);
		}
	}

	// �J�����̍X�V
	{
		// �v���C���[���J�����̃^�[�Q�b�g�ɐݒ�
		for (auto playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))tpsCamera_->SetTarget(playerBehavior->GetHolder());
	}
}

void Scene_Play::Draw()
{
}
void Scene_Play::Release()
{
}

// ���ƃ{�X�����񂾍ۂ̃V�[���؂�ւ��ł��ĂȂ�����m�F����
// ���������v���C���[�Ɠ���ւ���Ƃ��Ȃ񂩂���Ȋ�����