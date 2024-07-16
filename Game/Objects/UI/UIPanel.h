#pragma once
#include "../../../Engine/Json/JsonReader.h"
#include <vector>
#include "UIObject.h"


using std::vector;


class UIPanel : public UIObject
{
private:

	//�V���O���g���C���X�^���X
	static UIPanel* instance_;

	//�R���X�g���N�^
	UIPanel();

public:

	//�V���O���g���C���X�^���X�̎擾
	static UIPanel* GetInstance();
	
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void Save(json& _saveObj)override;
	void Load(json& _loadObj)override;
	void DrawData()override;

	//void AddUIObject(UIObject* _object);
	void DeleteUIObject(UIObject* _object);
	void DeleteAllUIObject();

	//���C���[�ԍ����Ƀ\�[�g
	void SortUIObject();
	vector<UIObject*> GetUIObjects() { return childList_; };

	// �Z�b�^�[��ǉ�
	void SetName(const std::string& name){ objectName_ = name; }
	void SetParent(UIObject* parent) { pParent_ = parent; }
	void SetLayerNum(int layerNum) { layerNumber_ = layerNum; }



};

