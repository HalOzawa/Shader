//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer gModel:register(b0)
{
    //�ϊ��s��A���_�A����
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW;
    float4x4 matNormal; //�@�������[���h���W�ɑΉ�������s�񁁉�]���X�P�[���̋t�s��i���s�ړ��͖���
    float4 diffuseColor; //�g�U���ˌW��
    float2 factor;
    bool isTextured; //�e�N�X�`���������Ă��邩�ǂ���
};

cbuffer gStage:register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float4 wpos : POSITION0;
    float2 uv : TEXCOORD; //UV���W
    float4 normal : NORMAL;
    //float4 col : COLOR; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    float4 spos = mul(pos, matWVP);
    float4 wpos = mul(pos, matW);
    float4 wnormal = mul(normal, matNormal);
    
    outData.pos = spos;
    outData.wpos = wpos;
    outData.uv = uv.xy;
    outData.normal = wnormal;
    //float4 dir = normalize(lightPosition - wpos);
    //outData.col = clamp(dot(normalize(wnormal), dir), 0, 1);
	
    //�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 diffuse;
    float4 ambient;
    
    float4 ambientSource = { 0.2, 0.2, 0.2, 1.0 };
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz);
    //inData.normal.z = 0;
    float color = saturate(dot(normalize(inData.normal.xyz), dir));
    //����
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