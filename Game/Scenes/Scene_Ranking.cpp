#include "Scene_Ranking.h"

// �C���N���[�h
#include "../Objects/UI/UIPanel.h"
#include "../Otheres/RankingManager.h"

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
	if (JsonReader::Load("Datas/UILayouts/rankingScene_layout.json", loadData)) panel->Load(loadData);
	
	// �����L���O�f�[�^��ǂݍ���
	RankingManager::GetInstance().Load("ranking.csv");
}

void Scene_Ranking::Update()
{
}

void Scene_Ranking::Draw()
{
}

void Scene_Ranking::Release()
{
}
