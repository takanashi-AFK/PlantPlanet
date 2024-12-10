#include "Scene_Result.h"

// インクルード
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
	// jsonファイル読込用オブジェクトを用意
	json loadData;

	// UIパネルを取得
	UIPanel* uiPanel = UIPanel::GetInstance();
	
	// UIパネル情報を読み込む
	if (JsonReader::Load("Datas/Test/resultTent.json", loadData)) uiPanel->Load(loadData);

	std::unordered_map<int, PlantData>allPlantData = PlantCollection::GetPlants();

	// 同じ植物の数をカウント
	std::unordered_map<std::string, int> countedPlant;
	for (const auto& plant : g_playerPlantData) {
		countedPlant[plant.name_]++;
	}


	for (int i = 0; i <= 9; i++) {
		// カウントした植物の数を取得
		int plantSize = countedPlant.size();

		// UIImageを取得
		UIObject* image = uiPanel->GetUIObject("GetPlant" + std::to_string(i + 1));
		UIObject* text = uiPanel->GetUIObject("GetPlant" + std::to_string(i + 1) + "Text");

		// 植物の数がiより大きい場合(取得できる場合)
		if (i <= plantSize - 1) {
			// countedPlantの中からi番目の植物を取得
			auto it = countedPlant.begin();
			std::advance(it, i); // i番目の要素に移動

			const std::string& plantName = it->first; // 植物の名前
			int plantCount = it->second;             // 植物の数

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
			// 取得できない場合は空の画像とテキストを表示
			((UIImage*)image)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
			((UIText*)text)->SetText("");
		}
	}


}

void Scene_Result::Update()
{
	// シーン遷移ボタンの処理
	{
		// ボタンが押されたら
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
		return 0; // クリアしていない場合のスコア
	}
	int clearBonus = 1000;					// クリアボーナス (1000ポイント)
	int timeBonus = remainingTime * 100;	// 時間のボーナス（1秒あたり100ポイント）
	int hpBonus = remainingHP * 500;		// HPのボーナス（HP1あたり500ポイント）

	return clearBonus + timeBonus + hpBonus;
}
