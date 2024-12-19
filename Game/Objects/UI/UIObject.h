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
	string objectName_;		// �I�u�W�F�N�g�̖��O
	UIType type_;			// �I�u�W�F�N�g�̎��
	int layerNumber_;		// ���C���[�ԍ�
	Transform transform_;	// �ʒu�A��]�A�g��k��
	UIObject* pParent_;		// �e�I�u�W�F�N�g
	bool isVisible_;		// �`�悷�邩

	std::vector<UIObject*> childList_;  // �q�I�u�W�F�N�g�̃��X�g

	//�t���O
	struct UI_STATE{
		unsigned initialized : 1;	//�������ς݂�
		unsigned entered : 1;		//�X�V���邩
		unsigned visible : 1;		//�`�悷�邩
		unsigned dead : 1;			//�폜���邩
	} state_;

public:
	/// <summary> �R���X�g���N�^ </summary>
	UIObject();
	UIObject(UIObject* parent);
	UIObject(string _name, UIType _type, UIObject* parent, int _layerNum);

	/// <summary> �f�X�g���N�^ </summary>
	virtual ~UIObject();

	/// <summary> ������ </summary>
	virtual void Initialize() {};

	/// <summary> �X�V </summary>
	virtual void Update() {};

	/// <summary> �`�� </summary>
	virtual void Draw() {};

	/// <summary> ��� </summary>
	virtual void Release() {};

	/// <summary> �ۑ� </summary>
	virtual void Save(json& saveObj) {};

	/// <summary> �Ǎ� </summary>
	virtual void Load(json& loadObj) {};

	/// <summary> ImGui�\�� </summary>
	virtual void DrawData() {};
	
	/// <summary> �q�I�u�W�F�N�g�̍X�V </summary>
	void UpdateSub();

	/// <summary> �q�I�u�W�F�N�g�̕`�� </summary>
	void DrawSub();

	/// <summary> �q�I�u�W�F�N�g�̉�� </summary>
	void ReleaseSub();
	
	/// <summary> �q�I�u�W�F�N�g�̕ۑ� </summary>
	void ChildSave(json& _saveObj);

	/// <summary> �q�I�u�W�F�N�g�̓Ǎ� </summary>
	void ChildLoad(json& _loadObj);

	/// <summary> �q�I�u�W�F�N�g��ImGui�\�� </summary>
	void ChildDrawData();

	/// <summary> �I�u�W�F�N�g�̒ǉ� </summary>
	void PushBackChild(UIObject* obj);

	/// <summary> ���C���[�ԍ���
	static bool CompareLayerNumber(UIObject* _object1, UIObject* _object2);

	void CreateEasing();
/*
setter :*/
	/// <summary> �ʒu�̌Œ� </summary>
	void LockPosition() { isPositionLocked_ = true; }

	/// <summary> �ʒu�̌Œ���� </summary>
	void UnlockPosition() { isPositionLocked_ = false; }
	
	/// <summary> ��]�̌Œ� </summary>
	void LockRotate() { isRotateLocked_ = true; }

	/// <summary> ��]�̌Œ���� </summary>
	void UnlockRotate() { isRotateLocked_ = false; }

	/// <summary> �g��k���̌Œ� </summary>
	void LockScale() { isScaleLocked_ = true; }

	/// <summary> �g��k���̌Œ���� </summary>
	void UnlockScale() { isScaleLocked_ = false; }

	/// <summary> ���S�t���O�𗧂Ă� </summary>
	void KillMe();

	/// <summary> ���C���[�ԍ���ݒ� </summary>
	void SetLayerNumber(int newLayerNumber_);

	/// <summary> ������ݒ� </summary>
	void SetVisible(bool _visible) { isVisible_ = _visible; }

	inline void SetTrasform(Transform t) { this->transform_ = t; }
/*
getter :*/
	/// <summary> �q�I�u�W�F�N�g���擾 </summary>
	UIObject* FindChildObject(const std::string& name);
	
	/// <summary> �I�u�W�F�N�g���擾 </summary>
	UIObject* FindObject(const std::string& name){ return GetRootJob()->FindChildObject(name); }

	/// <summary> �I�u�W�F�N�g�̖��O���擾 </summary>
	const string& GetObjectName(void) const;

	/// <summary> �I�u�W�F�N�g�̎�ނ��擾 </summary>
	UIType GetType(void) const;

	/// <summary> ���[�g�I�u�W�F�N�g���擾 </summary>
	UIObject* GetRootJob();
	
	/// <summary> �e�I�u�W�F�N�g���擾 </summary>
	UIObject* GetParent();
	
	/// <summary> ���C���[�ԍ����擾 </summary>
	int GetLayerNumber() const { return layerNumber_; }

	int GetMaxLayerNumber();

	Component_UIEasing* GetEasing();

	inline Transform GetTransform() { return this->transform_; }

	Transform GetCalcTransform(Transform _transform);
	Transform GetCalcTransform();

/*
predicate :*/
	/// <summary> �폜�t���O�������Ă��邩�ǂ��� </summary>
	bool IsDead();

	/// <summary> ���C���[�ԍ����d�����Ă��邩�ǂ��� </summary>
	bool IsLayerNumberDuplicate(int newLayerNumber_);

	static UIObject* CreateUIObject(string _name, UIType _type, UIObject* _parent, int _layerNum);
	static string GetUITypeString(UIType _type);

};

