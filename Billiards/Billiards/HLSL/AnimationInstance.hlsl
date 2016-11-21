//��`
#define MAX_BONE_MATRICES 255

struct PerInstanceData
{
	matrix instanceMat;
};

StructuredBuffer<PerInstanceData>	g_pInstanceData :register(t0);


cbuffer global_0:register(b0)
{
	float4 g_vLight;  //���C�g�̕����x�N�g��
	float4 g_vEye;//�J�����ʒu
};

cbuffer cbGlobal : register(b1)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix WVP;
};

cbuffer global_bones:register(b2)//�{�[���̃|�[�Y�s�񂪓���
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

cbuffer cbMaterial : register(b3)
{
	float4 g_Ambient;
	float4 g_Diffuse;
	float4 g_Specular;
	float g_Power;
	float4 g_Emmisive;
};

//�X�L�j���O��̒��_�E�@��������
struct Skin
{
	float4 Pos;
	float3 Norm;
};

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSSkinIn
{
	float3 Pos	: POSITION;//�ʒu   
	float3 Norm : NORMAL;//���_�@��
	float2 Tex	: TEXCOORD;//�e�N�X�`���[���W
	uint4  Bones : BONE_INDEX;//�{�[���̃C���f�b�N�X
	float4 Weights : BONE_WEIGHT;//�{�[���̏d��
};

//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct PSSkinIn
{
	float4 Pos	: SV_POSITION;//�ʒu
	float3 Norm : NORMAL;//���_�@��
	float2 Tex	: TEXCOORD;//�e�N�X�`���[���W
	float4 Color : COLOR0;//�ŏI�J���[�i���_�V�F�[�_�[�ɂ����Ắj
};

//
//matrix FetchBoneMatrix( uint iBone )
//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ��@�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
matrix FetchBoneMatrix(uint iBone)
{
	return  g_mConstBoneWorld[iBone];
}
//
// Skin SkinVert( VSSkinIn Input )
//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos, 1);
	//Pos.x *= -1;
	float3 Norm = Input.Norm;
	//�{�[��0
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3)m);
	//�{�[��1
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3)m);
	//�{�[��2
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3)m);
	//�{�[��3
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3)m);

	/*float w[4] = (float[4])Input.Weights;
	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 4; i++)
	{
	comb +=
	}*/

	return Output;
}

//
//PSSkinIn VSSkin(VSSkinIn input )
//�o�[�e�b�N�X�V�F�[�_�[
PSSkinIn VSSkin(VSSkinIn input, uint instanceID : SV_InstanceID)
{
	PSSkinIn output;

	Skin vSkinned = SkinVert(input);

	matrix instanceWVP = mul(Projection, View);
	instanceWVP = mul(instanceWVP, World);
	instanceWVP = mul(instanceWVP, g_pInstanceData[instanceID].instanceMat);

	instanceWVP = transpose(instanceWVP);
	output.Pos = mul(vSkinned.Pos, instanceWVP);
	output.Norm = normalize(mul(vSkinned.Norm, (float3x3)World));
	output.Tex = input.Tex;
	float3 LightDir = normalize(g_vLight);
	float3 PosWorld = mul(vSkinned.Pos, World);
	float3 ViewDir = normalize(g_vEye - PosWorld);
	float3 Normal = normalize(output.Norm);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);



	output.Color = g_Diffuse * NL + specular*g_Specular;

	return output;

	//PSSkinIn output;

	//Skin vSkinned = SkinVert(input);

	//output.Pos = mul(float4(input.Pos, 1.0f), WVP);
	//output.Norm = /*normalize(mul(*/input.Norm/*, (float3x3)World))*/;
	//output.Tex = input.Tex;
	//float3 LightDir = normalize(g_vLight);
	//float3 PosWorld = mul(vSkinned.Pos, World);
	//float3 ViewDir = normalize(g_vEye - PosWorld);
	//float3 Normal = normalize(output.Norm);
	//float4 NL = saturate(dot(Normal, LightDir));

	//float3 Reflect = normalize(2 * NL * Normal - LightDir);
	//float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);



	//output.Color = g_Diffuse * NL + specular*g_Specular;

	//return output;
}