//�萔�o�b�t�@�̃f�[�^��`�@�ύX���Ȃ��f�[�^
cbuffer cbNeverChanges	:	register(b0){	//��ɃX���b�g�u0�v���g��
	matrix Projection;	//�����ϊ��s��
};

//�萔�o�b�t�@�̃f�[�^��`�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer cbChangesEveryFrame	:	register(b1){	//��ɃX���b�g[1]���g��
	matrix View;	//�r���[�ϊ��s��
	float3 Light;	//�������W(�r���[���W�n)
};

//�萔�o�b�t�@�̃f�[�^��`�@�ύX�p�x�̍����f�[�^
cbuffer cbChangeEveryObject	:	register(b2){	//��ɃX���b�g[2]���g��
	matrix World;	//���[���h�ϊ��s��
	float4 Seek;	//�V�[�N
};

//���_�V�F�[�_�̓��̓f�[�^�̒�`
struct VS_INPUT{
	float3 Pos	:	POSITION;	//���_���W�i���f�����W�n�j
	float3 Col	:	COLOR;		//���_�F
};

//�W�I���g���V�F�[�_�̓��̓f�[�^�̒�`
struct GS_INPUT{
	float4 Pos	:	SV_POSITION;	//���_���W(�r���[���W�n)
	float4 Col	:	COLOR;			//���_�F
};

//�s�N�Z���V�F�[�_�\�̓��̓f�[�^�̒�`
struct PS_INPUT{
	float4 Pos	:	SV_POSITION;	//���_���W(�������W�n)
	float3 PosView	:	POSITION;	//���_���W(�r���[���W�n)
	float3 Norm	:	NORMAL;		//�@���x�N�g��(���[���h���W�n)
	float2	Tex	:	TEXTRUE;	//�e�N�X�`�����W
	float4	Col	:	COLOR;			//���_�F
};

//2D�e�N�X�`��
Texture2D myTex;

//�T���v��
SamplerState mySampler	:	register(s0);

//���_�V�F�[�_�̊֐�
GS_INPUT VS(VS_INPUT input){
	GS_INPUT output;
	//���f�����W�n�@���@�r���[���W�n
	float4 pos4 = float4(input.Pos,1.0);
	output.Pos = mul(pos4,World);
	output.Pos = mul(output.Pos,View);
	//���_�F
	output.Col = float4(input.Col,1.0);
	//�o��
	return output;
}

//�W�I���g���V�F�[�_�̊֐�
[maxvertexcount(3)]
void GS(triangle GS_INPUT input[3] : SV_POSITION,uint pID : SV_PrimitiveID,
		inout TriangleStream<PS_INPUT> TriStream){
	PS_INPUT output;

	//�e�N�X�`�����W�̌v�Z
	static const float2 tID[6] ={ {0.0,0.0},{1.0,0.0},{1.0,1.0},
								{0.0,0.0},{1.0,1.0},{0.0,1.0}};	//�e�N�X�`�����W(0.0,0.0)�`(1.0,1.0)
								
	uint tIdx = (pID&0x01)?3:0;	//�v���~�e�B�uID�����ɁA�e�N�X�`�����W�����߂�


	//�@���x�N�g���̌v�Z
	float3 faceEdge = input[0].Pos.xyz / input[0].Pos.w;
	float3 faceEdgeA = (input[1].Pos.xyz / input[1].Pos.w) - faceEdge;
	float3 faceEdgeB = (input[2].Pos.xyz / input[2].Pos.w) - faceEdge;
	output.Norm = normalize(cross(faceEdgeA,faceEdgeB));
	
	//�e���_�̌v�Z
	for(int i=0;i<3;++i){
		//���_���W�@�r���[���W�n
		output.PosView = input[i].Pos.xyz / input[i].Pos.w;
		//���_���W�@�r���[���W�n�@���@�ˉe���W�n
		output.Pos = mul(input[i].Pos,Projection);
		//���_�F
		output.Col = input[i].Col;
		//�e�N�X�`�����W
		output.Tex = tID[tIdx + i];
		//�o��
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}

//�W�I���g���V�F�[�_�֐��i�����\���p�j
[maxvertexcount(3)]
void GS_STRING(triangle GS_INPUT input[3] : SV_POSITION,uint pID : SV_PrimitiveID,
		inout TriangleStream<PS_INPUT> TriStream){
	PS_INPUT output;

	//�e�N�X�`�����W�̌v�Z
	static const float2 tID[6] ={ {Seek.x,Seek.y},{Seek.z,Seek.y},{Seek.z,Seek.w},
								{Seek.x,Seek.y},{Seek.z,Seek.w},{Seek.x,Seek.w}};	//�e�N�X�`�����W(0.0,0.0)�`(1.0,1.0)
	
	/*static const float2 tID[6] ={ {0.0,0.0},{1.0,0.0},{1.0,1.0},
							{0.0,0.0},{1.0,1.0},{0.0,1.0}};	//�e�N�X�`�����W(0.0,0.0)�`(1.0,1.0)
	*/
	uint tIdx = (pID&0x01)?3:0;	//�v���~�e�B�uID�����ɁA�e�N�X�`�����W�����߂�


	//�@���x�N�g���̌v�Z
	float3 faceEdge = input[0].Pos.xyz / input[0].Pos.w;
	float3 faceEdgeA = (input[1].Pos.xyz / input[1].Pos.w) - faceEdge;
	float3 faceEdgeB = (input[2].Pos.xyz / input[2].Pos.w) - faceEdge;
	output.Norm = normalize(cross(faceEdgeA,faceEdgeB));
	
	//�e���_�̌v�Z
	for(int i=0;i<3;++i){
		//���_���W�@�r���[���W�n
		output.PosView = input[i].Pos.xyz / input[i].Pos.w;
		//���_���W�@�r���[���W�n�@���@�ˉe���W�n
		output.Pos = mul(input[i].Pos,Projection);
		//���_�F
		output.Col = input[i].Col;
		//�e�N�X�`�����W
		output.Tex = tID[tIdx + i];
		//�o��
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}

//�s�N�Z���V�F�[�_�̊֐�
float4 PS(PS_INPUT input)	:	SV_TARGET
{
	//�����x�N�g��
	float3 light = Light - input.PosView;
	//����
	float len = length(light);
	//���邳
	float bright = 30 * dot(normalize(light),input.Norm) / pow(len,2);
	
	//�e�N�X�`��
	uint width,height;
	myTex.GetDimensions(width,height);	//���A����
	float3 texCoord = float3(input.Tex.x*width,input.Tex.y*height,0);//�~�b�v�}�b�v���x���u�O�v
	float4 texCol = myTex.Load(texCoord);	//�e�N�Z���ǂݍ���

	//�F
	return saturate(bright*texCol);
}

//�s�N�Z���V�F�[�_��Sample�֐�
float4 PS_Sample(PS_INPUT input)	:	SV_TARGET
{
	//�����x�N�g��
	float3 light = Light - input.PosView;
	//����
	float len = length(light);
	//���邳
	float bright = 30 * dot(normalize(light),input.Norm) / pow(len,2);

	//�e�N�X�`���擾
	float4 texCol = myTex.Sample(mySampler,input.Tex);	//�e�N�Z���ǂݍ���
	
	//�F
	return saturate(texCol);
}