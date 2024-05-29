#pragma once
#include "DirectX/Direct3D.h"

// ���� �}�N�� �������������������������������������� //

//���S�Ƀ��������J�����邽�߂̃}�N��
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

// XMFLOAT3�^�̕ϐ����R���}��؂�ŕ\������}�N��
#define REFERENCE_XMFLOAT3(p) p.x,p.y,p.z 

// ���� �萔 ���������������������������������������� //

const float FPS = 60.0f;	// �t���[�����[�g


// �������������������������������������������������� //
