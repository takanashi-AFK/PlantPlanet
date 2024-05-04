#include "TestScene.h"

// �C���N���[�h
#include "../Objects/Stage/Stage.h"
#include "../../Engine/ImGui/imgui.h"

TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene"),pStage_(nullptr)
{
}

void TestScene::Initialize()
{
	// �X�e�[�W���쐬
	pStage_ = Instantiate<Stage>(this);
	
	// �X�e�[�W����Ǎ�
	json loadObj;
	JsonReader::Load("testData.json", loadObj);
	pStage_->Load(loadObj);
}

void TestScene::Update()
{
#ifdef _DEBUG

	// �ۑ�
	if (ImGui::Button("Save")) {
		json saveObj;
		pStage_->Save(saveObj);
		JsonReader::Save("testData.json", saveObj);
	}
	ImGui::SameLine();

	// �Ǎ�
	if (ImGui::Button("Load")) {
		json loadObj;
		JsonReader::Load("testData.json", loadObj);
		pStage_->Load(loadObj);
	}
	ImGui::SameLine();

	// �폜
	if (ImGui::Button("Delete")) {
		pStage_->DeleteAllStageObject();
	}
	ImGui::Separator();
#endif // _DEBUG
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
