#pragma once
#include "global.h"
#include "GameObject/GameObject.h"
#include "ResourceManager/Transition.h"

//ゲームに登場するシーン
enum SCENE_ID
{
	SCENE_ID_TEST = 0,
	SCENE_ID_SPLASH,
	SCENE_ID_TITLE,
	SCENE_ID_DIFFICULTYSELECT,
	SCENE_ID_PLAY,
	SCENE_ID_RESULT,
	SCENE_ID_RANKING,
	SCENE_ID_END,
	SCENE_ID_MAX
};


//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	//引数：next	次のシーンのID
	//引数：type	トランジションID
	//引数：time	時間(秒)
	void ChangeScene(SCENE_ID next, TRANSITION_ID _type, float _time);
	void ChangeScene(SCENE_ID next, TRANSITION_ID _type);
	void ChangeScene(SCENE_ID next);

/*
getter :*/
	/// <summary> 現在のシーンIDを取得 </summary>
	SCENE_ID GetCurrentSceneID() const { return currentSceneID_; }

	/// <summary> 次のシーンIDを取得 </summary>
	SCENE_ID GetNextSceneID() const { return nextSceneID_; }

	/// <summary> 一時的なシーンIDを取得 </summary>
	SCENE_ID GetTmpSceneID() const { return tmpID_; }

/*
predicate :*/
	/// <summary> シーンが切り替わっているか </summary>
	bool IsChanging() const { return currentSceneID_ != nextSceneID_; }
	bool IsChanging(SCENE_ID _prev, SCENE_ID _next);

	/// <summary> 現在のシーンが該当シーンか </summary>
	bool IsCurrentScene(SCENE_ID _id) const { return currentSceneID_ == _id; }

private:

	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン
	SCENE_ID tmpID_;
};