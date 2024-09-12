#include "Scene_Ranking.h"

// �C���N���[�h
#include "../Objects/UI/UIPanel.h"

Scene_Ranking::Scene_Ranking(GameObject* _parent)
	: GameObject(_parent, "Scene_Ranking")
{
}

void Scene_Ranking::Initialize()
{
	// UILayout_json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load("Datas/UILayouts/rankingScene_layout.json", loadData)) {

		// UI�p�l�����擾
		UIPanel* panel = UIPanel::GetInstance();

		// �X�v���b�V���V�[���̃p�l��layout��ݒ�
		panel->Load(loadData);
	}
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
