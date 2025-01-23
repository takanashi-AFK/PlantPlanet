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
#include "../Otheres/UserManager.h"
#include "../Objects/UI/Components/Component_UIEasing.h"

using namespace Constants;


Scene_Result::Scene_Result(GameObject* parent_)
	: GameObject(parent_, "Scene_Result"), scoreNum_(0), layerNumberCount_(-1),
	scoreAttackUpdateFunction{&Scene_Result::UpdateTexts},easingValue_ {1.0f}
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
	for (const auto& plant : g_thisPlayGetPlantData) {
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

	UIText* timeUI = static_cast<UIText*>(uiPanel->GetUIObject("time"));
	UserManager& um = UserManager::GetInstance();
	auto user = um.GetLoggedInUser();

	string timeStr = std::format("PLAY TIME:{:0>2}:{:0>2}", g_playTime / 60, g_playTime % 60);
	timeUI->SetText(timeStr);

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

void Scene_Result::CheckSkipScoreAttackResult()
{
	//演出スキップボタン押されなかったら抜ける
	if (!(Input::IsPadButtonDown(XINPUT_GAMEPAD_A) || Input::IsKeyDown(DIK_SPACE)))return;
	
	scoreAttackUpdateFunction = &Scene_Result::UpdateWaitingForReturn;
	
	//演出スキップ時の処理
	ApplyFinalData();

}

void Scene_Result::InitForScoreAttack()
{
	json loadData;

	// UIパネルを取得
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UIパネル情報を読み込む
	if (JsonReader::Load("Datas/Test/SoreAttackResultTent.json", loadData)) uiPanel->Load(loadData);

	for (auto& item : uiPanel->GetUIObjects()) {

		//イージングがふくまれるUIかどうか
		if (!item->GetEasing())continue;

		//含まれていたら初期位置である1.0へ
		auto easing = item->GetEasing()->GetEasing();
		easing->pile_ = 1.0f;
	}

	totalScore_ = CalculateScore(ScoreManager::isClear,ScoreManager::time,ScoreManager::playerHp);
	remainHP_ = ScoreManager::playerHp;
	dealtDMG_ = ScoreManager::dealtDMG;
	recieveDMG_ = ScoreManager::recieveDMG;
	justAvoidanceAmount_ = ScoreManager::justAvoidance;

	ApplyFinalData();
	
	scoreAttackUpdateFunction = ScoreManager::isClear ?
		&Scene_Result::UpdateTexts :
		&Scene_Result::UpdateWaitingForReturn;
}

void Scene_Result::UpdateForScoreAttack()
{
	(this->*scoreAttackUpdateFunction)();
}

void Scene_Result::UpdateTexts()
{
	//スキップしたかどうか確認
	CheckSkipScoreAttackResult();

	std::list<UIObject*> texts = {};

	//Text から始まるuiを取得
	for (auto& ui : UIPanel::GetInstance()->GetUIObjects()) {
		if (ui->GetObjectName().starts_with("Text") && ui->GetEasing())
			texts.push_back(ui);
	}

	//全てのUIの処理が終わったかどうか
	bool isAllFinish = true;

	float tempEasingValue = easingValue_;

	//取得したuiにイージング処理
	for (auto& ui : texts) {
		auto& pile = ui->GetEasing()->GetEasing()->pile_;
		pile = tempEasingValue;
		if (pile > 0)	isAllFinish = false;//0になっていないならまだイージング処理中

		//ちょっとずつずらす
		tempEasingValue += 0.2;
	}

	//それぞれ1秒で元の位置に戻る
	easingValue_ -= 1.f / FPS;

	if (!isAllFinish)return;

	easingValue_ = 1.0;//初期化
	scoreAttackUpdateFunction = &Scene_Result::UpdateValues;//次の処理に移る
}

void Scene_Result::UpdateValues()
{
	//スキップしたかどうか確認
	CheckSkipScoreAttackResult();

	std::list<UIText*> texts = {};

	//Value から始まるuiを取得(TotalScore以外)
	for (auto& ui : UIPanel::GetInstance()->GetUIObjects()) {
		if (ui->GetObjectName().starts_with("Value") && ui->GetEasing() && ui->GetObjectName() != "Value_TotalScore")
			texts.push_back(static_cast<UIText*>(ui));
	}

	//全てのUIの処理が終わったかどうか
	bool isAllFinish = true;

	float tempEasingValue = easingValue_;
	constexpr float defaultCharSize = 1.5f;

	//取得したuiにイージング処理
	for (auto& ui : texts) {
		auto& pile = ui->GetEasing()->GetEasing()->pile_;
		pile = tempEasingValue;
		if (pile > 0)	isAllFinish = false;//0になっていないならまだイージング処理中

		//ちょっとずつずらす
		tempEasingValue += 0.01;

		//イージングに合わせてサイズも変える
		ui->SetSize(tempEasingValue * defaultCharSize);

		//4桁の乱数を表示
		ui->SetText(to_string(rand() % 1001));
	}

	//それぞれ0.5秒で元の位置に戻る
	easingValue_ -= .5f / FPS;

	if (!isAllFinish)return;

	easingValue_ = 1.0;//初期化
	scoreAttackUpdateFunction = &Scene_Result::UpdateTotalScore;//次の処理に移る

	auto uiPanel = UIPanel::GetInstance();

	//数値をちゃんとしたものにする
	static_cast<UIText*>(uiPanel->GetUIObject("Value_remainingHP"))->SetText(to_string(remainHP_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_recieveDamageAmount"))->SetText(to_string(recieveDMG_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_justAvoidanceAmount"))->SetText(to_string(justAvoidanceAmount_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_dealtDamageAmount"))->SetText(to_string(dealtDMG_));
}

void Scene_Result::UpdateTotalScore()
{
	//スキップしたかどうか確認
	CheckSkipScoreAttackResult();

	UIText* totalScore = static_cast<UIText*>(UIPanel::GetInstance()->GetUIObject("Value_TotalScore"));
	totalScore->GetEasing()->GetEasing()->pile_ = easingValue_;

	//イージングに合わせて数値も増加させていく
	auto drawScore = (1 - easingValue_) * totalScore_;
	static_cast<UIText*>(UIPanel::GetInstance()->GetUIObject("Value_TotalScore"))->SetText(to_string(drawScore));

	//1.5秒掛けてイージング
	easingValue_ -= 1.5f / FPS;

	//0より大きかったらまだイージング中
	if (easingValue_ >= 0)	return;

	easingValue_ = 1.0f;
	scoreAttackUpdateFunction = &Scene_Result::UpdateButton;

	//数値を正常なものに
	static_cast<UIText*>(UIPanel::GetInstance()->GetUIObject("Value_TotalScore"))->SetText(to_string(totalScore_));
}

void Scene_Result::UpdateButton()
{
	//スキップしたかどうか確認
	CheckSkipScoreAttackResult();

	//戻るためのボタンを探す
	UIButton* returnButton = static_cast<UIButton*>(UIPanel::GetInstance()->GetUIObject("returnButton"));
	returnButton->GetEasing()->GetEasing()->pile_ = easingValue_;

	//0.3秒掛けてイージング
	easingValue_ -= 0.3f / FPS;

	//0より大きかったらまだイージング中
	if (easingValue_ >= 0)	return;

	easingValue_ = 1.0f;
	scoreAttackUpdateFunction = &Scene_Result::UpdateWaitingForReturn;
	ApplyFinalData();
}

void Scene_Result::UpdateWaitingForReturn()
{
	UIButton* returnButton = static_cast<UIButton*>(UIPanel::GetInstance()->GetUIObject("returnButton"));
	if (returnButton->OnClick() || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
		sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);

		scoreAttackUpdateFunction = &Scene_Result::UpdateEmptyWork;
	}
}

void Scene_Result::UpdateEmptyWork()
{
}

void Scene_Result::ApplyFinalData()
{
	auto uiPanel = UIPanel::GetInstance();
	for (auto& item : uiPanel->GetUIObjects()) {

		//イージングがふくまれるUIかどうか
		if (!item->GetEasing())continue;

		//含まれていたら最終的な位置へ
		auto easing = item->GetEasing()->GetEasing();
		easing->pile_ = .0f;
	}

	static_cast<UIText*>(uiPanel->GetUIObject("Value_remainingHP"))->SetText(to_string(remainHP_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_recieveDamageAmount"))->SetText(to_string(recieveDMG_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_justAvoidanceAmount"))->SetText(to_string(justAvoidanceAmount_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_dealtDamageAmount"))->SetText(to_string(dealtDMG_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_TotalScore"))->SetText(to_string(totalScore_));
}
