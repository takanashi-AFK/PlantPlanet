#include "TestScene.h"
#include "../Objects/Stage/Stage.h"
#include "../../Engine/ImGui/imgui.h"


//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	pStage_ = Instantiate<Stage>(this);
}

//�X�V
void TestScene::Update()
{
	if (ImGui::Button("Save")) {
		json saveObj;
		pStage_->Save(saveObj);
		JsonReader::Save("testData.json", saveObj);
	}

	ImGui::SameLine();

	if (ImGui::Button("Load")) {
		json loadObj;
		JsonReader::Load("testData.json", loadObj);
		pStage_->Load(loadObj);
	}
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
}
