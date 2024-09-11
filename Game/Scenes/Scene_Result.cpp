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
	json loadData;

	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	if (JsonReader::Load("Datas/UILayouts/ResultScene_layout.json", loadData)) {

		// UI�p�l�����擾
		panel = UIPanel::GetInstance();
		panel->Load(loadData);
		UIText* scoreNum = (UIText*)panel->GetUIObject("ScoreNum");

		scoreNum->SetText(&ScoreManager::g_Score);

		UIText* timeNum = (UIText*)panel->GetUIObject("TimeNum");

		// timeNum->SetText(&ScoreManager::g_Time);

		UIText* hpNum = (UIText*)panel->GetUIObject("HPNum");

		// hpNum->SetText(&ScoreManager::g_HP);

	}

	// �X�e�[�W���쐬
	Stage* pStage = Instantiate<Stage>(this); {
		json stageData;
		JsonReader::Load("Datas/StageLayouts/EndSceneStage.json", stageData);
		pStage->Load(stageData);
	}
}

void Scene_Result::Update()
{
	UIButton* button = (UIButton*)panel->GetUIObject("NextSceneButton");

	// �{�^���������ꂽ��
	if (button->OnClick()) {
		SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
		sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
	}

	
	// �J�����̈ʒu�ƒ����_���擾
	XMFLOAT3 camPos = Camera::GetPosition();
	XMFLOAT3 camTarget = Camera::GetTarget();
	// �J�����̍������Œ�
	camPos.y = CAMERA_HEIGHT;

	// 1f�ɂ���]����p�x
	static float angle = 0.01f;

	// ��]�s����쐬 (Y������ɉ�])
	XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

	// �J�����̈ʒu���x�N�g����
	XMVECTOR vCamPosition = XMLoadFloat3(&camPos);

	// �J�����̈ʒu�����W��]
	vCamPosition = XMVector3Transform(vCamPosition, rotationMatrix);
	XMStoreFloat3(&camPos, vCamPosition);

	// �J�����̈ʒu�ƒ����_��ݒ�
	Camera::SetPosition(camPos);
	Camera::SetTarget(0,0,0);
}

void Scene_Result::Draw()
{
}

void Scene_Result::Release()
{
}
