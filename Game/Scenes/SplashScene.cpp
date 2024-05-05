#include "SplashScene.h"

// �C���N���[�h
#include "../../Engine/ResourceManager/Image.h"
#include "../../Engine/SceneManager.h"

// �萔
namespace {
	const float FPS = 60.f;			// �t���[�����[�g
	const float LIMIT_TIME = 1.f;	// �V�[����؂�ւ���܂łɊ|���鎞�ԁi�b�j
	const float IMAGE_REDUCTION_RATE = 0.67f;	// �X�v���b�V���摜�̏k���{��
}

SplashScene::SplashScene(GameObject* _parent)
	:GameObject(_parent,"SplashScene"),imageHandle_(-1),count_(0)
{
}

void SplashScene::Initialize()
{
	// �摜�̓Ǎ�
	imageHandle_ = Image::Load("Images/splash_Image_dungeon_Path.png");
	assert(imageHandle_ >= 0);

	// �摜�T�C�Y���w��
	transform_.scale_ = { IMAGE_REDUCTION_RATE,IMAGE_REDUCTION_RATE,1.f };
}

void SplashScene::Update()
{
	// n�b�o�߂�����V�[�����ړ�
	if (count_ >= LIMIT_TIME * FPS)((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID_TEST, TID_BLACKOUT);
	
	// �J�E���g�A�b�v
	count_++;
}

void SplashScene::Draw()
{
	// �摜�̕`��
	Image::SetTransform(imageHandle_,transform_);
	Image::Draw(imageHandle_);
}

void SplashScene::Release()
{
}
