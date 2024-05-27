#pragma once
#include "StageObject.h"

//�Q�[���I�u�W�F�N�g����Ȃ��ăX�e�[�W�I�u�W�F�N�g���p������悤�ɕύX


class Bullet :public StageObject
{
private:
	SphereCollider* collision;
	int modelHandle_;
	bool isActive_;
	int frame;
	float speed_;
	XMVECTOR direction_;
	const int FPS = GetPrivateProfileInt("GAME", "Fps", 60, ".\\setup.ini");
public:

	Bullet(GameObject* _parent);
	~Bullet();

	//Update�֐����I�[�o�[���[�h
	//XMFLOAT3 _playerPosition, XMVECTOR _direction�������ɒǉ�
	//���������ɒe�𔭎˂��鏈����ǉ�
	//�e�̈ړ�������ǉ�
	//Cactan��Update�֐���Bullet��Update���Ăяo��?


	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void DrawData() override;
	void Save(json& _saveobj) override;
	void Load(json& _loadobj) override;

	void SetSpeed(float _speed) { speed_ = _speed; }
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }


	/// <summary>
	/// �U���J�n
	/// </summary>
	void Execute() { isActive_ = true; }
};

