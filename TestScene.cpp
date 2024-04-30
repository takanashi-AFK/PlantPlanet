#include "TestScene.h"
#include "StageObject.h"
#include "Engine/ImGui/imgui.h"

#include "RotationYComponent.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

static StageObject* obj = nullptr;
//������
void TestScene::Initialize()
{

	obj = CreateStageObject("obj", "DebugCollision/BoxCollider.fbx", this);
	obj->AddComponent(new RotationYComponent(obj));
}

//�X�V
void TestScene::Update()
{
	if (ImGui::Button("load")) {
		json loadObj;
		JsonReader::Load("testdata.json", loadObj);
		obj->Load(loadObj);
	}
	
	if (ImGui::Button("save")) {
		json savaObj;
		obj->Save(savaObj);
		JsonReader::Save("testdata.json", savaObj);
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
