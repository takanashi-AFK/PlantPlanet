#include "Scene_Title.h"

// インクルード
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"

#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIInputString.h"
using namespace Constants;

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title")
{
}

void Scene_Title::Initialize()
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if (JsonReader::Load("Datas/Test/Prottype_titleScene_Layout.json", loadData)) UIPanel::GetInstance()->Load(loadData);
}

void Scene_Title::Update()
{
	// シーン切替処理
	{
		if (Input::IsKeyDown(DIK_SPACE))
		{
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
		}
	}
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
