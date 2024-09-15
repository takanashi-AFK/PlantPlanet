#pragma once
#include "GameObject.h"

namespace Camera
{
	/// <summary> �J�����̏����� </summary>
	void Initialize();

	/// <summary> �J�����̍X�V </summary>
	void Update();

/*
getter :*/
	/// <summary> �ʒu���擾 </summary>
	XMFLOAT3 GetPosition();

	/// <summary> �œ_���擾 </summary>
	XMFLOAT3 GetTarget();

	/// <summary> �r���[�s����擾 </summary>
	XMMATRIX GetViewMatrix();

	/// <summary> �v���W�F�N�V�����s����擾 </summary>
	XMMATRIX GetProjectionMatrix();

	/// <summary> �r���{�[�h�s����擾 </summary>
	XMMATRIX GetBillboardMatrix();

	/// <summary> �������擾 </summary>
	XMVECTOR GetSightLine();
/*
setter :*/
	/// <summary> �ʒu��ݒ� </summary>
	void SetPosition(XMFLOAT3 position);
	void SetPosition(float x, float y, float z);

	/// <summary> �œ_��ݒ� </summary>
	void SetTarget(XMFLOAT3 target);
	void SetTarget(float x, float y, float z);

	/// <summary> �v���W�F�N�V�����s���ݒ� </summary>
	void SetProjectionMatrix(XMMATRIX proj);
	void SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ);
	void SetProjectionMatrix(float aspect);

};