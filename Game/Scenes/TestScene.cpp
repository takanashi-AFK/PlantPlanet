#include "TestScene.h"
#include "../Objects/Stage/StageObject.h"
#include "../../Engine/ImGui/imgui.h"

#include "../Objects/Stage/Components/RotationYComponent.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

static StageObject* obj = nullptr;

//������
void TestScene::Initialize()
{
}

//�X�V
void TestScene::Update()
{
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
}
