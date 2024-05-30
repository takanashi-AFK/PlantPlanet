#pragma once

// �C���N���[�h
#include "../../../../Engine/Json/JsonReader.h"

// �O���錾
class StageObject;

// using�錾
using std::vector;

// �R���|�[�l���g�^�C�v
enum ComponentType {
    Rotation,
    RotationY,
    RotationX,
    RotationZ,
    MoveX,
    HealthManager,
    Chase,
	CircleRangeDetector,
    FanRangeDetector,
    OtiBehavior,
    Timer,
    Fall,
    Attack,
    CactanAttack,
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
    /// ������
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>
    /// �X�V
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// �J��
    /// </summary>
    virtual void Release() = 0;

    /// <summary>
    /// �ۑ�
    /// </summary>
    /// <param name="_saveObj">�ۑ����</param>
    virtual void Save(json& _saveObj) = 0;

    /// <summary>
    /// �Ǎ�
    /// </summary>
    /// <param name="_saveObj">�Ǎ����</param>
    virtual void Load(json& _loadObj) = 0;

    /// <summary>
    /// ImGui�p�l���\��
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
    /// �q�R���|�[�l���g�����X�g�ɒǉ�
    /// </summary>
    /// <param name="comp">�R���|�[�l���g�^�C�v</param>
    bool AddChildComponent(Component* _comp);

    /// <summary>
    /// �q�R���|�[�l���g�����X�g����폜
    /// </summary>
    /// <param name="comp">�R���|�[�l���g�^�C�v</param>
    bool DeleteChildComponent(Component* _comp);

    /// <summary>
    /// �q�R���|�[�l���g�ɊY���̃^�C�v�̃R���|�[�l���g�����X�g�ɂ��邩��T��
    /// </summary>
    /// <param name="_type">�R���|�[�l���g�^�C�v</param>
    /// <returns>�����������ǂ���</returns>
    bool FindChildComponent(ComponentType _type);

    /// <returns>
    /// �q�R���|�[�l���g���擾
    /// </returns>
    /// <param name="_type">�R���|�[�l���g�^�C�v</param>
    Component* GetChildComponent(ComponentType _type);
    
    /// <returns>
    /// �R���|�[�l���g�^�C�v���擾
    /// </returns>
    ComponentType GetType() const { return type_; }

    /// <returns>
    /// �R���|�[�l���g�����擾
    /// </returns>
    string GetName() { return name_; }

};

/// <summary>
/// �R���|�[�l���g���쐬����
/// </summary>
/// <param name="_type">�R���|�[�l���g�^�C�v</param>
/// <param name="_holder">�ۗL��</param>
/// <returns>�쐬�����R���|�[�l���g</returns>
Component* CreateComponent(string _name,ComponentType _type,StageObject* _holder);