//定数バッファのデータ定義　変更しないデータ
cbuffer cbNeverChanges	:	register(b0){	//常にスロット「0」を使う
	matrix Projection;	//透視変換行列
};

//定数バッファのデータ定義　変更頻度の低いデータ
cbuffer cbChangesEveryFrame	:	register(b1){	//常にスロット[1]を使う
	matrix View;	//ビュー変換行列
	float3 Light;	//光源座標(ビュー座標系)
};

//定数バッファのデータ定義　変更頻度の高いデータ
cbuffer cbChangeEveryObject	:	register(b2){	//常にスロット[2]を使う
	matrix World;	//ワールド変換行列
	float4 Seek;	//シーク
};

//頂点シェーダの入力データの定義
struct VS_INPUT{
	float3 Pos	:	POSITION;	//頂点座標（モデル座標系）
	float3 Col	:	COLOR;		//頂点色
};

//ジオメトリシェーダの入力データの定義
struct GS_INPUT{
	float4 Pos	:	SV_POSITION;	//頂点座標(ビュー座標系)
	float4 Col	:	COLOR;			//頂点色
};

//ピクセルシェーダ―の入力データの定義
struct PS_INPUT{
	float4 Pos	:	SV_POSITION;	//頂点座標(透視座標系)
	float3 PosView	:	POSITION;	//頂点座標(ビュー座標系)
	float3 Norm	:	NORMAL;		//法線ベクトル(ワールド座標系)
	float2	Tex	:	TEXTRUE;	//テクスチャ座標
	float4	Col	:	COLOR;			//頂点色
};

//2Dテクスチャ
Texture2D myTex;

//サンプラ
SamplerState mySampler	:	register(s0);

//頂点シェーダの関数
GS_INPUT VS(VS_INPUT input){
	GS_INPUT output;
	//モデル座標系　→　ビュー座標系
	float4 pos4 = float4(input.Pos,1.0);
	output.Pos = mul(pos4,World);
	output.Pos = mul(output.Pos,View);
	//頂点色
	output.Col = float4(input.Col,1.0);
	//出力
	return output;
}

//ジオメトリシェーダの関数
[maxvertexcount(3)]
void GS(triangle GS_INPUT input[3] : SV_POSITION,uint pID : SV_PrimitiveID,
		inout TriangleStream<PS_INPUT> TriStream){
	PS_INPUT output;

	//テクスチャ座標の計算
	static const float2 tID[6] ={ {0.0,0.0},{1.0,0.0},{1.0,1.0},
								{0.0,0.0},{1.0,1.0},{0.0,1.0}};	//テクスチャ座標(0.0,0.0)～(1.0,1.0)
								
	uint tIdx = (pID&0x01)?3:0;	//プリミティブIDを元に、テクスチャ座標を決める


	//法線ベクトルの計算
	float3 faceEdge = input[0].Pos.xyz / input[0].Pos.w;
	float3 faceEdgeA = (input[1].Pos.xyz / input[1].Pos.w) - faceEdge;
	float3 faceEdgeB = (input[2].Pos.xyz / input[2].Pos.w) - faceEdge;
	output.Norm = normalize(cross(faceEdgeA,faceEdgeB));
	
	//各頂点の計算
	for(int i=0;i<3;++i){
		//頂点座標　ビュー座標系
		output.PosView = input[i].Pos.xyz / input[i].Pos.w;
		//頂点座標　ビュー座標系　→　射影座標系
		output.Pos = mul(input[i].Pos,Projection);
		//頂点色
		output.Col = input[i].Col;
		//テクスチャ座標
		output.Tex = tID[tIdx + i];
		//出力
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}

//ジオメトリシェーダ関数（文字表示用）
[maxvertexcount(3)]
void GS_STRING(triangle GS_INPUT input[3] : SV_POSITION,uint pID : SV_PrimitiveID,
		inout TriangleStream<PS_INPUT> TriStream){
	PS_INPUT output;

	//テクスチャ座標の計算
	static const float2 tID[6] ={ {Seek.x,Seek.y},{Seek.z,Seek.y},{Seek.z,Seek.w},
								{Seek.x,Seek.y},{Seek.z,Seek.w},{Seek.x,Seek.w}};	//テクスチャ座標(0.0,0.0)～(1.0,1.0)
	
	/*static const float2 tID[6] ={ {0.0,0.0},{1.0,0.0},{1.0,1.0},
							{0.0,0.0},{1.0,1.0},{0.0,1.0}};	//テクスチャ座標(0.0,0.0)～(1.0,1.0)
	*/
	uint tIdx = (pID&0x01)?3:0;	//プリミティブIDを元に、テクスチャ座標を決める


	//法線ベクトルの計算
	float3 faceEdge = input[0].Pos.xyz / input[0].Pos.w;
	float3 faceEdgeA = (input[1].Pos.xyz / input[1].Pos.w) - faceEdge;
	float3 faceEdgeB = (input[2].Pos.xyz / input[2].Pos.w) - faceEdge;
	output.Norm = normalize(cross(faceEdgeA,faceEdgeB));
	
	//各頂点の計算
	for(int i=0;i<3;++i){
		//頂点座標　ビュー座標系
		output.PosView = input[i].Pos.xyz / input[i].Pos.w;
		//頂点座標　ビュー座標系　→　射影座標系
		output.Pos = mul(input[i].Pos,Projection);
		//頂点色
		output.Col = input[i].Col;
		//テクスチャ座標
		output.Tex = tID[tIdx + i];
		//出力
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}

//ピクセルシェーダの関数
float4 PS(PS_INPUT input)	:	SV_TARGET
{
	//光源ベクトル
	float3 light = Light - input.PosView;
	//距離
	float len = length(light);
	//明るさ
	float bright = 30 * dot(normalize(light),input.Norm) / pow(len,2);
	
	//テクスチャ
	uint width,height;
	myTex.GetDimensions(width,height);	//幅、高さ
	float3 texCoord = float3(input.Tex.x*width,input.Tex.y*height,0);//ミップマップレベル「０」
	float4 texCol = myTex.Load(texCoord);	//テクセル読み込み

	//色
	return saturate(bright*texCol);
}

//ピクセルシェーダのSample関数
float4 PS_Sample(PS_INPUT input)	:	SV_TARGET
{
	//光源ベクトル
	float3 light = Light - input.PosView;
	//距離
	float len = length(light);
	//明るさ
	float bright = 30 * dot(normalize(light),input.Norm) / pow(len,2);

	//テクスチャ取得
	float4 texCol = myTex.Sample(mySampler,input.Tex);	//テクセル読み込み
	
	//色
	return saturate(texCol);
}