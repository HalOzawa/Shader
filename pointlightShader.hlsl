//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer gModel:register(b0)
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW;
    float4x4 matNormal; //法線をワールド座標に対応させる行列＝回転＊スケールの逆行列（平行移動は無視
    float4 diffuseColor; //拡散反射係数
    float2 factor;
    bool isTextured; //テクスチャが張られているかどうか
};

cbuffer gStage:register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float4 wpos : POSITION0;
    float2 uv : TEXCOORD; //UV座標
    float4 normal : NORMAL;
    //float4 col : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    float4 spos = mul(pos, matWVP);
    float4 wpos = mul(pos, matW);
    float4 wnormal = mul(normal, matNormal);
    
    outData.pos = spos;
    outData.wpos = wpos;
    outData.uv = uv.xy;
    outData.normal = wnormal;
    //float4 dir = normalize(lightPosition - wpos);
    //outData.col = clamp(dot(normalize(wnormal), dir), 0, 1);
	
    //まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 diffuse;
    float4 ambient;
    
    float4 ambientSource = { 0.2, 0.2, 0.2, 1.0 };
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz);
    //inData.normal.z = 0;
    float color = saturate(dot(normalize(inData.normal.xyz), dir));
    //減衰
    float3 k = { 1.0f, 1.0f, 1.0f };
    float len = length(lightPosition.xyz - inData.wpos.xyz);
    float dTern = saturate(1.0 / (k.x + k.y*len + k.z*len*len));
    
    if (isTextured == false)
    {
        diffuse = diffuseColor * color * dTern * factor.x;
        ambient = diffuseColor * ambientSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * color * dTern*factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    return diffuse;
}