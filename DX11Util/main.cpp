#define     NAME    L"DirectX Util"  //�^�C�g���o�[�ɕ\������e�L�X�g
#define     DEFAULTWIDTH 800
#define     DEFAULTHEIGHT 600
#include	"Terminal.h"

//�v���g�^�C�v�錾
LRESULT  CALLBACK   WndProc(HWND, UINT, WPARAM, LPARAM);
int      WINAPI     WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

//Windws �C�x���g�p�֐�
LRESULT  CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	//�n���ꂽ message ����A�C�x���g�̎�ނ���͂���
	switch (msg){
		//----�I������----
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//----�f�t�H���g�̏���----
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0L;
}

//Windows Main �֐�
int  WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow){
	HWND        hWnd;
	MSG         msg;

	// Set up and register window class
	WNDCLASS wc = { CS_CLASSDC,
		WndProc,                                //�C�x���gcallback�֐�
		0,
		0,
		hInstance,
		NULL,                                   //�A�C�R��
		LoadCursor(NULL, IDC_ARROW),          //�}�E�X�J�[�\��
		(HBRUSH)GetStockObject(BLACK_BRUSH), //�w�i�F
		NULL,                                   //���j���[
		NAME };                                 //�N���X��
	if (RegisterClass(&wc) == 0) return FALSE;

	//�E�C���h�E����
	hWnd = CreateWindow(NAME,                  //�^�C�g���o�[�e�L�X�g
		NAME,                  //�N���X��
		WS_OVERLAPPED,   //�E�C���h�E�X�^�C��
		CW_USEDEFAULT,         //�E�C���h�E����x���W
		CW_USEDEFAULT,         //�E�C���h�E����y���W
		DEFAULTWIDTH,         //�E�C���h�E��
		DEFAULTHEIGHT,         //�E�C���h�E����
		NULL,                  //�e�E�C���h�E�̃n���h��
		NULL,
		hInstance,
		NULL);
	if (!hWnd) return FALSE;

	//D3DUtil* d3dUtil = new D3DUtil();
	/***********************************************************************************************/
	/*D3DUtil�ɕ\���T�C�Y��^����ɂ������āA�^�C�g���o�[�⋫�E���̕����������傫����^���Ă�������*/
	/*�����Ȃ��ƁA�N���C�A���g�̈�̃}�E�X���W�����s�N�Z������ĔߎS�Ȃ��ƂɂȂ�܂�               */
	/*�Ⴆ��GetClientRect�֐����g���΁A��L�̕����������A�`��ł���͈͂��A���Ă��܂�			   */
	/*ps.�@initD3DUtil���ŏ�L�̏��������܂���                                                 */
	/***********************************************************************************************/
	//RECT clientRect;
	//GetClientRect(hWnd, &clientRect);
	//d3dUtil->initD3DUtil(&hWnd, clientRect.right, clientRect.bottom,true);
	D3DUtil::initD3DUtil(&hWnd, DEFAULTWIDTH, DEFAULTHEIGHT,false);
	/*************�����OK**************************/
	
	ShowCursor(FALSE);	//�}�E�X�J�[�\��������

	//Texture tex;
	//Texture t_str;
	//const wchar_t* t;//fps�\���p
	//D3DUtil::loadTextrue(L"test.bmp", &tex);
	//FpsController fps(60);
	//D3DUtil::initFont(L"MSP �S�V�b�N", 100);
	POINT pt;	//�}�E�X���W

	

	ShowWindow(hWnd, nCmdShow);     //Window ��\��
	UpdateWindow(hWnd);             //�\����������
	SetFocus(hWnd);                 //�t�H�[�J�X��ݒ�

	// ���b�Z�[�W���[�v
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
				//�}�E�X�̌��݂̍��W���擾����
				pt.x = msg.pt.x;
				pt.y = msg.pt.y;
				ScreenToClient(hWnd, &pt);
				//D3DUtil::drawGraph(&tex, 0, 0, 1, 1);
				//D3DUtil::drawString(pt.x, -(pt.y - D3DUtil::getWindowY()), 0.5, 0.5, L"Hello DirectX");
				//D3DUtil::drawString(0, 0, 0.5, 0.5, L"�悤�����_�C���N�gX�ցI�I");
				//D3DUtil::refreshScreen();
			}*/
		}
	} while (msg.message != WM_QUIT);

	D3DUtil::CleanupDirect3D();

	return msg.wParam;
}