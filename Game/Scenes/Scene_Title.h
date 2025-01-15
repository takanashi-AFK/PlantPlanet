#pragma once

#include "../../Engine/GameObject/GameObject.h"

#include <string>
#include <vector>

using std::string;
using std::vector;
class UIPanel;

class Scene_Title:public GameObject
{
private:
	bool	isFirstSelectButton_;	// 最初にボタンを選択しているどうかを保持する変数
	int		status_;				// 現在の状態を保持する変数

public:
	/// <summary> コンストラクタ </summary>
	Scene_Title(GameObject* parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;

private:
	/// <summary> UI入力処理 </summary>
	void HandleUIInput(UIPanel* _uiPanel, bool& _isFirstSelectButton);

	/// <summary> ボタンアクション処理 </summary>
	void ProcessButtonAction(UIPanel* _uiPanel,string _buttonName, string _userName);

/*
uiControl: */
	/// <summary> UIオブジェクトの表示/非表示を設定 </summary>
	void SetUIVisible(UIPanel* _uiPanel, vector<string> _uiObjectNames, bool _visible);

	/// <summary> ポップアップを閉じる </summary>
	void ClosePopup(UIPanel* _uiPanel);

};
