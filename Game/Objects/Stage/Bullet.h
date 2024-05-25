#pragma once
#include "StageObject.h"

//�Q�[���I�u�W�F�N�g����Ȃ��ăX�e�[�W�I�u�W�F�N�g���p������悤�ɕύX


class Bullet :public StageObject
{
private:
	XMFLOAT3 Move_Position;
	SphereCollider* collision;
	int modelHandle_;
	bool isShot_;
	int frame;
	float speed_;
	XMVECTOR direction_;
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

	void Attack(XMFLOAT3 _playerPosition);

	void SetSpeed(float _speed) { speed_ = _speed; }
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }
};

