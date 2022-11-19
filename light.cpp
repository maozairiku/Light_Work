//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "player.h"
#include <math.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static	LIGHT	g_Light[LIGHT_MAX];

static	FOG		g_Fog;

static	BOOL	g_FogEnable = false;

float g_time = 0.0f;


//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{

	//ライト初期化
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 100.0f;	// 減衰距離
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ライトのタイプ
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// 並行光源の設定（世界を照らす光）
	//g_Light[0].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );			// 光の向き
	//g_Light[0].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// 光の色
	//g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;						// 並行光源
	//g_Light[0].Enable = true;										// このライトをON
	//SetLight(0, &g_Light[0]);										// これで設定している

	// 並行光源の設定（point1）
	//g_Light[1].Position = XMFLOAT3(-70.0f, 5.0f, 30.0f);		
	//g_Light[1].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[1].Diffuse = XMFLOAT4(0.0f, 0.392f, 0.0f, 1.0f);		// 光の色
	//g_Light[1].Attenuation = 50.0f;								// 減衰距離
	//g_Light[1].Type = LIGHT_TYPE_POINT;							// 並行光源
	//g_Light[1].Enable = true;										// このライトをON
	//SetLight(1, &g_Light[1]);										// これで設定している

	// 並行光源の設定（point2）
	//g_Light[2].Position = XMFLOAT3(10.0f, 2.0f, 0.0f);
	//g_Light[2].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[2].Diffuse = XMFLOAT4(0.0f, 0.749f, 1.0f, 1.0f);		// 光の色
	//g_Light[2].Attenuation = 50.0f;								// 減衰距離
	//g_Light[2].Type = LIGHT_TYPE_POINT;							// 並行光源
	//g_Light[2].Enable = true;										// このライトをON
	//SetLight(2, &g_Light[2]);										// これで設定している

	//// 並行光源の設定（point3）
	//g_Light[3].Position = XMFLOAT3(40.0f, 8.0f, 20.0f);
	//g_Light[3].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[3].Diffuse = XMFLOAT4(0.6f, 0.196f, 0.8f, 1.0f);		// 光の色
	//g_Light[3].Attenuation = 50.0f;								// 減衰距離
	//g_Light[3].Type = LIGHT_TYPE_POINT;							// 並行光源
	//g_Light[3].Enable = true;										// このライトをON
	//SetLight(3, &g_Light[3]);										// これで設定している

	//// 並行光源の設定（point4）
	//g_Light[4].Position = XMFLOAT3(70.0f, 5.0f, -20.0f);
	//g_Light[4].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[4].Diffuse = XMFLOAT4(1.0f, 0.270f, 0.0f, 1.0f);		// 光の色
	//g_Light[4].Attenuation = 50.0f;								// 減衰距離
	//g_Light[4].Type = LIGHT_TYPE_POINT;							// 並行光源
	//g_Light[4].Enable = true;										// このライトをON
	//SetLight(4, &g_Light[4]);										// これで設定している

	//// 並行光源の設定（point5）
	//g_Light[5].Position = XMFLOAT3(100.0f, 12.0f, -40.0f);
	//g_Light[5].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 光の向き
	//g_Light[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);		// 光の色
	//g_Light[5].Attenuation = 50.0f;								// 減衰距離
	//g_Light[5].Type = LIGHT_TYPE_POINT;							// 並行光源
	//g_Light[5].Enable = true;										// このライトをON
	//SetLight(5, &g_Light[5]);										// これで設定している

	// 並行光源の設定（spotlight）
	// 光的亮度由法向量 x 光線向量 = 內積,而內積出來的結果要乘以「-1」是因為要合理化光線照射正面要是最亮而被對最暗的結果
	// 衰減率公式: (R-D)/R
	// 之所以要normalize是為了要讓兩者向量距離為「1」乘以cos後可以算出cos角度
	// acos的功用是為了要將上方的angle數值轉回角度
	PLAYER* player = GetPlayer();
	g_Light[6].Position = XMFLOAT3(player[0].pos.x, player[0].pos.y + 20.0f, player[0].pos.z);
	g_Light[6].Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);				// 光の向き
	g_Light[6].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// 光の色
	g_Light[6].Angle = XMConvertToRadians(60.0f);					// 角度
	g_Light[6].Attenuation = 100.0f;								// 減衰距離
	g_Light[6].Type = LIGHT_TYPE_SPOT;								// 並行光源
	g_Light[6].Enable = true;										// このライトをON
	SetLight(6, &g_Light[6]);										// これで設定している

	// フォグの初期化（霧の効果）
	g_Fog.FogStart = 100.0f;										// 視点からこの距離離れるとフォグがかかり始める
	g_Fog.FogEnd   = 250.0f;										// ここまで離れるとフォグの色で見えなくなる
	g_Fog.FogColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );			// フォグの色
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);										// 他の場所もチェックする shadow

}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{	
	// 世界の光
	g_Light[0].Direction.x = -cosf(g_time);
	g_Light[0].Direction.y = -sinf(g_time);
	SetLight(0, &g_Light[0]);
	g_time += 0.01f;

	//spotlight
	PLAYER* player = GetPlayer();
	g_Light[6].Position = XMFLOAT3(player[0].pos.x, player[0].pos.y + 20.0f, player[0].pos.z);
	SetLight(6, &g_Light[6]);
}


//=============================================================================
// ライトの設定
// Typeによってセットするメンバー変数が変わってくる
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// フォグの設定
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}



