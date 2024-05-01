#pragma once

// �C���N���[�h
#include "../../../../Engine/Json/JsonReader.h"

// �O���錾
class StageObject;

// using�錾
using std::vector;

// �R���|�[�l���g�^�C�v
enum ComponentType {
    RotationY,

    // �R���|�[�l���g�ǉ����Ɏ��ʔԍ���ǉ�
};

// �R���|�\�l���g���N���X
class Component
{
protected:
    string name_;                           // �R���|�[�l���g��
    ComponentType type_;                    // �R���|�[�l���g�^�C�v
    StageObject* holder_;                   // �R���|�\�l���g�ۗL�҂̃|�C���^
    vector<Component*> childComponents_;    // �q�R���|�[�l���g�Q
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
	/// ���g���q�R���|�[�l���g��`��
	/// </summary>
	void ChildDrawData();

    /// <summary>
    /// ���g���q�R���|�[�l���g��ۑ�
    /// </summary>
    void ChildSave(json& _saveObj);

    /// <summary>
    /// ���g���q�R���|�[�l���g��Ǎ�
    /// </summary>
    void ChildLoad(json& _loadObj);

    /// <summary>
    /// �q�R���|�[�l���g��ǉ�
    /// </summary>
    /// <param name="comp"></param>
    bool AddChildComponent(Component* _comp);

    /// <summary>
    /// �q�R���|�[�l���g���폜
    /// </summary>
    /// <param name="comp"></param>
    bool DeleteChildComponent(Component* _comp);

// getter
    ComponentType GetType() { return type_; }
    string GetName() { return name_; }
};

/// <summary>
///�R���|�[�l���g���쐬����
/// </summary>
/// <param name="_type">�R���|�[�l���g�^�C�v</param>
/// <param name="_holder">�ۗL��</param>
/// <returns>�쐬�����R���|�[�l���g</returns>
Component* CreateComponent(ComponentType _type,StageObject* _holder);