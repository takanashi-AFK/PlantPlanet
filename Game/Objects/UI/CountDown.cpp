#include "CountDown.h"

// インクルード
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/GameObject/Camera.h"

namespace {
	const int COUNT_DOWN_MAX = 240;		// カウントダウンの最大値
	const int FPS = 60;					// フレームレート
	const int COUNT_DOWN_IMAGE_NUM = 4; // カウントダウンの画像の数
}

CountDown::CountDown(GameObject* _parent):
	GameObject(_parent,"CountDown"),count_(COUNT_DOWN_MAX)
{
}

void CountDown::Initialize()
{
	for (int i = 0; i < COUNT_DOWN_IMAGE_NUM; i++) countDownImages_.push_back(Image::Load("Images/playScene/SCA/countDown/count_" + std::to_string(i) + ".png"));
}

void CountDown::Update()
{
}

void CountDown::Draw()
{
	// カウントダウンが有効でないなら描画しない	
	if (isActive_ = false) return;

	// カウントが0以下なら描画しない
	if(this->IsFinished())return;

	// カウントから画像のインデックスを取得
	int index = (int)(count_ / FPS);

	float camScale = count_ * 0.8f;
	Camera::SetPosition(camScale + 1, camScale + 1, camScale + 1);

	// 画像の描画
	Image::SetTransform(countDownImages_[index], transform_);
	Image::Draw(countDownImages_[index],NULL);
	
	// カウントを減少
	count_--;
}

void CountDown::Release()
{
}

float CountDown::GetCount() const
{
	return count_ / FPS;
}
