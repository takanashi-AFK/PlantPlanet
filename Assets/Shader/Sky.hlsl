#include"CB3D.hlsli"

//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture: register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[


//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, g_matWVP);		

	//�@���̕ό`
	Normal.w = 0;					//4�����ڂ͎g��Ȃ��̂�0
	Normal = mul(Normal, g_matNormalTrans);		//�I�u�W�F�N�g���ό`����Ζ@�����ό`
	outData.normal = Normal;		//������s�N�Z���V�F�[�_�[��

	//�����x�N�g���i�n�C���C�g�̌v�Z�ɕK�v
	float4 worldPos = mul(pos, g_matWorld);					//���[�J�����W�Ƀ��[���h�s��������ă��[���h���W��
	outData.eye = normalize(g_vecCameraPosition - worldPos);	//���_���璸�_�ʒu�������Z�����������߂ăs�N�Z���V�F�[�_�[��

	//UV�u���W
	outData.uv = Uv;	//���̂܂܃s�N�Z���V�F�[�_�[��

    outData.localPos = mul(pos, g_matWorld);

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	float4 diffuse;
	//�e�N�X�`���L��
	if (g_isTexture == true)
	{
		//�e�N�X�`���̐F
		diffuse = g_texture.Sample(g_sampler, inData.uv);
	}
	else
	{
		//�}�e���A���̐F
		diffuse = g_vecDiffuse;
	}
	
	//�ŏI�I�ȐF
    return diffuse;
}