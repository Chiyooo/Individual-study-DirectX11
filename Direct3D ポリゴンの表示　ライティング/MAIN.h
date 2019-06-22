#pragma once
//ヘッダーファイルのインクルード
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
//警告非表示
// doubleからfloatに値を絞り込むことに関する警告。
#pragma warning(disable : 4305)
//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME L"ポリゴンの表示 D3d11"//タイトルバーの名前
//マクロ コードの重複を除去する
#define SAFE_RELEASE(x) if(x){x->Release(); x;}

//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置 Vec型 X,Y,Z
	D3DXVECTOR3 Normal;//法線　シェーディング
};
//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 
//もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mW;      //ワールド変換
	D3DXMATRIX mWVP;    //変換行列
	D3DXVECTOR4 vLightDir;//ライト方向
	D3DXVECTOR4 vColor; //ディフューズ色

};
//
//
//クラス定義
class MAIN
{
public:
	MAIN();//コンストラクタ
	~MAIN();//デストラクタ
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);//ウィンドウの作成
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);//ウィンドウプロシージャー
	HRESULT InitD3D();//初期化
	HRESULT InitPolygon();//ポリゴンの初期化
	HRESULT InitShader();//シェーダーの初期化(バーテックスシェーダーとピクセルシェーダーの初期化)
	void Loop();          //メッセージループ
	void App();           //アプリケーション処理
	void Render();    //ポリゴン描画処理
	void DestroyD3D();//オブジェクトの破棄
	//↓アプリにひとつ
	HWND m_hWnd;//ウィンドウのハンドル
	ID3D11Device* m_pDevice;                    //デバイスインターフェース
	ID3D11DeviceContext* m_pDeviceContext;      //レンダリングコマンドを生成するデバイスコンテキスト
	IDXGISwapChain* m_pSwapChain;               //レンダリングされたデータを出力に提示する前に格納するための1つ以上のサーフェスを実装
	ID3D11RenderTargetView* m_pRenderTargetView;//レンダリング中にアクセスできるレンダーターゲットサブリソースを識別
	ID3D11DepthStencilView* m_pDepthStencilView;//深度ステンシルテスト中にテクスチャリソースにアクセス
	ID3D11Texture2D* m_pDepthStencil;           //テクセルデータを管理,テクセルデータは構造化メモリ
	D3DXVECTOR3 m_vLight;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	//↓ポリゴン描画に必要な物
	//下の４つは同じタイプのポリゴンで
	//ある限り１つで足りる
	ID3D11InputLayout* m_pVertexLayout;  //頂点レイアウトのポインタ、座標のみのレイアウトの初期化
	ID3D11VertexShader* m_pVertexShader; //バーテックスシェーダーのポインタ
	ID3D11GeometryShader* m_pGeometryShader; //シェーダーのポインタ
	ID3D11PixelShader* m_pPixelShader;   //ピクセルシェーダーのポインタ
	ID3D11Buffer* m_pConstantBuffer;     //コンスタンスとバッファー
	//↓モデルごとに必要	
	ID3D11Buffer* m_pVertexBuffer;       //バーテックスバッファー(頂点バッファー),頂点を保存しておくメモリー領域
};
