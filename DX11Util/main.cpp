#define     NAME    L"DirectX Util"  //タイトルバーに表示するテキスト
#define     DEFAULTWIDTH 800
#define     DEFAULTHEIGHT 600
#include	"Terminal.h"

//プロトタイプ宣言
LRESULT  CALLBACK   WndProc(HWND, UINT, WPARAM, LPARAM);
int      WINAPI     WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

//Windws イベント用関数
LRESULT  CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	//渡された message から、イベントの種類を解析する
	switch (msg){
		//----終了処理----
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//----デフォルトの処理----
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0L;
}

//Windows Main 関数
int  WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow){
	HWND        hWnd;
	MSG         msg;

	// Set up and register window class
	WNDCLASS wc = { CS_CLASSDC,
		WndProc,                                //イベントcallback関数
		0,
		0,
		hInstance,
		NULL,                                   //アイコン
		LoadCursor(NULL, IDC_ARROW),          //マウスカーソル
		(HBRUSH)GetStockObject(BLACK_BRUSH), //背景色
		NULL,                                   //メニュー
		NAME };                                 //クラス名
	if (RegisterClass(&wc) == 0) return FALSE;

	//ウインドウ生成
	hWnd = CreateWindow(NAME,                  //タイトルバーテキスト
		NAME,                  //クラス名
		WS_OVERLAPPED,   //ウインドウスタイル
		CW_USEDEFAULT,         //ウインドウ左上x座標
		CW_USEDEFAULT,         //ウインドウ左上y座標
		DEFAULTWIDTH,         //ウインドウ幅
		DEFAULTHEIGHT,         //ウインドウ高さ
		NULL,                  //親ウインドウのハンドル
		NULL,
		hInstance,
		NULL);
	if (!hWnd) return FALSE;

	//D3DUtil* d3dUtil = new D3DUtil();
	/***********************************************************************************************/
	/*D3DUtilに表示サイズを与えるにあたって、タイトルバーや境界線の幅を除いた大きさを与えてください*/
	/*さもないと、クライアント領域のマウス座標が数ピクセルずれて悲惨なことになります               */
	/*例えばGetClientRect関数を使えば、上記の幅を除いた、描画できる範囲が帰ってきます			   */
	/*ps.　initD3DUtil内で上記の処理を内包しました                                                 */
	/***********************************************************************************************/
	//RECT clientRect;
	//GetClientRect(hWnd, &clientRect);
	//d3dUtil->initD3DUtil(&hWnd, clientRect.right, clientRect.bottom,true);
	D3DUtil::initD3DUtil(&hWnd, DEFAULTWIDTH, DEFAULTHEIGHT,false);
	/*************これでOK**************************/
	
	ShowCursor(FALSE);	//マウスカーソルを消す

	//Texture tex;
	//Texture t_str;
	//const wchar_t* t;//fps表示用
	//D3DUtil::loadTextrue(L"test.bmp", &tex);
	//FpsController fps(60);
	//D3DUtil::initFont(L"MSP ゴシック", 100);
	POINT pt;	//マウス座標

	

	ShowWindow(hWnd, nCmdShow);     //Window を表示
	UpdateWindow(hWnd);             //表示を初期化
	SetFocus(hWnd);                 //フォーカスを設定

	// メッセージループ
	do
	{
		D3DUtil::clearScreen();
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			switch (msg.message){
			case WM_LBUTTONDOWN:
				break;
			case WM_MOUSEHOVER:
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_KEYUP:
				switch (msg.wParam){
				case VK_DELETE:
					break;
				case VK_UP:
					break;
				case VK_DOWN:
					break;
				case VK_RIGHT:
					break;
				case VK_ESCAPE:
					if (D3DUtil::getScreenMode()){
						D3DUtil::changeScreenMode();
					}
					PostQuitMessage(0);
					break;
				}
			case WM_SIZE:
				if (!D3DUtil::getScreenMode)
					D3DUtil::changeSwapChain();
				break;
			}
			/*if (!fps.updataFps() && !fps.stopFlag){
				//マウスの現在の座標を取得する
				pt.x = msg.pt.x;
				pt.y = msg.pt.y;
				ScreenToClient(hWnd, &pt);
				//D3DUtil::drawGraph(&tex, 0, 0, 1, 1);
				//D3DUtil::drawString(pt.x, -(pt.y - D3DUtil::getWindowY()), 0.5, 0.5, L"Hello DirectX");
				//D3DUtil::drawString(0, 0, 0.5, 0.5, L"ようこそダイレクトXへ！！");
				//D3DUtil::refreshScreen();
			}*/
		}
	} while (msg.message != WM_QUIT);

	D3DUtil::CleanupDirect3D();

	return msg.wParam;
}