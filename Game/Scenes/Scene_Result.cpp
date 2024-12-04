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
#include "../../Engine/DirectX/Input.h"

using namespace Constants;


Scene_Result::Scene_Result(GameObject* parent_)
	: GameObject(parent_, "Scene_Result"), scoreNum_(0)
{
}

void Scene_Result::Initialize()
{
	ShowCursor(true);

	// json�t�@�C���Ǎ��p�I�u�W�F�N�g��p��
	json loadData;

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UI�p�l������ǂݍ���
	if (JsonReader::Load("Datas/Test/Prottype_ResultScene_Layout.json", loadData)) uiPanel->Load(loadData);

	// flower���J�E���g����
	{
		XMFLOAT3 flowerCount = { 0,0,0 };

		for (auto plant : g_playerPlantData) {

			switch (plant.rarity_)
			{
			case 1:flowerCount.x++; break;
			case 2:flowerCount.y++; break;
			case 3:flowerCount.z++; break;
			}
		}

		// �e�L�X�g���擾���ăJ�E���g��\��
		UIText* text1 = (UIText*)uiPanel->GetUIObject("flower1Count");
		text1->SetText("x" + std::to_string((int)flowerCount.x));

		UIText* text2 = (UIText*)uiPanel->GetUIObject("flower2Count");
		text2->SetText("x" + std::to_string((int)flowerCount.y));

		UIText* text3 = (UIText*)uiPanel->GetUIObject("flower3Count");
		text3->SetText("x" + std::to_string((int)flowerCount.z));
	}
}

void Scene_Result::Update()
{
	// �V�[���J�ڃ{�^���̏���
	{
		// �{�^���������ꂽ��
		if (((UIButton*)UIPanel::GetInstance()->GetUIObject("returnButton"))->OnClick() || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
		}
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
