#include"CB2D.hlsli"
//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUTPUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
    VS_OUTPUT output;
    
    pos.xy *= 0.8;
    output.pos = mul(pos, g_matWorld);
    output.uv = mul(uv, g_matTexture);
    return output;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 col = g_vecColor * g_texture.Sample(g_sampler, input.uv);
    col.rgb *= 0.7;
    return col;

}