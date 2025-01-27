#include "Scene_End.h"

// インクルード
#include "../../Engine/SceneManager.h"
#include "../Constants.h"
#include "../Objects/UI/UIPanel.h"

using namespace Constants;

// 定数
namespace {
	const float LIMIT_TIME = 3.f;				// シーンを切り替えるまでに掛かる時間（秒）
}


Scene_End::Scene_End(GameObject* parent_)
	: GameObject(parent_, "Scene_End"), count_()
{
}

void Scene_End::Initialize()
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if(JsonReader::Load(END_SCENE_UI_LAYOUT_JSON,loadData))UIPanel::GetInstance()->Load(loadData);
}

void Scene_End::Update()
{
	// シーン切替処理
	{
		// 一定時間経過したらシーンを切り替える
		if (count_ >= LIMIT_TIME * FPS) {

			//// シーンを切り替える
			//SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			//sceneManager->ChangeScene(SCENE_ID_TITLE, TID_BLACKOUT);
			PostQuitMessage(0);
		}

		// 経過時間を加算
		count_++;
	}
}

void Scene_End::Draw()
{
}

void Scene_End::Release()
{
}
