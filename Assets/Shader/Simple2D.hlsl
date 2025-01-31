#include"CB2D.hlsli"
//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUTPUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	VS_OUTPUT output;
	output.pos = mul(pos, g_matWorld);
	output.uv = mul(uv, g_matTexture);
	return output;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUTPUT input) : SV_Target
{
    return g_vecColor * g_texture.Sample(g_sampler, input.uv);

}