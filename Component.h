#pragma once

// �C���N���[�h
#include "Engine/Json/JsonReader.h"

// �O���錾
class StageObject;

// using�錾
using std::vector;


// �R���|�[�l���g�^�C�v
enum ComponentType {

};


// �R���|�\�l���g���N���X
class Component
{
protected:
    string name_;                           // �R���|�[�l���g��
    StageObject* holder_;                   // �R���|�\�l���g�ۗL�҂̃|�C���^
    vector<Component*> childComponents_;    // �q�R���|�[�l���g�Q
    ComponentType type_;
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="_holder">�ۗL��</param>
    /// <param name="_name">���O</param>
    Component(StageObject* _holder, string _name,ComponentType _type);

    /// <summary>
    /// �������������s���������z�֐�
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>
    /// �X�V�������s���������z�֐�
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// �J���������s���������z�֐�
    /// </summary>
    virtual void Release() = 0;

    /// <summary>
    /// json�t�@�C���Ɍ��݂̃f�[�^��ۑ����鏃�����z�֐�
    /// </summary>
    /// <param name="_saveObj">�Z�[�u����f�[�^</param>
    virtual void Save(json& _saveObj) = 0;

    /// <summary>
    /// json�t�@�C���Ɍ��݂̃f�[�^��ۑ����鏃�����z�֐�
    /// </summary>
    /// <param name="_saveObj">�Z�[�u����f�[�^</param>
    virtual void Load(json& _loadObj) = 0;

    /// <summary>
    /// �R���|�[�l���g�������Ă���f�[�^��ImGui�ɕ\������֐�
    /// </summary>
    virtual void DrawData() {};

    /// <summary>
    /// ���g���q�R���|�[�l���g��������
    /// </summary>
    void ChildIntialize();

	/// <summary>
	/// ���g���q�R���|�[�l���g���X�V
	/// </summary>
	void ChildUpdate();

    /// <summary>
    /// ���g���q�R���|�[�l���g�����
    /// </summary>
    void ChildRelease();

	/// <summary>
	/// ���g���q�R���|�[�l���g��ۑ�
	/// </summary>
	/// <param name="_saveObj"></param>
	void ChildSave(json& _saveObj);

    /// <summary>
    /// ���g���q�R���|�[�l���g��ۑ�
    /// </summary>
    /// <param name="_saveObj"></param>
    void ChildLoad(json& _saveObj);

	/// <summary>
	/// ���g���q�R���|�[�l���g��`��
	/// </summary>
	void ChildDrawData();

    /// <summary>
    /// ���g�̃R���|�[�l���g�^�C�v���擾
    /// </summary>
    /// <returns>�R���|�[�l���g�^�C�v</returns>
    ComponentType GetType() { return type_; }
};

/// <summary>
    /// �R���|�[�l���g�C���X�^���X�𐶐�
    /// </summary>
    /// <param name="_type">�R���|�[�l���g�^�C�v</param>
    /// <returns>���������R���|�[�l���g</returns>
Component* LoadComponent(ComponentType _type, json& _loadObj);
