#pragma once
#include <vector>
#include<memory>
#include "../../../Engine/Json/JsonReader.h"
#include "../../../../../Engine/GameObject/Transform.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../../../../Engine/Collider/SphereCollider.h"

using std::vector;

class Component_UIEasing;

enum UIType {
	UI_NONE,
	UI_BUTTON,
	UI_IMAGE,
	UI_TEXT,
	UI_PROGRESSBAR,
	UI_PANEL,
	UI_TIMER,
	UI_INPUTSTRING,
	UI_PROGRESSCIRCLE,
	UI_ITEMTABLE,
	UI_MAX,
};

class UIObject
{
private:
	bool isPositionLocked_;
	bool isRotateLocked_;
	bool isScaleLocked_;

protected:
	std::unique_ptr<Component_UIEasing> easing_;

protected:
	string objectName_;		// オブジェクトの名前
	UIType type_;			// オブジェクトの種類
	int layerNumber_;		// レイヤー番号
	Transform transform_;	// 位置、回転、拡大縮小
	UIObject* pParent_;		// 親オブジェクト
	bool isVisible_;		// 描画するか

	std::vector<UIObject*> childList_;  // 子オブジェクトのリスト

	//フラグ
	struct UI_STATE{
		unsigned initialized : 1;	//初期化済みか
		unsigned entered : 1;		//更新するか
		unsigned visible : 1;		//描画するか
		unsigned dead : 1;			//削除するか
	} state_;

public:
	/// <summary> コンストラクタ </summary>
	UIObject();
	UIObject(UIObject* parent);
	UIObject(string _name, UIType _type, UIObject* parent, int _layerNum);

	/// <summary> デストラクタ </summary>
	virtual ~UIObject();

	/// <summary> 初期化 </summary>
	virtual void Initialize() {};

	/// <summary> 更新 </summary>
	virtual void Update() {};

	/// <summary> 描画 </summary>
	virtual void Draw() {};

	/// <summary> 解放 </summary>
	virtual void Release() {};

	/// <summary> 保存 </summary>
	virtual void Save(json& saveObj) {};

	/// <summary> 読込 </summary>
	virtual void Load(json& loadObj) {};

	/// <summary> ImGui表示 </summary>
	virtual void DrawData() {};
	
	/// <summary> 子オブジェクトの更新 </summary>
	void UpdateSub();

	/// <summary> 子オブジェクトの描画 </summary>
	void DrawSub();

	/// <summary> 子オブジェクトの解放 </summary>
	void ReleaseSub();
	
	/// <summary> 子オブジェクトの保存 </summary>
	void ChildSave(json& _saveObj);

	/// <summary> 子オブジェクトの読込 </summary>
	void ChildLoad(json& _loadObj);

	/// <summary> 子オブジェクトのImGui表示 </summary>
	void ChildDrawData();

	/// <summary> オブジェクトの追加 </summary>
	void PushBackChild(UIObject* obj);

	/// <summary> レイヤー番号を
	static bool CompareLayerNumber(UIObject* _object1, UIObject* _object2);

	void CreateEasing();
/*
setter :*/
	/// <summary> 位置の固定 </summary>
	void LockPosition() { isPositionLocked_ = true; }

	/// <summary> 位置の固定解除 </summary>
	void UnlockPosition() { isPositionLocked_ = false; }
	
	/// <summary> 回転の固定 </summary>
	void LockRotate() { isRotateLocked_ = true; }

	/// <summary> 回転の固定解除 </summary>
	void UnlockRotate() { isRotateLocked_ = false; }

	/// <summary> 拡大縮小の固定 </summary>
	void LockScale() { isScaleLocked_ = true; }

	/// <summary> 拡大縮小の固定解除 </summary>
	void UnlockScale() { isScaleLocked_ = false; }

	/// <summary> 死亡フラグを立てる </summary>
	void KillMe();

	/// <summary> レイヤー番号を設定 </summary>
	void SetLayerNumber(int newLayerNumber_);

	/// <summary> 可視化を設定 </summary>
	void SetVisible(bool _visible) { isVisible_ = _visible; }

	inline void SetTrasform(Transform t) { this->transform_ = t; }
/*
getter :*/
	/// <summary> 子オブジェクトを取得 </summary>
	UIObject* FindChildObject(const std::string& name);
	
	/// <summary> オブジェクトを取得 </summary>
	UIObject* FindObject(const std::string& name){ return GetRootJob()->FindChildObject(name); }

	/// <summary> オブジェクトの名前を取得 </summary>
	const string& GetObjectName(void) const;

	/// <summary> オブジェクトの種類を取得 </summary>
	UIType GetType(void) const;

	/// <summary> ルートオブジェクトを取得 </summary>
	UIObject* GetRootJob();
	
	/// <summary> 親オブジェクトを取得 </summary>
	UIObject* GetParent();
	
	/// <summary> レイヤー番号を取得 </summary>
	int GetLayerNumber() const { return layerNumber_; }

	int GetMaxLayerNumber();

	Component_UIEasing* GetEasing();

	inline Transform GetTransform() { return this->transform_; }

	Transform GetCalcTransform(Transform _transform);
	Transform GetCalcTransform();

/*
predicate :*/
	/// <summary> 削除フラグが立っているかどうか </summary>
	bool IsDead();

	/// <summary> レイヤー番号が重複しているかどうか </summary>
	bool IsLayerNumberDuplicate(int newLayerNumber_);

	static UIObject* CreateUIObject(string _name, UIType _type, UIObject* _parent, int _layerNum);
	static string GetUITypeString(UIType _type);

};

