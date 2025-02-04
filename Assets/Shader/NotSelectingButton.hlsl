#include"CB2D.hlsli"
//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUTPUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
    VS_OUTPUT output;
    
    pos.xy *= 1;
    output.pos = mul(pos, g_matWorld);
    output.uv = mul(uv, g_matTexture);
    return output;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 col = g_vecColor * g_texture.Sample(g_sampler, input.uv);
    float gpos = ((input.pos.x / 1288.f) + (input.pos.y / 728.f)) / 2.f;
    gpos = 1 - gpos;
    col.a = gpos <= fade_ ? .0f : 1.f * col.a;
    return col;

}