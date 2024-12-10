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
#include "../Plants/PlantCollection.h"
#include "../Objects/UI/UIImage.h"
#include "../Objects/UI/UICursor.h"


using namespace Constants;


Scene_Result::Scene_Result(GameObject* parent_)
	: GameObject(parent_, "Scene_Result"), scoreNum_(0), layerNumberCount_(-1)
{
	UICursor::ToHide(false);
}

void Scene_Result::Initialize()
{
	// json�t�@�C���Ǎ��p�I�u�W�F�N�g��p��
	json loadData;

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();
	
	// UI�p�l������ǂݍ���
	if (JsonReader::Load("Datas/Test/resultTent.json", loadData)) uiPanel->Load(loadData);

	std::unordered_map<int, PlantData>allPlantData = PlantCollection::GetPlants();

	// �����A���̐����J�E���g
	std::unordered_map<std::string, int> countedPlant;
	for (const auto& plant : g_playerPlantData) {
		countedPlant[plant.name_]++;
	}


	for (int i = 0; i <= 9; i++) {
		// �J�E���g�����A���̐����擾
		int plantSize = countedPlant.size();

		// UIImage���擾
		UIObject* image = uiPanel->GetUIObject("GetPlant" + std::to_string(i + 1));
		UIObject* text = uiPanel->GetUIObject("GetPlant" + std::to_string(i + 1) + "Text");

		// �A���̐���i���傫���ꍇ(�擾�ł���ꍇ)
		if (i <= plantSize - 1) {
			// countedPlant�̒�����i�Ԗڂ̐A�����擾
			auto it = countedPlant.begin();
			std::advance(it, i); // i�Ԗڂ̗v�f�Ɉړ�

			const std::string& plantName = it->first; // �A���̖��O
			int plantCount = it->second;             // �A���̐�

			for (const auto& p : allPlantData) {
				if (plantName == p.second.name_) {
					PlantData plantData = p.second;
					((UIImage*)image)->SetImage(plantData.imageFilePath_);
					((UIText*)text)->SetText("x" + std::to_string(plantCount));
					break;
				}
			}
		}
		else {
			// �擾�ł��Ȃ��ꍇ�͋�̉摜�ƃe�L�X�g��\��
			((UIImage*)image)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
			((UIText*)text)->SetText("");
		}
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
