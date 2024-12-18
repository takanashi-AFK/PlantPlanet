//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	g_matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	g_matNormalTrans;	// �@���̕ϊ��s��i��]�s��Ɗg��̋t�s��j
	float4x4	g_matWorld;			// ���[���h�ϊ��s��
	float4		g_vecLightDir;		// ���C�g�̕����x�N�g��
    float4      g_light;            // Color of light and Intensity
    float4      g_ambinetLight;     // Color of light and Intensity
	float4		g_vecDiffuse;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4		g_vecAmbient;		// �A���r�G���g�J���[�i�e�̐F�j
	float4		g_vecSpeculer;		// �X�y�L�����[�J���[�i�n�C���C�g�̐F�j
	float4		g_vecCameraPosition;// ���_�i�J�����̈ʒu�j
	float		g_shuniness;		// �n�C���C�g�̋����i�e�J����j
	int			g_isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
    float		val0;				//�V�F�[�_�[�̎��X�ɉ����Ďg���ėp���l
    float		val1;				//�V�F�[�_�[�̎��X�ɉ����Ďg���ėp���l
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float4 normal : TEXCOORD2; //�@��
    float2 uv : TEXCOORD0; //UV���W
    float4 eye : TEXCOORD1; //����
    float4 globalPos : TEXCOORD3; //���[���h���W
    float4 localPos : TEXCOORD4;
    //float4 shadowPos : TEXCOORD3;	//�e�̈ʒu	
};
