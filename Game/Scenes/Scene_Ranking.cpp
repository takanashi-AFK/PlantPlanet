#include "Scene_Ranking.h"

// �C���N���[�h
#include "../Objects/UI/UIPanel.h"
#include "../Otheres/RankingManager.h"
#include "../Objects/UI/UIButton.h"
#include "../../Engine/SceneManager.h"
#include "../Objects/UI/UIText.h"
#include "../Constants.h"

using namespace Constants;

Scene_Ranking::Scene_Ranking(GameObject* _parent)
	: GameObject(_parent, "Scene_Ranking")
{
}

void Scene_Ranking::Initialize()
{
	// UI�p�l�����擾
	UIPanel* panel = UIPanel::GetInstance();
	
	// UILayout_json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load(RANKING_SCENE_LAYOUT_JSON, loadData)) panel->Load(loadData);
	
	// �����L���O�f�[�^��ǂݍ���
	RankingManager::GetInstance().Load(RANKING_DATA_CSV);

	// �����L���O�f�[�^��\��
	{
		// �����L���O�f�[�^���擾 ��5�ʂ܂�
		for (int i = 1; i <= 5; i++) {

			// ���[�U�[�l�[�����擾
			{
				// UI�e�L�X�g�����擾
				std::string name = "rank" + std::to_string(i) + "_userName";
				UIText* text = (UIText*)panel->GetUIObject(name);

				// ���[�U�[�l�[�����擾
				string userName = RankingManager::GetInstance().GetUser(i - 1);

				// �e�L�X�g�Ƀ��[�U�[�l�[����ݒ�
				if (text != nullptr) text->SetText(userName);
			}
			
			// �X�R�A���擾
			{
				// UI�e�L�X�g�����擾
				std::string name = "rank" + std::to_string(i) + "_scoreNum";
				UIText* text = (UIText*)panel->GetUIObject(name);

				// �X�R�A���擾
				string score = std::to_string(RankingManager::GetInstance().GetScore(i - 1));

				// �e�L�X�g�ɃX�R�A��ݒ�
				if (text != nullptr) text->SetText(score);
			}
		}
	}

}

void Scene_Ranking::Update()
{
	UIButton* returnButton = (UIButton*)UIPanel::GetInstance()->GetUIObject(RANKING_SCENE_RETURN_BUTTON_NAME);
	// �{�^���������ꂽ��V�[����؂�ւ���
	if (returnButton != nullptr) if (returnButton->OnClick() == true) {

		// �V�[����؂�ւ���
		SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
		sceneManager->ChangeScene(SCENE_ID_TITLE, TID_BLACKOUT);
	}
}

void Scene_Ranking::Draw()
{
}

void Scene_Ranking::Release()
{
}
