#include "Scene_Test.h"

// �C���N���[�h
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/Stage/SkySphere.h"
#include "../../TestObject_Box.h"

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test"),pStage_(nullptr)
{
}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A��W���Ő���
	//Instantiate<SkySphere>(this);

	// �e�X�g�I�u�W�F�N�g�𐶐�
	Instantiate<TestObject_Box>(this);


}

void Scene_Test::Update()
{
}

void Scene_Test::Draw()
{
}

void Scene_Test::Release()
{
}
