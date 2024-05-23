#pragma once
//#include "../../../"

//�Q�[���I�u�W�F�N�g����Ȃ��ăX�e�[�W�I�u�W�F�N�g���p������悤�ɕύX

const float BULLET_SPEED = 2.0f;
class Bullet :public GameObject
{
private:
	XMFLOAT3 Move_Position;
	XMVECTOR Move_Direction;
	SphereCollider* collision;
public:

	Bullet(GameObject* parent);
	~Bullet();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void Attack(XMFLOAT3 _playerPosition, XMVECTOR _direction);

};

