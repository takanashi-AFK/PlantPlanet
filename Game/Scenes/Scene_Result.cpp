#include "Scene_Result.h"

// �C���N���[�h
#include "../../Engine/Global.h"
#include "../../Engine/SceneManager.h"
//#include "../../Engine/GameObject/Camera.h"
#include "../Constants.h"
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/UI/UIButton.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIText.h"
#include "../Otheres/RankingManager.h"

using namespace Constants;

namespace {
	const int CAMERA_HEIGHT = 30; // �J�����̍���
}

Scene_Result::Scene_Result(GameObject* parent_)
	: GameObject(parent_, "Scene_Result"), scoreNum_(0)
{
}

void Scene_Result::Initialize()
{
	// �J�[�\���̕\��
	ShowCursor(true);

	// json�t�@�C���Ǎ��p�I�u�W�F�N�g��p��
	json loadData;

	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UI�p�l������ǂݍ���
	if (JsonReader::Load(RESULT_SCENE_LAYOUT_JSON, loadData)) uiPanel->Load(loadData);

	// UI�p�l���̏����擾�E�ݒ�
	{
		// �e�L�X�g�Ƀv���C���[��HP�̒l��ݒ�
		UIText* hpNumText = (UIText*)uiPanel->GetUIObject(RESULT_SCENE_HP_TEXT_NAME);
		hpNumText->SetText(&ScoreManager::playerHp);

		// �e�L�X�g�Ƀ^�C���̒l��ݒ�
		UIText* timeNumText = (UIText*)uiPanel->GetUIObject(RESULT_SCENE_TIME_TEXT_NAME);
		timeNumText->SetText(&ScoreManager::time);

		// �X�R�A�̌v�Z
		scoreNum_ = CalculateScore(ScoreManager::isClear, ScoreManager::time, ScoreManager::playerHp);

		// �e�L�X�g�ɃX�R�A�̒l��ݒ�
		UIText* scoreNumText = (UIText*)uiPanel->GetUIObject(RESULT_SCENE_SCORE_TEXT_NAME);
		scoreNumText->SetText(&scoreNum_);
	}

	// �����L���O�ɃX�R�A��ǉ�
	RankingManager* rm = &RankingManager::GetInstance(); {
		rm->Load(RANKING_DATA_CSV);
		rm->AddScore(scoreNum_);
		rm->Save(RANKING_DATA_CSV);
	}

	// �X�e�[�W���쐬 & �ǂݍ���
	Stage* pStage = Instantiate<Stage>(this);
	if(JsonReader::Load(RESULT_SCENE_STAGE_LAYOUT_JSON, loadData))pStage->Load(loadData);
}

void Scene_Result::Update()
{
	// �V�[���J�ڃ{�^���̏���
	{
		// UIButton�̎擾
		UIButton* button = (UIButton*)UIPanel::GetInstance()->GetUIObject(RESULT_SCENE_NEXT_BUTTON_NAME);

		// �{�^���������ꂽ��
		if (button->OnClick()) {
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
		}
	}

	// �J�����̉�]����
	{
		//// �J�����̈ʒu�ƒ����_���擾
		//XMFLOAT3 camPos = Camera::GetPosition();
		//XMFLOAT3 camTarget = Camera::GetTarget();

		//// �J�����̍������Œ�
		//camPos.y = CAMERA_HEIGHT;

		//// 1f�ɂ���]����p�x
		//static float angle = 0.005f;

		//// ��]�s����쐬 (Y������ɉ�])
		//XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

		//// �J�����̈ʒu���x�N�g����
		//XMVECTOR vCamPosition = XMLoadFloat3(&camPos);

		//// �J�����̈ʒu�����W��]
		//vCamPosition = XMVector3Transform(vCamPosition, rotationMatrix);
		//XMStoreFloat3(&camPos, vCamPosition);

		//// �J�����̈ʒu�ƒ����_��ݒ�
		//Camera::SetPosition(camPos);
		//Camera::SetTarget(0, 0, 0);
	}
}

void Scene_Result::Draw()
{
}

void Scene_Result::Release()
{
}

int Scene_Result::CalculateScore(bool isCleared, int remainingTime, int remainingHP)
{
	if (!isCleared) {
		return 0; // �N���A���Ă��Ȃ��ꍇ�̃X�R�A
	}
	int clearBonus = 1000;					// �N���A�{�[�i�X (1000�|�C���g)
	int timeBonus = remainingTime * 100;	// ���Ԃ̃{�[�i�X�i1�b������100�|�C���g�j
	int hpBonus = remainingHP * 500;		// HP�̃{�[�i�X�iHP1������500�|�C���g�j

	return clearBonus + timeBonus + hpBonus;
}
