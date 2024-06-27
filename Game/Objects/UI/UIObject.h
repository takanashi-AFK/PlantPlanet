#pragma once
#include "../../../Engine/GameObject/GameObject.h"
#include <vector>
#include "../../../Engine/Json/JsonReader.h"

using std::vector;

enum UIType {
	UI_NONE,
	UI_BUTTON,
	UI_IMAGE,
	UI_TEXT,
	UI_MAX,
};

class UIObject : public GameObject
{
protected:
	bool isEnable_;	// �L�����ǂ���
	UIType type_;	// UI�̎��
	int layerNumber_; // ���C���[�ԍ�

public:
	UIObject(string _name,UIType _type,GameObject* parent, int _layerNum);
   
	virtual void Initialize() override{}
	virtual void Update() override{}
	virtual void Draw() override{}
	virtual void Release() override{}

	virtual void Save(json& saveObj) {};
	virtual void Load(json& loadObj) {};
	virtual void DrawData() {};
	  
	void ChildSave(json& _saveObj);
	void ChildLoad(json& _loadObj);
	void ChildDrawData();

	// Getter
	int GetLayerNumber() { return layerNumber_; }

	//layerNumber_�̒l���\�[�g���邽�߂̊֐�
	static void SortLayerNumber(vector<UIObject*>& _objects);

};

UIObject* CreateUIObject(string _name, UIType _type, GameObject* _parent, int _layerNum);
string GetUITypeString(UIType _type);