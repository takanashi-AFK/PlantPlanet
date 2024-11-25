#include "Scene_Play.h"

// �C���N���[�h
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/Global.h"
#include"../../Engine/SceneManager.h"
#include "../Constants.h"
#include "../Objects/Camera/TPSCamera.h"
#include "../Objects/Stage/Components/BehaviorComponents/Component_BossBehavior.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include"../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include"../Objects/Stage/SkySphere.h"
#include"../Objects/Stage/Stage.h"
#include "../Objects/UI/CountDown.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UITimer.h"
#include "../Plants/PlantCollection.h"

using namespace Constants;

Scene_Play::Scene_Play(GameObject* parent)
	:GameObject(parent,"Scene_Play"), pStage_(nullptr), tpsCamera_(nullptr), fixedCursorPos(true), cursorVisible(true), isGameStart_(false)
{
}

void Scene_Play::Initialize()
{
	// UI�p�l���̏�����
	InitUIPanel();

	// �X�e�[�W�̏�����
	InitStage();

	// �J�����̏�����
	InitCamera();

	// �J�E���g�_�E���̐���
	countDown_ = Instantiate<CountDown>(this);
	countDown_->Start();

	// �J�[�\���̔�\����
	ShowCursor(false);
}

void Scene_Play::Update()
{
	// �^�C�}�[�̎擾
	UITimer* uiTimer = (UITimer*)UIPanel::GetInstance()->FindObject(PLAY_SCENE_TIMER_NAME);

	// �J�[�\���Œ艻����
	SetCursorMode();

	// �Q�[���J�n����
	if (countDown_->IsFinished() && isGameStart_ == false) {
		
		// �J�����̃A�N�e�B�u��
		tpsCamera_->SetActive(true);

		// �Q�[���J�n�t���O�𗧂Ă�
		isGameStart_ = true;

		// �^�C�}�[�̊J�n
		if (uiTimer != nullptr) uiTimer->StartTimer();
	}

	// �V�[���ؑ֏���
	{
		// �V�[���ؑփt���O��p��
		bool isSceneChange = false;

		// �v���C���[�����񂾏ꍇ�A�ؑփt���O�𗧂Ă�
		for (auto playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))if (((Component_PlayerBehavior*)playerBehavior)->IsDead()) {ScoreManager::isClear = false; isSceneChange = true;}
		
		// �{�X�����񂾏ꍇ�A�ؑփt���O�𗧂Ă�
		//for (auto bossBehavior : pStage_->FindComponents(ComponentType::BossBehavior)) if (((Component_BossBehavior*)bossBehavior)->IsDead()){ScoreManager::isClear = true;isSceneChange = true;}
		
		// �^�C�}�[���I�������ꍇ�A�ؑփt���O�𗧂Ă�
		if(uiTimer!=nullptr)if (uiTimer->IsEnd()) { ScoreManager::isClear = false; isSceneChange = true; }

		// �V�[���ؑփt���O�������Ă���ꍇ
		if (isSceneChange == true) {
			
			// �^�C�}�[�̍ŏI�l���擾
			ScoreManager::time = uiTimer->GetSeconds();
			
			// �V�[����؂�ւ���
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT);
		}
	}

	// �v���C���[���J�����̃^�[�Q�b�g�ɐݒ�
	for (auto playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))tpsCamera_->SetTarget(playerBehavior->GetHolder());
}

void Scene_Play::Draw()
{
}

void Scene_Play::Release()
{
}

void Scene_Play::InitUIPanel()
{
	// UI�p�l�� & ���C�A�E�g�̓ǂݍ���
	json loadData;
	if (JsonReader::Load(PLAY_SCENE_UI_LAYOUT_JSON, loadData)) UIPanel::GetInstance()->Load(loadData);
}

void Scene_Play::InitStage()
{
	// �X�J�C�X�t�B�A�̐���
	Instantiate<SkySphere>(this);

	// �X�e�[�W�f�[�^�̓ǂݍ���
	json loadData;
	if (JsonReader::Load("Datas/debug/testStage_Intract.json", loadData)) {

		// �X�e�[�W�𐶐�
		pStage_ = Instantiate<Stage>(this);
		
		// �X�e�[�W�̓ǂݍ���
		pStage_->Load(loadData);
	}

	// �A���̐���
	{
		// �A���f�[�^�̓ǂݍ���
		json loadData;
		JsonReader::Load("Datas/PlantData/plant.json", loadData);
		PlantCollection::Load(loadData);

		// generator�̋N��
		for (auto pg : (pStage_->GetStageObject("generator"))->FindComponent(ComponentType::PlantGenerator)) 
			pg->Execute();
	}
}

void Scene_Play::InitCamera()
{
	// �J�������̓ǂݍ���
	json loadData;
	if (JsonReader::Load(PLAY_SCENE_CAMERA_LAYOUT_JSON, loadData)) {

		// �J�����̐���
		tpsCamera_ = Instantiate<TPSCamera>(this);
		
		// �J�����̓ǂݍ���
		tpsCamera_->Load(loadData);

		// �J�����̃A�N�e�B�u��
		tpsCamera_->SetActive(false);
	}
}

void Scene_Play::SetCursorMode()
{
	// F3�L�[�������ꂽ��
	if (Input::IsKeyDown(DIK_F3)) {

		// �J�[�\���Œ艻�̐؂�ւ�
		fixedCursorPos = !fixedCursorPos;

		// �J�[�\���̕\����Ԃ�؂�ւ���
		cursorVisible = !fixedCursorPos;
		ShowCursor(cursorVisible);
	}

	// �J�[�\���̈ʒu�𒆉��ɌŒ�
	if (fixedCursorPos) {
		SetCursorPos(Direct3D::screenWidth_ / 2, Direct3D::screenHeight_ / 2);
	}
}
