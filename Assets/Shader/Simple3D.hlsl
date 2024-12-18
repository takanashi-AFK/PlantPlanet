#include<Alpha.hlsli>
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

    outData.globalPos = worldPos;
    outData.localPos = pos;
   // outData.pos = mul(outData.pos,g_matShadow); //�e�s����g���ĉe�̈ʒu���v�Z
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
	
  //  Dithering(inData.pos.xy, diffuse.a);
	
	//���C�g�̌���
	float4 lightDir = g_vecLightDir;	//�O���[�o���ϐ��͕ύX�ł��Ȃ��̂ŁA�������񃍁[�J���ϐ���
	lightDir = normalize(lightDir);	//�����������K�v�Ȃ̂Ő��K��

	//�@���̓s�N�Z���V�F�[�_�[�Ɏ����Ă������_�ŕ⊮���꒷�����ς���Ă���
	//���K�����Ă����Ȃ��Ɩʂ̖��邳�����������Ȃ�
	inData.normal = normalize(inData.normal);
	
	//�g�U���ˌ��i�f�B�t���[�Y�j
	//�@���ƌ��̃x�N�g���̓��ς��A�����̖��邳�ɂȂ�
    float4 shade = saturate(dot(inData.normal, -lightDir)) * (g_light );
    shade.rgb *= g_light.a;
	shade.a = 1;	//�Â��Ƃ��낪�����ɂȂ�̂ŁA�����I�ɃA���t�@��1

	//�����i�A���r�G���g�j
	//�����Maya���Ŏw�肵�A�O���[�o���ϐ��Ŏ󂯎�������̂����̂܂�
	float4 ambient = g_vecAmbient;
    ambient = 0.3;

	//���ʔ��ˌ��i�X�y�L�����[�j
	float4 speculer = float4(0, 0, 0, 0);	//�Ƃ肠�����n�C���C�g�͖����ɂ��Ă����āc
	if (g_vecSpeculer.a != 0)	//�X�y�L�����[�̏�񂪂����
	{
		float4 R = reflect(lightDir, inData.normal);			//�����˃x�N�g��
		speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer;	//�n�C���C�g�����߂�
	}

	//�ŏI�I�ȐF
	
    float grad = 4.0f;
    float base = g_ambinetLight.a;
	
    int3 temp = int3((shade.rgb - floor(shade.rgb)) * grad);
    float3 th = float3((temp.rgb / grad) + base);
	
    th.r = min(th.r, 1);
    th.g = min(th.g, 1);
    th.b = min(th.b, 1);
	
    diffuse.rgb += (g_ambinetLight.rgb) ;
	
    //float4 col = float4(diffuse.rgb * th + shade.rgb, 1);//beautiful shader
    float4 col = float4(diffuse.rgb * th.rgb,1);
	
    return float4(col);
}