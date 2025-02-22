#pragma once
#include "../../../Engine/Json/JsonReader.h"
#include <vector>
#include "UIObject.h"


using std::vector;
class UIButton;


class UIPanel : public UIObject
{
private:
	static UIPanel* instance_;	//シングルトンインスタンス

public:
	/// <summary> インスタンスの取得 </summary>
	static UIPanel* GetInstance();
	
	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;

	/// <summary> 保存 </summary>
	void Save(json& _saveObj)override;

	/// <summary> 読込 </summary>
	void Load(json& _loadObj)override;
	
	/// <summary> ImGui表示 </summary>
	void DrawData()override;

	/// <summary> オブジェクトの削除 </summary>
	void DeleteUIObject(UIObject* _object);

	/// <summary> 全オブジェクトの削除 </summary>
	void DeleteAllUIObject();

	/// <summary> オブジェクトのソート </summary>
	void SortUIObject();


/*
getter :*/
	/// <summary> オブジェクトの取得 </summary>
	UIObject* GetUIObject(string _name);

	/// <summary> オブジェクトの取得 </summary>
	vector<UIObject*> GetUIObject(UIType _type);

	/// <summary> オブジェクトリストの取得 </summary>
	vector<UIObject*> GetUIObjects() { return childList_; };


/*
setter :*/
	/// <summary> オブジェクトの可視化を設定 </summary>
	void SetVisible(string _name, bool _visible);
private:
	/// <summary> コンストラクタ </summary>
	UIPanel();

	/// <summary> コピーコンストラクタの除外 </summary>
	UIPanel(const UIPanel&) = delete;

	/// <summary> 代入演算子の除外 </summary>
	UIPanel& operator=(const UIPanel&) = delete;

//---------------------------------マウス以外で座標を取得する

private:

	std::list<UIButton*>  arrayButton_;
	int buttonIndexX_;
	int buttonIndexY_;
	UIButton* selectingButton_;
	
public:

	enum class SELECTOR_MOVE_TO
	{
		UP=0,
		LEFT,
		RIGHT,
		BOTTOM,

		AMOUNT
	};

	bool SetButtonArrayIndex(UIButton* b);
	bool SetButtonArrayIndex(int x, int y);
	void SetSelectingButton(UIButton* btn);
	void GetButtonIndex(int* x, int* y);
	UIButton* GetSelectingButton();

	[[deprecated("正常動作しないため非推奨")]]
	void SetCursorToSelectingButton();
	void SelectorMove(SELECTOR_MOVE_TO way);

	void PushButtonToArray(UIButton* b);
	void RemoveButtonFromArray(UIButton* b);
	void ResetArrayOfButton();

	void CheckSelectingButton();

	std::list<UIButton*> GetArrayList() { return arrayButton_; };


	/// <summary>
	/// 選択しているボタンを選択解除する
	/// </summary>
	void ResetSelectedButton() { selectingButton_ = nullptr; }
private:

	std::vector<UIButton*> GetSelectorMovable(SELECTOR_MOVE_TO way);
	
};

