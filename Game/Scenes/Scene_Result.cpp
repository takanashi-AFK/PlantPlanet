#include "Scene_Result.h"
#include "../Objects/UI/UIPanel.h"
#include "../../Engine/Global.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIObject.h"
#include "../Objects/UI/UIText.h"
#include "../Objects/UI/UIButton.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/Camera/TPSCamera.h"
#include "../Otheres/GameEditor.h"
#include "../Objects/Stage/SkySphere.h"
#include "../../Engine/GameObject/Camera.h"

Scene_Result::Scene_Result(GameObject* parent_)
{
}

namespace {
	const int CAMERA_HEIGHT = 30;
}

void Scene_Result::Initialize()
{
	// json�t�@�C���Ǎ��p�I�u�W�F�N�g��p��
	json loadData;

	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UI�p�l������ǂݍ���
	if (JsonReader::Load("Datas/UILayouts/ResultScene_layout.json", loadData)) uiPanel->Load(loadData);

	// UI�p�l���̏����擾�E�ݒ�
	{
		// �e�L�X�g�Ƀv���C���[��HP�̒l��ݒ�
		UIText* hpNumText = (UIText*)uiPanel->GetUIObject("HPNum");
		hpNumText->SetText(&ScoreManager::playerHp);

		// �e�L�X�g�Ƀ^�C���̒l��ݒ�
		UIText* timeNumText = (UIText*)uiPanel->GetUIObject("TimeNum");
		timeNumText->SetText(&ScoreManager::time);

		// ��L�̒l�l����X�R�A���v�Z
		scoreNum_ = ScoreManager::playerHp * 100 + ScoreManager::time * 10;

		// �e�L�X�g�ɃX�R�A�̒l��ݒ�
		UIText* scoreNumText = (UIText*)uiPanel->GetUIObject("ScoreNum");
		scoreNumText->SetText(&scoreNum_);

	}

	// �X�e�[�W���쐬 & �ǂݍ���
	Stage* pStage = Instantiate<Stage>(this);
	if(JsonReader::Load("Datas/StageLayouts/EndSceneStage.json", loadData))pStage->Load(loadData);
	
}

void Scene_Result::Update()
{
	// �V�[���J�ڃ{�^���̏���
	{
		// UIButton�̎擾
		UIButton* button = (UIButton*)UIPanel::GetInstance()->GetUIObject("NextSceneButton");

		// �{�^���������ꂽ��
		if (button->OnClick()) {
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
		}
	}

	// �J�����̉�]����
	{
		// �J�����̈ʒu�ƒ����_���擾
		XMFLOAT3 camPos = Camera::GetPosition();
		XMFLOAT3 camTarget = Camera::GetTarget();

		// �J�����̍������Œ�
		camPos.y = CAMERA_HEIGHT;

		// 1f�ɂ���]����p�x
		static float angle = 0.005f;

		// ��]�s����쐬 (Y������ɉ�])
		XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

		// �J�����̈ʒu���x�N�g����
		XMVECTOR vCamPosition = XMLoadFloat3(&camPos);

		// �J�����̈ʒu�����W��]
		vCamPosition = XMVector3Transform(vCamPosition, rotationMatrix);
		XMStoreFloat3(&camPos, vCamPosition);

		// �J�����̈ʒu�ƒ����_��ݒ�
		Camera::SetPosition(camPos);
		Camera::SetTarget(0, 0, 0);
	}
}

void Scene_Result::Draw()
{
}

void Scene_Result::Release()
{
}
