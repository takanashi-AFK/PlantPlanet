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

using namespace Constants;


Scene_Result::Scene_Result(GameObject* parent_)
	: GameObject(parent_, "Scene_Result"), scoreNum_(0)
{
}

void Scene_Result::Initialize()
{
	ShowCursor(true);

	// jsonファイル読込用オブジェクトを用意
	json loadData;

	// UIパネルを取得
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UIパネル情報を読み込む
	if (JsonReader::Load("Datas/Test/Prottype_ResultScene_Layout.json", loadData)) uiPanel->Load(loadData);

	// flowerをカウントする
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

		// テキストを取得してカウントを表示
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
