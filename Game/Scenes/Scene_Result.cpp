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
#include "../Otheres/UserManager.h"
#include "../Objects/UI/Components/Component_UIEasing.h"

using namespace Constants;


Scene_Result::Scene_Result(GameObject* parent_)
	: GameObject(parent_, "Scene_Result"), scoreNum_(0), layerNumberCount_(-1)
{
	UICursor::ToHide(false);
}

void Scene_Result::Initialize()
{
	switch (g_gameMode) {
	case MODE_ADVENTURE:
		InitializeAdventureResult();
		break;

	case MODE_SCOREATTACK:
		InitializeScoreAttackResult();
		break;
	}


	// �J�[�\���̕\����Ԃ�؂�ւ���
	UICursor::ToHide(false);


	UserManager& um = UserManager::GetInstance();
}

void Scene_Result::Update()
{
	switch (g_gameMode) {
	case MODE_ADVENTURE:
		UpdateAdventureResult();
		break;

	case MODE_SCOREATTACK:
		UpdateScoreAttackResult();
		break;
	}

	// �{�^���������ꂽ��
	if (((UIButton*)UIPanel::GetInstance()->GetUIObject("returnButton"))->OnClick() || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {
		SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
		sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);
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

void Scene_Result::InitializeScoreAttackResult()
{
	json loadData;

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UI�p�l������ǂݍ���
	if (JsonReader::Load("Datas/Test/SoreAttackResultTent.json", loadData)) uiPanel->Load(loadData);

	totalScore_ = CalculateScore(ScoreManager::isClear, ScoreManager::time, ScoreManager::playerHp);
	remainHP_ = ScoreManager::playerHp;
	dealtDMG_ = ScoreManager::dealtDMG;
	recieveDMG_ = ScoreManager::recieveDMG;
	justAvoidanceAmount_ = ScoreManager::justAvoidance;

	ApplyFinalData();

	//�ŏ��͉B���Ă���
	static_cast<UIText*>(uiPanel->GetUIObject("Value_remainingHP"))->SetSize(0);
	static_cast<UIText*>(uiPanel->GetUIObject("Value_recieveDamageAmount"))->SetSize(0);
	static_cast<UIText*>(uiPanel->GetUIObject("Value_justAvoidanceAmount"))->SetSize(0);
	static_cast<UIText*>(uiPanel->GetUIObject("Value_dealtDamageAmount"))->SetSize(0);
	scoreAttackUpdateFunction = ScoreManager::isClear ?
		&Scene_Result::UpdateTexts :
		&Scene_Result::UpdateTexts;
	//&Scene_Result::UpdateWaitingForReturn;


	for (auto& item : uiPanel->GetUIObjects()) {

		//�C�[�W���O���ӂ��܂��UI���ǂ���
		if (!item->GetEasing())continue;

		//�܂܂�Ă����珉���ʒu�ł���1.0��
		auto easing = item->GetEasing()->GetEasing();
		easing->pile_ = 1.0f;
	}
}

void Scene_Result::InitializeAdventureResult()
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
	for (const auto& plant : g_thisPlayGetPlantData) {
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

void Scene_Result::UpdateScoreAttackResult()
{
	(this->*scoreAttackUpdateFunction)();
}

void Scene_Result::UpdateAdventureResult()
{

}

void Scene_Result::CheckSkipScoreAttackResult()
{
	//���o�X�L�b�v�{�^��������Ȃ������甲����
	if (!(Input::IsPadButtonDown(XINPUT_GAMEPAD_A) || Input::IsKeyDown(DIK_SPACE)))return;

	scoreAttackUpdateFunction = &Scene_Result::UpdateWaitingForReturn;

	//���o�X�L�b�v���̏���
	ApplyFinalData();

}

void Scene_Result::UpdateTexts()
{
	//�X�L�b�v�������ǂ����m�F
	CheckSkipScoreAttackResult();

	std::list<UIObject*> texts = {};

	//Text ����n�܂�ui���擾
	for (auto& ui : UIPanel::GetInstance()->GetUIObjects()) {
		if (ui->GetObjectName().starts_with("Text") && ui->GetEasing())
			texts.push_back(ui);
	}

	//�S�Ă�UI�̏������I��������ǂ���
	bool isAllFinish = true;

	float tempEasingValue = easingValue_;

	//�擾����ui�ɃC�[�W���O����
	for (auto& ui : texts) {
		auto& pile = ui->GetEasing()->GetEasing()->pile_;
		pile = tempEasingValue;
		if (pile > 0)	isAllFinish = false;//0�ɂȂ��Ă��Ȃ��Ȃ�܂��C�[�W���O������

		//������Ƃ����炷
		tempEasingValue += 0.2;
	}

	//���ꂼ��3�b�Ō��̈ʒu�ɖ߂�
	easingValue_ -= 1 / (1.f * FPS);

	if (!isAllFinish)return;

	easingValue_ = 1.0;//������
	scoreAttackUpdateFunction = &Scene_Result::UpdateValues;//���̏����Ɉڂ�
}

void Scene_Result::UpdateValues()
{
	//�X�L�b�v�������ǂ����m�F
	CheckSkipScoreAttackResult();

	std::list<UIText*> texts = {};

	//Value ����n�܂�ui���擾(TotalScore�ȊO)
	for (auto& ui : UIPanel::GetInstance()->GetUIObjects()) {
		if (ui->GetObjectName().starts_with("Value") && ui->GetEasing() && ui->GetObjectName() != "Value_TotalScore")
			texts.push_back(static_cast<UIText*>(ui));
	}

	//�S�Ă�UI�̏������I��������ǂ���
	bool isAllFinish = true;

	float tempEasingValue = easingValue_;
	constexpr float defaultCharSize = 1.5f;

	//�擾����ui�ɃC�[�W���O����
	for (auto& ui : texts) {
		auto& pile = ui->GetEasing()->GetEasing()->pile_;
		pile = tempEasingValue;
		if (pile > 0)	isAllFinish = false;//0�ɂȂ��Ă��Ȃ��Ȃ�܂��C�[�W���O������

		//������Ƃ����炷
		tempEasingValue += 0.01;

		//�C�[�W���O�ɍ��킹�ăT�C�Y���ς���
		ui->SetSize((1 - tempEasingValue) * defaultCharSize);

		//4���̗�����\��
		ui->SetText(to_string(rand() % 1001));
	}

	//���ꂼ��2.5�b�Ō��̈ʒu�ɖ߂�
	easingValue_ -= 1 / (2.5f * FPS);

	if (!isAllFinish)return;

	easingValue_ = 1.0;//������
	scoreAttackUpdateFunction = &Scene_Result::UpdateTotalScore;//���̏����Ɉڂ�

	auto uiPanel = UIPanel::GetInstance();

	//���l�������Ƃ������̂ɂ���
	static_cast<UIText*>(uiPanel->GetUIObject("Value_remainingHP"))->SetText(to_string(remainHP_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_recieveDamageAmount"))->SetText(to_string(recieveDMG_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_justAvoidanceAmount"))->SetText(to_string(justAvoidanceAmount_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_dealtDamageAmount"))->SetText(to_string(dealtDMG_));
}

void Scene_Result::UpdateTotalScore()
{
	//�X�L�b�v�������ǂ����m�F
	CheckSkipScoreAttackResult();

	UIText* totalScore = static_cast<UIText*>(UIPanel::GetInstance()->GetUIObject("Value_TotalScore"));
	totalScore->GetEasing()->GetEasing()->pile_ = easingValue_;

	//�C�[�W���O�ɍ��킹�Đ��l�����������Ă���
	int drawScore = (1 - easingValue_) * totalScore_;
	static_cast<UIText*>(UIPanel::GetInstance()->GetUIObject("Value_TotalScore"))->SetText(to_string(drawScore));

	//3�b�|���ăC�[�W���O
	easingValue_ -= 1 / (3.0f * FPS);

	//0���傫��������܂��C�[�W���O��
	if (easingValue_ >= 0)	return;

	easingValue_ = 1.0f;
	scoreAttackUpdateFunction = &Scene_Result::UpdateButton;

	//���l�𐳏�Ȃ��̂�
	static_cast<UIText*>(UIPanel::GetInstance()->GetUIObject("Value_TotalScore"))->SetText(to_string(totalScore_));
}

void Scene_Result::UpdateButton()
{
	//�X�L�b�v�������ǂ����m�F
	CheckSkipScoreAttackResult();

	//�߂邽�߂̃{�^����T��
	UIButton* returnButton = static_cast<UIButton*>(UIPanel::GetInstance()->GetUIObject("returnButton"));
	returnButton->GetEasing()->GetEasing()->pile_ = easingValue_;

	//1�b�|���ăC�[�W���O
	easingValue_ -= 1 / (1.0 * FPS);

	//0���傫��������܂��C�[�W���O��
	if (easingValue_ >= 0)	return;

	easingValue_ = 1.0f;
	scoreAttackUpdateFunction = &Scene_Result::UpdateWaitingForReturn;
	ApplyFinalData();
}

void Scene_Result::UpdateWaitingForReturn()
{
	auto uiPanel = UIPanel::GetInstance();

	constexpr float defaultCharSize = 1.5f;
	static_cast<UIText*>(uiPanel->GetUIObject("Value_remainingHP"))->SetSize(defaultCharSize);
	static_cast<UIText*>(uiPanel->GetUIObject("Value_recieveDamageAmount"))->SetSize(defaultCharSize);
	static_cast<UIText*>(uiPanel->GetUIObject("Value_justAvoidanceAmount"))->SetSize(defaultCharSize);
	static_cast<UIText*>(uiPanel->GetUIObject("Value_dealtDamageAmount"))->SetSize(defaultCharSize);

	for (auto& item : uiPanel->GetUIObjects()) {

		//�C�[�W���O���ӂ��܂��UI���ǂ���
		if (!item->GetEasing())continue;

		//�܂܂�Ă�����ŏI�ʒu�ł���0��
		auto easing = item->GetEasing()->GetEasing();
		easing->pile_ = .0f;
	}

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

		//�C�[�W���O���ӂ��܂��UI���ǂ���
		if (!item->GetEasing())continue;

		//�܂܂�Ă�����ŏI�I�Ȉʒu��
		auto easing = item->GetEasing()->GetEasing();
		easing->pile_ = .0f;
	}

	static_cast<UIText*>(uiPanel->GetUIObject("Value_remainingHP"))->SetText(to_string(remainHP_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_recieveDamageAmount"))->SetText(to_string(recieveDMG_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_justAvoidanceAmount"))->SetText(to_string(justAvoidanceAmount_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_dealtDamageAmount"))->SetText(to_string(dealtDMG_));
	static_cast<UIText*>(uiPanel->GetUIObject("Value_TotalScore"))->SetText(to_string(totalScore_));
}