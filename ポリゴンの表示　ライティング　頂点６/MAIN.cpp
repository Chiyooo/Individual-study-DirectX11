#include "MAIN.h"
#include <fstream>

//グローバル変数
MAIN* g_pMain=NULL;
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//アプリケーションのエントリー関数 
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE , _In_ LPSTR ,_In_ INT )
{
	g_pMain=new MAIN;
	if(g_pMain != NULL)
	{
		if(SUCCEEDED(g_pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{
			if(SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Loop();
			}
		}
		//アプリ終了
		g_pMain->DestroyD3D();
		delete g_pMain;
	}
	return 0;
}
//
//
//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//
MAIN::MAIN()
{
	ZeroMemory(this,sizeof(MAIN)); 
	m_vLight = D3DXVECTOR3(0, 0, 1);//ライト初期化
	D3DXVec3Normalize(&m_vLight, &m_vLight);//3D ベクトルの正規化されたバージョンを返す
}
//
//
//
MAIN::~MAIN()
{
}
//
//
//ウィンドウ作成
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPCWSTR WindowName)
 {
	 // ウィンドウの定義
	 WNDCLASSEX  wc;//構造体
	 ZeroMemory(&wc,sizeof(wc));//ゼロクリア
	 wc.cbSize = sizeof(wc);//構造体サイズ
	 wc.style = CS_HREDRAW | CS_VREDRAW;              //ウィンドウをメッセージ処理するプロシージャを設定
	 wc.lpfnWndProc = WndProc;                        //ウィンドウのメッセージを処理するコールバック関数へのポインタ
	 wc.hInstance = hInstance;                        //クラスのためのウィンドウプロシージャがあるインスタンスハンドル
	 wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);      //アイコンハンドル
	 wc.hCursor = LoadCursor(NULL, IDC_ARROW);        //マウスカーソルハンドル
	 wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);//背景色
	 wc.lpszClassName = WindowName;                   //ウィンドウクラス名
	 wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);     //16*16のアイコン
	 RegisterClassEx(&wc);//ウィンドウ登録
	 //ウィンドウの作成
	 m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
		 0,0,iWidth,iHeight,0,0,hInstance,0);
	 if(!m_hWnd)
	 {
		 return E_FAIL;
	 }
	 //ウインドウの表示
	 ShowWindow(m_hWnd,SW_SHOW);//表示
	 UpdateWindow(m_hWnd) ;//更新
	 
	 return S_OK;
 }
//
//
//ウィンドウプロシージャー
 LRESULT MAIN::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN://何かのキーが押されたら
		switch((char)wParam)
		{
			case VK_ESCAPE://ESCキーで修了
			DestroyWindow(hWnd);
			break;
		}
		break;
		case WM_DESTROY://ウィンドウ消滅する
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
 }
//
//OSからメッセージを取得
//メッセージループとアプリケーション処理の入り口
 void MAIN::Loop()
 {
	 // メッセージループ
	 MSG msg={0};
	 ZeroMemory(&msg,sizeof(msg));//ゼロクリア
	 while(msg.message!=WM_QUIT)
	 {   //アプリケーションに送られてくるメッセージをメッセージキューから取得する
		 if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		 {
			 TranslateMessage(&msg);
			 DispatchMessage(&msg);//アプリケーションの各ウィンドウプロシージャにメッセージを転送する
		 }
		 else
		 {
			 //アプリケーションの処理はここから飛ぶ。
			 App();
		 }
	 }
	 //アプリケーションの終了
}
//
//
//アプリケーション処理。アプリのメイン関数。
void MAIN::App()
{
	Render();

	/*int a[10];
	int c;
	cin >> c;
	int *p = malloc(c);

	if (p != nullptr) {
		p[2] = 23;
	}

	if (p != nullptr) {
		free(p);
		p = nullptr;
	}

	if (p != nullptr) {
		p[2] = 23;
	}*/

}
//
//
//
HRESULT MAIN::InitD3D()
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );                  //構造体を０クリア
	sd.BufferCount = 1;                             //スワップチェーンのバッファ数
	sd.BufferDesc.Width=WINDOW_WIDTH;               //スワップチェーンのバッファサイズ、幅 640
	sd.BufferDesc.Height=WINDOW_HEIGHT;             //スワップチェーンのバッファサイズ、高さ 480
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;//スワップチェーンのバッファフォーマット 32ビットカラー
	sd.BufferDesc.RefreshRate.Numerator=60;	        //   リフレッシュレート　60fps　分母
	sd.BufferDesc.RefreshRate.Denominator=1;        //   1/60　分子
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT; //バッファをレンダーターゲットとして使用
	sd.OutputWindow=m_hWnd;                         //HWNDハンドル(整数値
	sd.SampleDesc.Count=1;                          //マルチサンプリングのピクセル単位の数
	sd.SampleDesc.Quality=0;                        //マルチサンプリングの品質
	sd.Windowed=TRUE;                               //ウィンドウモード
	
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;//作成されたデバイスの機能レベル
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	
	if( FAILED( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,
		0,&pFeatureLevels,1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel,&m_pDeviceContext )) )//D3D11CreateDeviceAndSwapChainで初期化
	{
		return FALSE;
	}
	//レンダーターゲットビューの作成
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer);    
	m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
	SAFE_RELEASE(pBackBuffer);	
	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;                  //ステンシルビュー構造体
	descDepth.Width = WINDOW_WIDTH;                  //ウィンドウサイズ 幅 640  バックバッファーと同じサイズを
	descDepth.Height = WINDOW_HEIGHT;                //ウィンドウサイズ 高さ 480　指定する
	descDepth.MipLevels = 1;                         //ミップマップ　レベル数
	descDepth.ArraySize = 1;                         //配列サイズ
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;        //フォーマット(深度のみ
	descDepth.SampleDesc.Count = 1;                  //マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;                //マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;           //使用方法 デフォルト
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; //深度ステンシルターゲットとしてバインドさせる
	descDepth.CPUAccessFlags = 0;                   //CPUからアクセスしない
	descDepth.MiscFlags = 0;                        //その他の設定なし

	m_pDevice->CreateTexture2D( &descDepth,         //作成する2Dテクスチャ
		                        NULL,               //NULL
		                        &m_pDepthStencil );//作成したテクスチャを受け取る

	m_pDevice->CreateDepthStencilView( m_pDepthStencil,        //深度/ステンシルビューを作るテクスチャ
		                               NULL,                   //深度/ステンシルビューの設定
		                               &m_pDepthStencilView ); //作成したビューを受け取る

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1,                   //
		                                 &m_pRenderTargetView,//デバイスにバインドするレンダリングターゲットを表すID3D11RenderTargetViewの配列へのポインタ
		                                 m_pDepthStencilView);//デバイスにバインドする深度ステンシルビューを表すID3D11DepthStencilViewへのポインタ

	//ビューポートの設定
	D3D11_VIEWPORT vp;           //ビューポート構造体
	vp.Width = WINDOW_WIDTH;     //ウィンドウサイズ 幅   640
	vp.Height = WINDOW_HEIGHT;   //ウィンドウサイズ 高さ 480
	vp.MinDepth = 0.0f;          //Z軸の幅 最小 0
	vp.MaxDepth = 1.0f;          //Z軸の幅 最大 1
	vp.TopLeftX = 0;             //左上の X 座標
	vp.TopLeftY = 0;             //左上の Y 座標
	m_pDeviceContext->RSSetViewports( 1, &vp );//ビューポートの配列をパイプラインのラスタライザーステージにバインド

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;//構造体
	ZeroMemory(&rdc,sizeof(rdc));//ゼロクリア
	rdc.CullMode=D3D11_CULL_NONE;//指定した方向に向いている三角形が描画されない
	//rdc.CullMode= D3D11_CULL_FRONT;//指定した方向に向いている三角形が描画されない
	rdc.FrontCounterClockwise = true;//三角形が前面または背面に向いているかどうかを指定
	rdc.FillMode=D3D11_FILL_SOLID;//三角形をレンダリングするときに使用する塗りつぶしモードを指定
	ID3D11RasterizerState* pIr=NULL;//

	//↓ラスタライザーステージの動作を指示するラスタライザー状態オブジェクトを作成
	m_pDevice->CreateRasterizerState(&rdc,&pIr);
	m_pDeviceContext->RSSetState(pIr); //パイプラインのラスタライザーステージのラスタライザー状態を設定
	SAFE_RELEASE(pIr);//メモリ解放
	//シェーダー初期化
	if(FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	if(FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()//オブジェクトの破棄
{
	//オブジェクトの同士で大小関係がある場合、親を後に破棄する
	//後で子の廃棄ができなくなるから
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
//
//
//シェーダーを作成　頂点レイアウトを定義
HRESULT MAIN::InitShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader=NULL;
	ID3DBlob *pErrors=NULL;

	//ブロブからバーテックスシェーダー作成
	if(FAILED(D3DX11CompileFromFile(L"Simple.hlsl",NULL,NULL,"VS","vs_5_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		BYTE* errt = (BYTE*)(pErrors->GetBufferPointer());
		MessageBox(0, L"Bhlsl読み込み失敗", L"err", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"バーテックスシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",               //シェーダーのinput-signatureでこの要素に関連付けられているHLSLセマンティクス
		0,                          //要素のセマンティックインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,//要素データのデータ型
		0,                          //入力アセンブラを識別する整数値
		0,                          //オプション
		D3D11_INPUT_PER_VERTEX_DATA,//単一入力スロットの入力データクラスを識別
		0 },                        //バッファ内を1要素進む前に同じインスタンスごとのデータを使用して描画するインスタンスの数

	   { "NORMAL",
	    0,
		DXGI_FORMAT_R32G32B32_FLOAT,
	    0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
	    0},
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);//
	//頂点インプットレイアウトを作成
	if( FAILED( m_pDevice->CreateInputLayout( //入力アセンブラステージの入力バッファデータを記述する入力レイアウトオブジェクトを作成
		layout,                               //入力アセンブラステージの入力データ型の配列
		numElements,                          //入力要素の配列内の入力データ型の数
		pCompiledShader->GetBufferPointer(),  //コンパイルされたシェーダへのポインタ
		pCompiledShader->GetBufferSize(),     //コンパイルされたシェーダのサイズ
		&m_pVertexLayout ) ) )                //作成された入力レイアウトオブジェクトへのポインタ
	{
		return FALSE;
	}
	if (FAILED(D3DX11CompileFromFile(L"Simple.hlsl", NULL, NULL, "GS", "gs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		BYTE* errt = (BYTE*)(pErrors->GetBufferPointer());
		MessageBox(0, L"Ghlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreateGeometryShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pGeometryShader)))
	{
		SAFE_RELEASE(m_pGeometryShader);
		MessageBox(0, L"ジオメトリシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//ブロブからピクセルシェーダー作成
	if(FAILED(D3DX11CompileFromFile(
		L"Simple.hlsl",              //シェーダーコードを含むファイルの名前
		NULL,                        //マクロ定義の配列へのポインタ オプション
		NULL,                        //インクルードファイルを処理するためのインタフェースへのポインタオプション
		"PS",                        //シェーダの実行が開始されるシェーダエントリポイント関数の名前
		"ps_5_0",                    //シェーダーモデルを指定する文字列
		0,                           //シェーダコンパイルフラグ
		0,                           //効果はフラグをコンパイル
		NULL,                        //スレッドポンプインタフェースへのポインタ この関数が完了するまで戻らないように指定するには、NULLを使用
		&pCompiledShader,            //コンパイルされたシェーダと、埋め込まれたデバッグおよびシンボルテーブル情報を含むメモリへのポインタ
		&pErrors,                    //コンパイル中に発生したエラーと警告のリストを含むメモリへのポインタ
		NULL)))                      //戻り値へのポインタ
	{
		MessageBox(0,L"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreatePixelShader(//ピクセルシェーダを作成
		pCompiledShader->GetBufferPointer(),//コンパイルされたシェーダへのポインタ
		pCompiledShader->GetBufferSize(),   //コンパイルされたピクセルシェーダのサイズ
		NULL,                               //クラスリンケージインタフェースへのポインタ
		&m_pPixelShader)))                  //ID3D11PixelShaderインターフェースへのポインターのアドレス
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,L"ピクセルシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);		

	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;                                //バッファリソースを記述 構造体
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;            //バッファをパイプラインにバインドする方法を特定
	cb.ByteWidth= sizeof( SIMPLESHADER_CONSTANT_BUFFER );//バッファーのサイズ
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;            //CPUアクセスフラグ D3D11_CPU_ACCESS_FLAGを参照
	cb.MiscFlags	=0;                                  //その他のフラグ　または未使用の場合は0
	cb.StructureByteStride=0;                            //バッファが構造化バッファ構造体内の各要素のサイズ
	cb.Usage=D3D11_USAGE_DYNAMIC;                        //バッファーの読み書き方法を識別

	if( FAILED(m_pDevice->CreateBuffer(//バッファ（頂点バッファ、インデックスバッファ、またはシェーダ定数バッファ）を作成
		&cb,                           //バッファを記述するD3D11_BUFFER_DESC構造体へのポインタ
		NULL,                          //作成されたバッファオブジェクトのID3D11Bufferインターフェイスへのポインタのアドレス
		&m_pConstantBuffer)))          //バッファを作成するのに十分なメモリがない場合、このメソッドはE_OUTOFMEMORYを返す
	{
		return E_FAIL;
	}
	return S_OK;
}
//
//
//ポリゴンの初期化
HRESULT MAIN::InitPolygon()
{
	//SimpleVertex  a;
	//void* po = (void*)(&a.Pos);//0x00aff554
	//void* nm = (void*)(&a.Normal);//0x00aff560
	//バーテックスバッファー作成
	SimpleVertex vertices[6];
	{
		std::ifstream fin("./data/polygon.txt");//読み込み
		if (!fin) {
			return E_FAIL; }

		for (int i = 0; i < 6; ++i) {//頂点読み込み
			fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
			vertices[i].Normal.x = 0;
			vertices[i].Normal.y = 0;
			vertices[i].Normal.z = -1;

		}
		fin.close();
	}


	D3D11_BUFFER_DESC bd;//バッファリソース
	bd.Usage = D3D11_USAGE_DEFAULT;//バッファーの読み取りおよび書き込みがどのようにされるかを識別
	bd.ByteWidth = sizeof(SimpleVertex)*6;//バッファのサイズ 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//バッファーがパイプラインにバインドされる方法を識
	bd.CPUAccessFlags = 0;//Cpu アクセスフラグ
	bd.MiscFlags = 0;//その他のフラグ

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;//初期化データへのポインタ
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ) ) )
	{
		return E_FAIL;
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	return S_OK;
}
//
//
//シーンを画面にレンダリング、ポリゴン描画処理
void MAIN::Render()
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = {0.6f,0.2f, 0.2f,1};// クリア色作成　RGBAの順　青
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);//深度バッファクリア

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	//ワールドトランスフォーム（絶対座標変換）
	static float  ry = 0.0f;//Y
	//static float  rx = 0.0f;//X
	//static float rz = 0.0f;//Z

	ry += 0.0001f;//Y
	//rx += 0.0001f;//X
	//rz += 0.0001f;//Z

	D3DXMatrixRotationY(&mWorld, ry);//単純にyaw回転させる
	//D3DXMatrixRotationY(&mWorld,timeGetTime()/2000.0f );//単純にyaw回転させる
	//D3DXMatrixRotationX(&mWorld, rx);//X軸回転
	//D3DXMatrixRotationZ(&mWorld, -rz);//Z軸回転

	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt( 0.0f, 1.0f,-2.0f ); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );//注視位置
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//上方位置
	D3DXMatrixLookAtLH( &mView, &vEyePt, &vLookatPt, &vUpVec );
	// プロジェクショントランスフォーム（射影変換）
	D3DXMatrixPerspectiveFovLH( &mProj, D3DX_PI/4, (FLOAT)WINDOW_WIDTH/(FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f );	

	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->GSSetShader(m_pGeometryShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if( SUCCEEDED( m_pDeviceContext->Map( 
		m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ワールド行列
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m=mWorld*mView*mProj;
		cb.mWVP=m;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

		//カラーを渡す
		D3DXVECTOR4 vColor(0,0,1,1);//ポリゴンの色設定
		cb.vColor=vColor;

		//ライト方向
		cb.vLightDir = (D3DXVECTOR4)m_vLight;
//		cb.vLightDir = D3DXVECTOR4(m_vLight.x, m_vLight.y, m_vLight.z, 0);
	
		auto s = sizeof(cb);
		memcpy_s( pData.pData, pData.RowPitch, (void*)( &cb), pData.RowPitch);
		m_pDeviceContext->Unmap( m_pConstantBuffer, 0 );
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer );
	m_pDeviceContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );//TRIANGLELIST
	//プリミティブをレンダリング
	m_pDeviceContext->Draw(6,0);

	m_pSwapChain->Present(0,0);//画面更新（バックバッファをフロントバッファに）	
}