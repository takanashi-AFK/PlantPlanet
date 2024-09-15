#include "CountDown.h"

// �C���N���[�h
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/GameObject/Camera.h"

namespace {
	const int COUNT_DOWN_MAX = 240;		// �J�E���g�_�E���̍ő�l
	const int FPS = 60;					// �t���[�����[�g
	const int COUNT_DOWN_IMAGE_NUM = 4; // �J�E���g�_�E���̉摜�̐�
}

CountDown::CountDown(GameObject* _parent):
	GameObject(_parent,"CountDown"),count_(COUNT_DOWN_MAX)
{
}

void CountDown::Initialize()
{
	for (int i = 0; i < COUNT_DOWN_IMAGE_NUM; i++) countDownImages_.push_back(Image::Load("Images/playScene/countDown/count_" + std::to_string(i) + ".png"));
}

void CountDown::Update()
{
}

void CountDown::Draw()
{
	// �J�E���g��0�ȉ��Ȃ�`�悵�Ȃ�
	if(this->IsFinished())return;

	// �J�E���g����摜�̃C���f�b�N�X���擾
	int index = (int)(count_ / FPS);

	float camScale = count_ * 0.8f;
	Camera::SetPosition(camScale + 1, camScale + 1, camScale + 1);

	// �摜�̕`��
	Image::SetTransform(countDownImages_[index], transform_);
	Image::Draw(countDownImages_[index]);
	
	// �J�E���g������
	count_--;
}

void CountDown::Release()
{
}
