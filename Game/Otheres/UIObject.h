#pragma once
#include"../../Engine/Json/JsonReader.h"
#include"../../Engine/GameObject/GameObject.h"
class UIObject: public GameObject
{

public:
	/// <summary>
	///�R���X�g���N�^ 
	/// </summary>
	/// <param name="_name">Ui�̃I�u�W�F�N�g�̖��O</param>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	UIObject(std::string _name, GameObject* _parent);

	 /// <summary>
    /// ������
   /// </summary>
   void Initialize() override;

     /// <summary>
    /// �X�V
    /// </summary>
    void Update() override;

    /// <summary>
   /// �`��
   /// </summary>
   void Draw() override;
	
	/// <summary>
	/// ���
	/// </summary>
	void Release() override;

	  /// <summary>
     /// �ۑ�
    /// </summary>
   /// <param name="_saveUiobj">�ۑ��f�[�^</param>
   virtual void Save(json& _saveUiobj) = 0;

	 /// <summary>
    /// �Ǎ�
   /// </summary>
  /// <param name="_loadUiobj">�Ǎ��f�[�^</param>
  virtual void Load(json& _loadUiobj) = 0;
};

