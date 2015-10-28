#include "D3DUtil.h"
#include "Texture.h"


D3DUtil::D3DUtil()
{
}


D3DUtil::~D3DUtil()
{
	CleanupDirect3D();
}

bool D3DUtil::initD3DUtil(HWND* targetWindow,float l_windowWidth,float l_windowHeight,bool fullScreen){
	if (XMVerifyCPUSupport() != TRUE)
	{
		DebugBreak();
		return false;
	}
	window = targetWindow;
	windowMode = fullScreen;

	g_pSwapChain = NULL;
	g_pVertexShader = NULL;
	g_pGeometryShader = NULL;
	g_pAnimateGeometryShader = NULL;
	g_pPixelShader = NULL;
	pBlobVS = NULL;
	pBlobGS = NULL;
	pBlobPS = NULL;
	g_pRasterizerState = NULL;
	g_pBlendState = NULL;
	g_pDepthStencilState = NULL;
	g_pInputLayout = NULL;
	g_pIdxBuffer = NULL;
	g_pSampler = NULL;

	if (!windowMode){
		RECT clientRect;
		GetClientRect(*window, &clientRect);
		windowWidth = clientRect.right;
		windowHeight = clientRect.bottom;
	}
	else{
		windowWidth = l_windowWidth;
		windowHeight = l_windowHeight;
	}

	if (!initDeviceAndSwapChain()){
		MessageBox(*targetWindow, L"filed to initialize the device and swapchain", NULL, 0);
		DebugBreak();
	}
	else if (!initBackBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the backbuffer", NULL, 0);
		DebugBreak();
	}
	else if (!initViewPort()){
		MessageBox(*targetWindow, L"filed to initialize the viewport", NULL, 0);
		DebugBreak();
	}
	else if (!initDepthAndStencil()){
		MessageBox(*targetWindow, L"filed to initialize the depth and stencil buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initVertexBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the vertex buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initShaderAndConstantBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the shader and constant buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initInputAssembler()){
		MessageBox(*targetWindow, L"filed to initialize the input assembler", NULL, 0);
		DebugBreak();
	}
	else if (!initRasterizer()){
		MessageBox(*targetWindow, L"filed to initialize the rasterizer", NULL, 0);
		DebugBreak();
	}
	else if (!initOutputMerger()){
		MessageBox(*targetWindow, L"filed to initialize the input merger", NULL, 0);
		DebugBreak();
	}
	else if (!initConstantBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the constant buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initSampler()){
		MessageBox(*targetWindow, L"filed to initialize sampler", NULL, 0);
		DebugBreak();
	}
	return true;
}

bool D3DUtil::initFont(LPCWSTR fontName,UINT fontSize){
	return FontLoder::initFont(NULL,fontName, fontSize);
}

bool D3DUtil::initDeviceAndSwapChain(){

	/****************************************/
	/*			�X���b�v�`�F�C���̐ݒ�		*/
	/****************************************/
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;			//�o�b�N�o�b�t�@��
	sd.BufferDesc.Width = windowWidth;	//�o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = windowHeight;	//�o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator = 60;	//���t���b�V�����[�g�@���q
	sd.BufferDesc.RefreshRate.Denominator = 1;	//���t���b�V�����[�g�@����
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g�p�@
	sd.OutputWindow = *window;	//�֘A�t����E�B���h�E
	sd.SampleDesc.Count = 1;	//�}���`�T���v�����O��
	sd.SampleDesc.Quality = 0;	//�}���`�T���v�����O�̃N�I���e�B
	sd.Windowed = !windowMode;			//�E�B���h�E���[�h�ɂ��邩�ǂ���
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//���[�h�����؂�ւ�
	
	/*****************************************/
	/*			  �@�\���x���̐ݒ�			 */
	/*****************************************/

	D3D_FEATURE_LEVEL	g_pFeatureLevels[] = {	//�@�\���x���z��
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	DWORD				g_FeatureLevels = 3;	//�z��̗v�f��
	D3D_FEATURE_LEVEL g_FeatureLevelsSupported;	//�f�o�C�X�쐬���ɕԂ����@�\���x��

	/*******************************************/
	/*			�f�o�b�N�p�f�o�C�X�̍쐬	   */
	/*******************************************/
#if defined(DEBUG) || defined(_DEBUG)
	DWORD createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;	//�f�o�b�O���ɂ̓f�o�b�N�p�f�o�C�X��ݒ�
#else
	DWORD createDeviceFlags = 0;
#endif

	/*******************************************/
	/*	�f�o�C�X�ƃX���b�v�`�F�C���̓����쐬   */
	/*******************************************/

	//�n�[�h�E�F�A�f�o�C�X���쐬
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		g_pFeatureLevels,
		g_FeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pD3DDevice,
		&g_FeatureLevelsSupported,
		&g_pImmediateContext);
	if (FAILED(hr)){
		//�n�[�h�E�F�A���_���Ȃ�WARP�f�o�C�X���쐬
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_WARP,
			NULL,
			createDeviceFlags,
			g_pFeatureLevels,
			g_FeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&g_pSwapChain,
			&g_pD3DDevice,
			&g_FeatureLevelsSupported,
			&g_pImmediateContext);
		if (FAILED(hr)){
			//WARP�f�o�C�X���_���Ȃ烊�t�@�����X�f�o�C�X�̍쐬
			hr = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_REFERENCE,
				NULL,
				createDeviceFlags,
				g_pFeatureLevels,
				g_FeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&g_pSwapChain,
				&g_pD3DDevice,
				&g_FeatureLevelsSupported,
				&g_pImmediateContext);
			if ((FAILED(hr))){
				return false;
			}
		}
	}
	return true;
}

bool D3DUtil::initBackBuffer(){
	/****************************************/
	/*        �`��^�[�Q�b�g�r���[�̐ݒ�    */
	/****************************************/

	//�X���b�v�`�F�C������ŏ��̃o�b�N�o�b�t�@���擾����
	ID3D11Texture2D* pBackBuffer;	//�o�b�t�@�̃A�N�Z�X�Ɏg���C���^�[�t�F�[�X
	HRESULT hr = g_pSwapChain->GetBuffer(	//�o�b�t�@��pBackBuffer�Ŏ󂯎��
		0,									//�o�b�N�E�o�b�t�@�̔ԍ�
		__uuidof(ID3D11Texture2D),			//�o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�[�X
		(LPVOID*)&pBackBuffer);				//�o�b�t�@���󂯎��ϐ�
	if (FAILED(hr)){
		return false;
	}

	//�o�b�N�o�b�t�@�̕`��^�[�Q�b�g�r���[�����
	hr = g_pD3DDevice->CreateRenderTargetView(
		pBackBuffer,				//�r���[�ŃA�N�Z�X���郊�\�[�X
		NULL,						//�`��^�[�Q�b�g�r���[�̒�`(NULL�̓f�t�H���g�ݒ�g�p)
		&g_pRenderTargetView);		//�`��^�[�Q�b�g�r���[���󂯎��ϐ�

	SAFE_RELEASE(pBackBuffer);		//�ݒ肪�I������̂ŁA���ڈ���Ȃ��o�b�N�o�b�t�@�̓��ꕨ�͉������
	if (FAILED(hr)){
		return false;
	}

	//�`��^�[�Q�b�g�r���[���o�̓}�[�W���[�̕`��^�[�Q�b�g�Ƃ��Đݒ�
	g_pImmediateContext->OMSetRenderTargets(
		1,						//�`��^�[�Q�b�g��
		&g_pRenderTargetView,	//�`��^�[�Q�b�g�r���[�z��
		NULL);					//�[�x/�X�e���V���r���[�̐ݒ�(NULL=�g�p���Ȃ�)

	return true;
}

bool D3DUtil::initViewPort(){
	/*****************************************/
	/*          �r���[�|�[�g�̐ݒ�           */
	/*****************************************/

	D3D11_VIEWPORT g_ViewPort[1];	//�r���[�|�[�g�̐ݒ�
	g_ViewPort[0].TopLeftX	= 0.0f;			//�r���[�|�[�g�̈�̍���X���W
	g_ViewPort[0].TopLeftY	= 0.0f;			//�r���[�|�[�g�̈�̍���Y���W
	g_ViewPort[0].Width = windowWidth;		//�r���[�|�[�g�̈�̕�
	g_ViewPort[0].Height = windowHeight;	//�r���[�|�[�g�̈�̍���
	g_ViewPort[0].MinDepth = 0.0f;			//�r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	g_ViewPort[0].MaxDepth = 1.0f;			//�r���[�|�[�g�̈�̐[�x�l�̍ő�l

	//���X�^���C�U�Ƀr���[�|�[�g��ݒ�
	g_pImmediateContext->RSSetViewports(1, g_ViewPort);

	return true;
}

bool D3DUtil::initDepthAndStencil(){
	/*************************************/
	/*  �[�x/�X�e���V���e�N�X�`���̍쐬  */
	/*************************************/

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = windowWidth;
	descDepth.Height = windowHeight;
	descDepth.MipLevels = 1;	//�~�b�v�}�b�v�̃��x����
	descDepth.ArraySize = 1;	//�z��T�C�Y
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;	//�t�H�[�}�b�g�i�[�x�̂݁j
	descDepth.SampleDesc.Count = 1;	//�}���`�T���v�����O�̐ݒ�
	descDepth.SampleDesc.Quality = 0;	//�}���`�T���v�����O�̕i��
	descDepth.Usage = D3D11_USAGE_DEFAULT;	//�f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//�[�x/�X�e���V���Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;	//CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags = 0;		//���̑��̐ݒ�Ȃ�

	HRESULT hr = g_pD3DDevice->CreateTexture2D(
		&descDepth,			//�쐬����2D�e�N�X�`���̐ݒ�
		NULL,				//
		&g_pDepthStencil);	//�쐬�����e�N�X�`�����󂯎��ϐ�

	if (FAILED(hr)){
		return false;
	}

	/************************************/
	/*   �[�x/�X�e���V���r���[�̍쐬    */
	/************************************/

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;	//�r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	
	hr = g_pD3DDevice->CreateDepthStencilView(
		g_pDepthStencil,		//�[�x/�X�e���V���r���[�����e�N�X�`��
		&descDSV,				//�[�x/�X�e���V���r���[�̐ݒ�
		&g_pDepthStencilView);	//�쐬�����r���[���󂯎��ϐ�
	if (FAILED(hr)){
		return false;
	}

	//�[�x/�X�e���V���r���[���o�̓}�[�W���[�ɐݒ�
	g_pImmediateContext->OMSetRenderTargets(
		1,						//�ݒ肷��`��^�[�Q�b�g�r���[�̐�
		&g_pRenderTargetView,	//�ݒ肷��`��^�[�Q�b�g�r���[�̔z��
		g_pDepthStencilView);	//�ݒ肷��[�x/�X�e���V���r���[

	return true;
}

void D3DUtil::initDispray(){
	//�`��^�[�Q�b�g�̃N���A
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };//�N���A����l(RGBA)
	g_pImmediateContext->ClearRenderTargetView(
		g_pRenderTargetView,	//�N���A����`��^�[�Q�b�g
		ClearColor);			//�N���A����l

	//�[�x�X�e���V���̃N���A
	g_pImmediateContext->ClearDepthStencilView(
		g_pDepthStencilView,	//�N���A����[�x�X�e���V���r���[
		D3D11_CLEAR_DEPTH,		//�[�x�l�������N���A����
		1.0f,					//�[�x�o�b�t�@���N���A����l
		0);						//�X�e���V���o�b�t�@���N���A����l(���̏ꍇ�͖��֌W)

	//�o�b�N�o�b�t�@�̕\��
	HRESULT hr = g_pSwapChain->Present(
		0,	//��ʂ������ɍX�V����
		0);	//��ʂ����ۂɍX�V����
}

bool D3DUtil::isDeviceDisappearance(){
	//�f�o�C�X�̏�������
	HRESULT hr = g_pD3DDevice->GetDeviceRemovedReason();
	switch (hr){
	case S_OK:
		break;			//����
	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:
		CleanupDirect3D();
		if (!InitDirect3D()){
			return false;		//�������s�B�A�v���P�[�V�����I��
		}
		break;
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case DXGI_ERROR_INVALID_CALL:
	default:
		return false;			//�ǂ����悤���̖����̂ŃA�v���P�[�V�����I��
	};
}

bool D3DUtil::InitDirect3D(){
	return initD3DUtil(window, windowWidth, windowHeight,windowMode);
}

void D3DUtil::CleanupDirect3D(){
	//�f�o�C�X�X�e�[�g�̃N���A
	if (g_pImmediateContext){
		g_pImmediateContext->ClearState();
	}

	//�t���X�N���[�����p�̏I������
	if (g_pSwapChain && windowMode){
		g_pSwapChain->SetFullscreenState(FALSE, NULL);
	}

	//�擾�����C���^�[�t�F�[�X�̉��
	SAFE_RELEASE(pBlobVS);
	SAFE_RELEASE(pBlobGS);
	SAFE_RELEASE(pBlobPS);

	SAFE_RELEASE(g_pVertexShader);
	SAFE_RELEASE(g_pGeometryShader);
	SAFE_RELEASE(g_pAnimateGeometryShader);
	SAFE_RELEASE(g_pPixelShader);

	SAFE_RELEASE(g_pCBuffer[0]);
	SAFE_RELEASE(g_pCBuffer[1]);
	SAFE_RELEASE(g_pCBuffer[2]);
	
	SAFE_RELEASE(g_pRasterizerState);
	SAFE_RELEASE(g_pBlendState);
	SAFE_RELEASE(g_pDepthStencilState);
	SAFE_RELEASE(g_pInputLayout);

	SAFE_RELEASE(g_pVerBuffer[0]);
	SAFE_RELEASE(g_pVerBuffer[1]);
	SAFE_RELEASE(g_pIdxBuffer);

	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pSampler);

	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pImmediateContext);
	SAFE_RELEASE(g_pD3DDevice);
}

bool D3DUtil::initShaderAndConstantBuffer(){
	/*****************************************/
	/*         �V�F�[�_�\�̏�����            */
	/*****************************************/

	//���_�V�F�[�_�̏�����
	HRESULT hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//�t�@�C����
		NULL,									//�}�N����`
		NULL,									//�C���N���[�h�t�@�C����`
		"VS",									//�ǂ̃V�F�[�_�\���iVS�͒��_�V�F�[�_�p�ƌ������Ɓj
		"vs_4_0",								//�V�F�[�_�\���f��
		D3D10_SHADER_ENABLE_STRICTNESS |		//�R���p�C���I�v�V����
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//�G�t�F�N�g�̃R���p�C���I�v�V����
		NULL,									//�����ɃR���p�C�����Ă���֐��𔲂���
		&pBlobVS,								//�o�C�g�R�[�h�̊i�[��
		NULL,									//�G���[���b�Z�[�W�͕s�v
		NULL);									//�߂�l
	if (FAILED(hr)){
		return false;
	}

	//�W�I���g���V�F�[�_�\�̏�����
	hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//�t�@�C����
		NULL,									//�}�N����`
		NULL,									//�C���N���[�h�t�@�C����`
		"GS",									//�ǂ̃V�F�[�_�\���iGS�̓W�I���g���V�F�[�_�p�ƌ������Ɓj
		"gs_4_0",								//�V�F�[�_�\���f��
		D3D10_SHADER_ENABLE_STRICTNESS |		//�R���p�C���I�v�V����
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//�G�t�F�N�g�̃R���p�C���I�v�V����
		NULL,									//�����ɃR���p�C�����Ă���֐��𔲂���
		&pBlobGS,								//�o�C�g�R�[�h�̊i�[��
		NULL,									//�G���[���b�Z�[�W�͕s�v
		NULL);									//�߂�l
	if (FAILED(hr)){
		return false;
	}

	//�W�I���g���V�F�[�_�I�u�W�F�N�g�̐���
	hr = g_pD3DDevice->CreateGeometryShader(
		pBlobGS->GetBufferPointer(),	//�o�C�g�R�[�h�ւ̃|�C���^
		pBlobGS->GetBufferSize(),		//�o�C�g�R�[�h�̒���
		NULL,
		&g_pGeometryShader);				//�V�F�[�_���󂯎��ϐ�
	if (FAILED(hr)){
		return false;
	}

	//�����p�W�I���g���V�F�[�_�̏�����
	hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//�t�@�C����
		NULL,									//�}�N����`
		NULL,									//�C���N���[�h�t�@�C����`
		"GS_STRING",							//�ǂ̃V�F�[�_�\���iGS�̓W�I���g���V�F�[�_�p�ƌ������Ɓj
		"gs_4_0",								//�V�F�[�_�\���f��
		D3D10_SHADER_ENABLE_STRICTNESS |		//�R���p�C���I�v�V����
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//�G�t�F�N�g�̃R���p�C���I�v�V����
		NULL,									//�����ɃR���p�C�����Ă���֐��𔲂���
		&pBlobGS,								//�o�C�g�R�[�h�̊i�[��
		NULL,									//�G���[���b�Z�[�W�͕s�v
		NULL);									//�߂�l
	if (FAILED(hr)){
		return false;
	}

	//�W�I���g���V�F�[�_�I�u�W�F�N�g�̐���
	hr = g_pD3DDevice->CreateGeometryShader(
		pBlobGS->GetBufferPointer(),	//�o�C�g�R�[�h�ւ̃|�C���^
		pBlobGS->GetBufferSize(),		//�o�C�g�R�[�h�̒���
		NULL,
		&g_pAnimateGeometryShader);				//�V�F�[�_���󂯎��ϐ�
	if (FAILED(hr)){
		return false;
	}

	//�s�N�Z���V�F�[�_�̏�����
	hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//�t�@�C����
		NULL,									//�}�N����`
		NULL,									//�C���N���[�h�t�@�C����`
		"PS_Sample",									//�ǂ̃V�F�[�_�\���iGS�̓W�I���g���V�F�[�_�p�ƌ������Ɓj
		"ps_4_0",								//�V�F�[�_�\���f��
		D3D10_SHADER_ENABLE_STRICTNESS |		//�R���p�C���I�v�V����
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//�G�t�F�N�g�̃R���p�C���I�v�V����
		NULL,									//�����ɃR���p�C�����Ă���֐��𔲂���
		&pBlobPS,								//�o�C�g�R�[�h�̊i�[��
		NULL,									//�G���[���b�Z�[�W�͕s�v
		NULL);									//�߂�l
	if (FAILED(hr)){
		return false;
	}

	//���_�V�F�[�_�I�u�W�F�N�g�̐���
	hr = g_pD3DDevice->CreateVertexShader(
		pBlobVS->GetBufferPointer(),	//�o�C�g�R�[�h�ւ̃|�C���^
		pBlobVS->GetBufferSize(),		//�o�C�g�R�[�h�̒���
		NULL,
		&g_pVertexShader);				//�V�F�[�_���󂯎��ϐ�
	if (FAILED(hr)){
		return false;
	}

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g�̐���
	hr = g_pD3DDevice->CreatePixelShader(
		pBlobPS->GetBufferPointer(),	//�o�C�g�R�[�h�ւ̃|�C���^
		pBlobPS->GetBufferSize(),		//�o�C�g�R�[�h�̒���
		NULL,
		&g_pPixelShader);				//�V�F�[�_���󂯎��ϐ�
	if (FAILED(hr)){
		return false;
	}

	//�V�F�[�_�I�u�W�F�N�g�̐ݒ�
	//���_�V�F�[�_
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);

	//�W�I���g���V�F�[�_
	g_pImmediateContext->GSSetShader(g_pGeometryShader, NULL, 0);

	//�s�N�Z���V�F�[�_
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	/*****************************************/
	/*         �萔�o�b�t�@�̍쐬            */
	/*****************************************/

	//�萔�o�b�t�@�̒�`
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				//���I�g�p�@
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//�萔�o�b�t�@
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU���珑������
	cBufferDesc.MiscFlags = 0;								
	
	//�萔�o�b�t�@�i�ύX���Ȃ��f�[�^�j�̍쐬
	cBufferDesc.ByteWidth = sizeof(cbNeverChanged);	//�o�b�t�@�T�C�Y
	hr = g_pD3DDevice->CreateBuffer(
		&cBufferDesc,
		NULL,
		&g_pCBuffer[0]);
	if (FAILED(hr)){
		return false;
	}

	//�萔�o�b�t�@�i�ύX�p�x�̒Ⴂ�f�[�^�j�̍쐬
	cBufferDesc.ByteWidth = sizeof(cbChangesEveryFrame);	//�o�b�t�@�T�C�Y
	hr = g_pD3DDevice->CreateBuffer(
		&cBufferDesc,
		NULL,
		&g_pCBuffer[1]);
	if (FAILED(hr)){
		return false;
	}

	//�萔�o�b�t�@�i�ύX�p�x�̍����f�[�^�j�̍쐬
	cBufferDesc.ByteWidth = sizeof(cbChangesEveryObject);	//�o�b�t�@�T�C�Y
	hr = g_pD3DDevice->CreateBuffer(
		&cBufferDesc,
		NULL,
		&g_pCBuffer[2]);
	if (FAILED(hr)){
		return false;
	}

	//���_�V�F�[�_�ɒ萔�o�b�t�@��ݒ�
	g_pImmediateContext->VSSetConstantBuffers(0, 3, g_pCBuffer);
	//�W�I���g���V�F�[�_�ɒ萔�o�b�t�@��ݒ�
	g_pImmediateContext->GSSetConstantBuffers(0, 3, g_pCBuffer);
	//�s�N�Z���V�F�[�_�ɒ萔�o�b�t�@��ݒ�
	g_pImmediateContext->PSSetConstantBuffers(0, 3, g_pCBuffer);
	
	return true;
}

bool D3DUtil::initConstantBuffer(){
	//�萔�o�b�t�@�i�ύX���Ȃ��j
	//�ˉe�ϊ��s��i�p�[�X�y�N�e�B�u�i�����@�j�ˉe�j
	/*XMMATRIX mat = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(30.0f),	//����p30��
		windowWidth / windowHeight,	//�A�X�y�N�g��
		1.0f,						//�O�����e�ʂ܂ł̋���
		100.0f						//������e�ʂ܂ł̋���
	);*/
	XMMATRIX mat = XMMatrixOrthographicLH(
		windowWidth,
		windowHeight,
		1.0f,
		20.f
		);
	mat = XMMatrixTranspose(mat);
	XMStoreFloat4x4(&g_cbNeverChanges.Projection, mat);

	//�萔�o�b�t�@�i�ύX���Ȃ��j�̃}�b�v�擾
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[0],						//�}�b�v���郊�\�[�X
		0,									//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,			//�������݃A�N�Z�X
		0,
		&MappedResource);					//�f�[�^�̏������ݐ�|�C���^
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbNeverChanges, sizeof(cbNeverChanged));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[0], 0);

	//�萔�o�b�t�@(�ύX�p�x�̒Ⴂ)
	//�r���[�ϊ��s��
	XMVECTORF32 eyePosition = { windowWidth/2.0f, windowHeight/2.0f, -10.0f, 1.0f };	//���_�i�J�����̈ʒu�j
	XMVECTORF32 focuPosition = { windowWidth / 2.0f, windowHeight / 2.0f, 0.0f, 1.0f };	//�����_
	XMVECTORF32 upDirection = { 0.0f, 1.0f, 0.0f, 1.0f };	//�J�����̏����
	mat = XMMatrixLookAtLH(eyePosition, focuPosition, upDirection);
	XMStoreFloat4x4(&g_cbChangesEveryFrame.View, XMMatrixTranspose(mat));
	//�_�������W
	g_vLightPos = XMFLOAT3(3.0f, 3.0f, -3.0f);
	XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&g_vLightPos), mat);
	XMStoreFloat3(&g_cbChangesEveryFrame.Light, vec);
	//�萔�o�b�t�@�i�ύX�p�x�̒Ⴂ�j�̃}�b�v�擾
	hr = g_pImmediateContext->Map(
		g_pCBuffer[1],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryFrame, sizeof(cbChangesEveryFrame));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[1], 0);

	//�萔�o�b�t�@�i�p�ɂɕύX����j���X�V
	//���[���h�ϊ��s��
	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(mat));
	//�萔�o�b�t�@�i�p�ɂɕύX����j�̃}�b�v�擾
	hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	return true;
}

bool D3DUtil::initVertexBuffer(){
	/*********************************************/
	/*          ���_�o�b�t�@�̍쐬               */
	/*********************************************/

	XYZBUFFER pos[] = {
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3(1.0f, 1.0f, 0.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	};

	posVertex = pos;

	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage = D3D11_USAGE_DEFAULT;			//�f�t�H���g�g�p�@
	xyzBufferDesc.ByteWidth = sizeof(XYZBuffer) * 6;	//
	xyzBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���_�o�b�t�@
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags = 0;
	xyzBufferDesc.StructureByteStride = 0;

	//���_�o�b�t�@�i���_�j�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem = pos;	//�o�b�t�@�f�[�^�̏����l
	xyzSubData.SysMemPitch = 0;
	xyzSubData.SysMemSlicePitch = 0;

	//���_�o�b�t�@�i���_�j�̍쐬
	HRESULT hr = g_pD3DDevice->CreateBuffer(
		&xyzBufferDesc,
		&xyzSubData,
		&g_pVerBuffer[0]);
	if (FAILED(hr)){
		return false;
	}

	COLBUFFER col[] = {
			XMFLOAT3(1.0, 0.0f, 0.0f) ,  XMFLOAT3(1.0f, 0.0f, 0.0f) ,  XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f)
	};

	colVertex = col;

	//���_�o�b�t�@�i�F�j�̒�`
	D3D11_BUFFER_DESC colBufferDesc;
	colBufferDesc.Usage = D3D11_USAGE_DEFAULT;			//�f�t�H���g�g�p�@
	colBufferDesc.ByteWidth = sizeof(ColBuffer) * 6;	//
	colBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���_�o�b�t�@
	colBufferDesc.CPUAccessFlags = 0;
	colBufferDesc.MiscFlags = 0;
	colBufferDesc.StructureByteStride = 0;

	//���_�o�b�t�@�i�F�j�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA colSubData;
	colSubData.pSysMem = col;	//�o�b�t�@�̏����l
	colSubData.SysMemPitch = 0;
	colSubData.SysMemSlicePitch = 0;

	//���_�o�b�t�@�i�F�j�̃T�u���\�[�X�̍쐬
	hr = g_pD3DDevice->CreateBuffer(&colBufferDesc, &colSubData, &g_pVerBuffer[1]);
	if (FAILED(hr)){
		return false;
	}

	/*******************************************/
	/*      �C���f�b�N�X�o�b�t�@�쐬           */
	/*******************************************/

	//�C���f�b�N�X�o�b�t�@�̃T�u���\�[�X�̏����l�i���_ID�j
	UINT idxVertexID[] = {
		0, 1, 2, 0, 2, 3
	};

	//�C���f�b�N�X�o�b�t�@�̒�`
	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;	//�f�t�H���g�g�p�@
	idxBufferDesc.ByteWidth = sizeof(idxVertexID);
	idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	//�C���f�b�N�X�o�b�t�@
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags = 0;
	idxBufferDesc.StructureByteStride = sizeof(UINT);

	//�C���f�b�N�X�o�b�t�@�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem = idxVertexID;	//�o�b�t�@�f�[�^�̏����l
	idxSubData.SysMemPitch = 0;
	idxSubData.SysMemSlicePitch = 0;

	//�C���f�b�N�X�o�b�t�@�̍쐬
	hr = g_pD3DDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &g_pIdxBuffer);
	if (FAILED(hr)){
		return false;
	}

	return true;
}

bool D3DUtil::initInputAssembler(){
	/*************************************/
	/*      ���̓A�Z���u���̐ݒ�         */
	/*************************************/

	D3D11_INPUT_ELEMENT_DESC l[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	layout = l;

	//IA�ɒ��_�o�b�t�@��ݒ�
	UINT strides[2] = {
		sizeof(XYZBUFFER),		//���_�o�b�t�@�i���_�j�Ɋ܂܂�钸�_�f�[�^�̃T�C�Y
		sizeof(COLBUFFER)		//���_�o�b�t�@�i�F�j�Ɋ܂܂�钸�_�f�[�^�̃T�C�Y
	};

	UINT offsets[2] = {
		0,					//���_�o�b�t�@�i���_�j�̃I�t�Z�b�g
		0					//���_�o�b�t�@�i�F�j�̃I�t�Z�b�g
	};
	g_pImmediateContext->IASetVertexBuffers(
		0,				//�ݒ肷��ŏ��̓��̓X���b�g�ԍ�
		2,				//�ݒ肷�钸�_�o�b�t�@�̐�
		g_pVerBuffer,	//�ݒ肷�钸�_�o�b�t�@�̔z��
		strides,		//�e���_�o�b�t�@�̗v�f�T�C�Y
		offsets			//�e���_�o�b�t�@�̃I�t�Z�b�g
		);

	//IA�ɃC���f�b�N�X�o�b�t�@��ݒ�
	g_pImmediateContext->IASetIndexBuffer(
		g_pIdxBuffer,			//�ݒ肷��C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,	//�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		0						//�C���f�b�N�X�o�b�t�@�̃I�t�Z�b�g
		);

	//���̓��C�A�E�g�I�u�W�F�N�g�̍쐬
	HRESULT hr = g_pD3DDevice->CreateInputLayout(
		layout,							//��`�̔z��
		_countof(l),				//��`�̗v�f��
		pBlobVS->GetBufferPointer(),	//�o�C�g�E�R�[�h�ւ̃|�C���^
		pBlobVS->GetBufferSize(),		//�o�C�g�E�R�[�h�̃T�C�Y
		&g_pInputLayout					//�󂯎��ϐ��̃|�C���^
		);
	if (FAILED(hr)){
		return false;
	}

	//���̓��C�A�E�g�I�u�W�F�N�g�̐ݒ�
	g_pImmediateContext->IASetInputLayout(g_pInputLayout);

	//�v���~�e�B�u�̎��
	g_pImmediateContext->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST	//�O�p�`���X�g
		);

	return true;
}

bool D3DUtil::initRasterizer(){
	/****************************************/
	/*          ���X�^���C�U�̐ݒ�          */
	/****************************************/

	//���X�^���C�U�X�e�[�g�I�u�W�F�N�g�̍쐬
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;	//���ʂ̕`��
	RSDesc.CullMode = D3D11_CULL_BACK;	//�ʂ̕\���ݒ�
	RSDesc.FrontCounterClockwise = FALSE;	//���v��肪�\��
	RSDesc.DepthBias = 0;				//�[�x�o�C�A�X�u0�v
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = FALSE;		//�[�x�N���b�s���O�Ȃ�
	RSDesc.ScissorEnable = FALSE;		//�V�U�[��`�Ȃ�
	RSDesc.MultisampleEnable = FALSE;	//�}���`�T���v�����O�Ȃ�
	RSDesc.AntialiasedLineEnable = FALSE;	//���C���A���`�G�C���A�V���O�Ȃ�
	HRESULT hr = g_pD3DDevice->CreateRasterizerState(
		&RSDesc,
		&g_pRasterizerState
		);
	if (FAILED(hr)){
		return false;
	}

	//RS�Ƀ��X�^���C�U�X�e�[�g�I�u�W�F�N�g��ݒ�
	g_pImmediateContext->RSSetState(g_pRasterizerState);

	return true;
}

bool D3DUtil::initOutputMerger(){
	/**********************************/
	/*      �o�̓}�[�W���[�ݒ�        */
	/**********************************/

	//�A���t�@�u�����f�B���O�̗L����
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;	//�A���t�@to�J�o���b�W�̗L��
	blendDesc.IndependentBlendEnable = FALSE;	//���������̃����_�[�^�[�Q�b�g�ɂ����ēƗ������u�����f�B���O��L�������邩�ǂ���
	blendDesc.RenderTarget[0].BlendEnable = TRUE;	//�u�����f�B���O�̗L����
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = g_pD3DDevice->CreateBlendState(&blendDesc, &g_pBlendState);
	if (FAILED(hr)){
		return false;
	}

	//OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pImmediateContext->OMSetBlendState(
		g_pBlendState,		//�ݒ肷��I�u�W�F�N�g
		BlendFactor,		//�萔�l(RGBA)
		0xffffffff);		//�T���v���p�}�X�N

	//�[�x�X�e���V���I�u�W�F�N�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable = TRUE;	//�[�x�e�X�g����
	DepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//��������
	DepthStencil.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;	//��O�̕��̂�`��
	DepthStencil.StencilEnable = FALSE;	//�X�e���V���e�X�g�Ȃ�
	DepthStencil.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	//�X�e���V���ǂݍ��݃}�X�N
	DepthStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	//�X�e���V���������݃}�X�N
		//�ʂ��\�������Ă���ꍇ�̃X�e���V���e�X�g�̐ݒ�
	DepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//�ێ�
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//�ێ�
	DepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//�ێ�
	DepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//��ɐ���
		//�ʂ����������Ă���ꍇ�̃X�e���V���e�X�g�̐ݒ�
	DepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//�ێ�
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//�ێ�
	DepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//�ێ�
	DepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//��ɐ���
	hr = g_pD3DDevice->CreateDepthStencilState(
		&DepthStencil,
		&g_pDepthStencilState);
	if (FAILED(hr)){
		return false;
	}

	//OM�ɐ[�x�X�e���V���X�e�[�g�I�u�W�F�N�g��ݒ�
	g_pImmediateContext->OMSetDepthStencilState(
		g_pDepthStencilState,
		0);//�X�e���V���e�X�g�Ŏg���Q�ƒl

	return true;
}

bool D3DUtil::initSampler(){

	//�T���v���X�e�[�g�I�u�W�F�N�g�̐ݒ�
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 2;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = -FLT_MAX;
	descSampler.MaxLOD = FLT_MAX;

	//�T���v���X�e�[�g�I�u�W�F�N�g�̍쐬
	HRESULT hr = g_pD3DDevice->CreateSamplerState(
		&descSampler,
		&g_pSampler);
	if (FAILED(hr)){
		return false;
	}

	//�s�N�Z���V�F�[�_�ɃT���v����ݒ�
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSampler);


	return true;
}

void D3DUtil::clearScreen(){
	//�`��^�[�Q�b�g�̃N���A
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//�N���A����l(RGBA)
	g_pImmediateContext->ClearRenderTargetView(
		g_pRenderTargetView,	//�N���A����`��^�[�Q�b�g
		ClearColor);			//�N���A����l

	//�[�x�X�e���V���̃N���A
	g_pImmediateContext->ClearDepthStencilView(
		g_pDepthStencilView,	//�N���A����[�x�X�e���V���r���[
		D3D11_CLEAR_DEPTH,		//�[�x�l�������N���A����
		1.0f,					//�[�x�o�b�t�@���N���A����l
		0);						//�X�e���V���o�b�t�@���N���A����l(���̏ꍇ�͖��֌W)
}

bool D3DUtil::loadTextrue(LPWSTR fileName,Texture* tex){
	/***************************************/
	/*�@�@�@�e�N�X�`���ǂݍ���             */
	/***************************************/
	D3DX11_IMAGE_INFO imageInfo;	//�����󂯎��ϐ�
	HRESULT hr;
	hr = D3DX11GetImageInfoFromFile(fileName, NULL, &imageInfo, &hr);
	if (FAILED(hr)){
		return false;
	}

	//�e�N�X�`���N���X�ɉ摜�̑傫����ۑ�
	tex->width = imageInfo.Width;
	tex->height = imageInfo.Height;

	//�쐬����e�N�X�`���̐ݒ�
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
	imageLoadInfo.Width = imageInfo.Width;	//�e�N�X�`���̕�
	imageLoadInfo.Height = imageInfo.Height;	//�e�N�X�`���̍���
	imageLoadInfo.Depth = 0;	//�e�N�X�`���̐[��
	imageLoadInfo.FirstMipLevel = 0;	//�ŏ��ɓǂݍ��ރ~�b�v�}�b�v���x��
	imageLoadInfo.MipLevels = 8;		//�~�b�v�}�b�v���x���̐�
	imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;	//�f�t�H���g�g�p�@
	imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X
	imageLoadInfo.CpuAccessFlags = 0;	//CPU����A�N�Z�X���Ȃ�
	imageLoadInfo.MiscFlags = 0;		//�I�v�V�����Ȃ�
	imageLoadInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//�t�H�[�}�b�g
	imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.pSrcInfo = NULL;
	//�e�N�X�`���ǂݍ���
	D3DX11CreateTextureFromFile(
		g_pD3DDevice,		//���\�[�X���쐬����f�o�C�X��
		fileName,			//�摜�t�@�C����
		&imageLoadInfo,		//�쐬����e�N�X�`���̐ݒ�
		NULL,				//�񓯊��Ŏ��s���Ȃ�	
		&tex->textrue,	//�e�N�X�`�����擾����ϐ�
		&hr					//�߂�l���擾����ϐ�
		);
	if (FAILED(hr)){
		return false;
	}
	//2D�e�N�X�`���ɃA�N�Z�X����V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = imageLoadInfo.Format;	//�t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;	//�ł��ڍׂȃ~�b�v�}�b�v���x��
	srDesc.Texture2D.MipLevels = -1;		//�S�Ẵ~�b�v�}�b�v���x��

	//�V�F�[�_�\���\�[�X�r���[�̍쐬
	hr = g_pD3DDevice->CreateShaderResourceView(
		tex->textrue,			//�A�N�Z�X����e�N�X�`�����\�[�X
		&srDesc,			//�V�F�[�_�\���\�[�X�r���[�̐ݒ�
		&tex->shaerResourceView		//�󂯎��ϐ�
		);
	if (FAILED(hr)){
		return false;
	}
	return true;
}

bool D3DUtil::drawGraph(Texture* tex,float x,float y,float scaleX,float scaleY){
	
	//PS�ɃV�F�[�_�\���\�[�X�r���[��ݒ�
	g_pImmediateContext->PSSetShaderResources(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[�̐�
		&tex->shaerResourceView	//�ݒ肷��V�F�[�_���\�[�X�r���[�̔z��
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);
	
	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(scale*mat));
	//�萔�o�b�t�@�i�p�ɂɕύX����j�̃}�b�v�擾
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	/****************************************/
	//�`��i�C���f�b�N�X�o�b�t�@���g���j
	g_pImmediateContext->DrawIndexed(
		6,		//���_��
		0,		//�C���f�b�N�X�o�b�t�@�̍ŏ��̃C���f�b�N�X����`��J�n
		0		//���_�o�b�t�@���̍ŏ��̒��_�f�[�^����g�p�J�n
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	return true;
}

bool D3DUtil::drawGraphAnimate(Texture* tex, float x, float y, float scaleX, float scaleY, float su, float sv, float eu, float ev){
	//PS�ɃV�F�[�_�\���\�[�X�r���[��ݒ�
	g_pImmediateContext->PSSetShaderResources(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[�̐�
		&tex->shaerResourceView	//�ݒ肷��V�F�[�_���\�[�X�r���[�̔z��
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);

	g_cbChangesEveryObject.Seek = XMFLOAT4(su, sv, eu, ev);

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(scale*mat));
	//�萔�o�b�t�@�i�p�ɂɕύX����j�̃}�b�v�擾
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	
	//�W�I���g���V�F�[�_
	g_pImmediateContext->GSSetShader(g_pAnimateGeometryShader, NULL, 0);
	/****************************************/
	//�`��i�C���f�b�N�X�o�b�t�@���g���j
	g_pImmediateContext->DrawIndexed(
		6,		//���_��
		0,		//�C���f�b�N�X�o�b�t�@�̍ŏ��̃C���f�b�N�X����`��J�n
		0		//���_�o�b�t�@���̍ŏ��̒��_�f�[�^����g�p�J�n
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	g_pImmediateContext->GSSetShader(g_pGeometryShader, NULL, 0);
	return true;
}

bool D3DUtil::refreshScreen(){
	//�o�b�N�o�b�t�@�̕\��
	HRESULT hr = g_pSwapChain->Present(
		0,	//��ʂ������ɍX�V����
		0);	//��ʂ����ۂɍX�V����

	return true;
}

bool D3DUtil::loadFontTextrue(LPWSTR fileName, Texture* tex){
	/***************************************/
	/*�@�@�@�e�N�X�`���ǂݍ���             */
	/***************************************/
	D3DX11_IMAGE_INFO imageInfo;	//�����󂯎��ϐ�
	HRESULT hr;
	hr = D3DX11GetImageInfoFromFile(fileName, NULL, &imageInfo, &hr);
	if (FAILED(hr)){
		return false;
	}

	//�e�N�X�`���N���X�ɉ摜�̑傫����ۑ�
	tex->width = 50;
	tex->height = 100;

	//�쐬����e�N�X�`���̐ݒ�
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
	imageLoadInfo.Width = imageInfo.Width;	//�e�N�X�`���̕�
	imageLoadInfo.Height = imageInfo.Height;	//�e�N�X�`���̍���
	imageLoadInfo.Depth = 0;	//�e�N�X�`���̐[��
	imageLoadInfo.FirstMipLevel = 0;	//�ŏ��ɓǂݍ��ރ~�b�v�}�b�v���x��
	imageLoadInfo.MipLevels = 8;		//�~�b�v�}�b�v���x���̐�
	imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;	//�f�t�H���g�g�p�@
	imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X
	imageLoadInfo.CpuAccessFlags = 0;	//CPU����A�N�Z�X���Ȃ�
	imageLoadInfo.MiscFlags = 0;		//�I�v�V�����Ȃ�
	imageLoadInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//�t�H�[�}�b�g
	imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.pSrcInfo = NULL;
	//�e�N�X�`���ǂݍ���
	D3DX11CreateTextureFromFile(
		g_pD3DDevice,		//���\�[�X���쐬����f�o�C�X��
		fileName,			//�摜�t�@�C����
		&imageLoadInfo,		//�쐬����e�N�X�`���̐ݒ�
		NULL,				//�񓯊��Ŏ��s���Ȃ�	
		&tex->textrue,	//�e�N�X�`�����擾����ϐ�
		&hr					//�߂�l���擾����ϐ�
		);
	if (FAILED(hr)){
		return false;
	}
	//2D�e�N�X�`���ɃA�N�Z�X����V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = imageLoadInfo.Format;	//�t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;	//�ł��ڍׂȃ~�b�v�}�b�v���x��
	srDesc.Texture2D.MipLevels = -1;		//�S�Ẵ~�b�v�}�b�v���x��

	//�V�F�[�_�\���\�[�X�r���[�̍쐬
	hr = g_pD3DDevice->CreateShaderResourceView(
		tex->textrue,			//�A�N�Z�X����e�N�X�`�����\�[�X
		&srDesc,			//�V�F�[�_�\���\�[�X�r���[�̐ݒ�
		&tex->shaerResourceView		//�󂯎��ϐ�
		);
	if (FAILED(hr)){
		return false;
	}
	return true;
}

bool D3DUtil::loadFontTexture(Texture* tex){
	/*******2D�e�N�X�`���쐬********/
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = FontLoder::gm.gmCellIncX;
	desc.Height = FontLoder::tm.tmHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	tex->width = desc.Width;
	tex->height = desc.Height;

	//ID3D11Texture2D* g_pTexture;
	SAFE_RELEASE(tex->dynamicsTexture);
	g_pD3DDevice->CreateTexture2D(&desc, NULL, &tex->dynamicsTexture);

	//���\�[�X�̃}�b�v
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HRESULT hr = g_pImmediateContext->Map(
		tex->dynamicsTexture,
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);
	if (FAILED(hr)){
		return false;
	}

	//�������W�̎Z�o
	UINT strX = FontLoder::gm.gmptGlyphOrigin.x;
	UINT strY = (FontLoder::tm.tmAscent - FontLoder::gm.gmptGlyphOrigin.y);

	//���\�[�X�̏�������
	UINT nLine = ((FontLoder::gm.gmBlackBoxX + 3) / 4) * 4;
	UCHAR* texel = (UCHAR*)MappedResource.pData;
	for (UINT row = 0; row < desc.Height; row++){	//�s
		UINT rowStart = row * MappedResource.RowPitch;
		for (UINT col = 0; col < desc.Width; col++){//��
			UINT colStart = col * 4;
			texel[rowStart + colStart + 0] = 255;	//R
			texel[rowStart + colStart + 1] = 255;	//G
			texel[rowStart + colStart + 2] = 255;	//B
			if ((strX < col && strX + FontLoder::gm.gmBlackBoxX > col) &&
				(strY < row && strY + FontLoder::gm.gmBlackBoxY > row)){
				texel[rowStart + colStart + 3] = (255 * FontLoder::bitmapData[((row - strY) * nLine) + (col - strX)]) / (65 - 1);
			}
			else{
				texel[rowStart + colStart + 3] = 0;
			}
			/*if (row == 0 || row == desc.Height-1){
				texel[rowStart + colStart + 3] = 255;
			}
			else if(col == 0 || col == desc.Width-1){
				texel[rowStart + colStart + 3] = 255;
			}*/
		}
	}

	//�}�b�v����
	g_pImmediateContext->Unmap(tex->dynamicsTexture, D3D11CalcSubresource(0, 0, 1));

	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	SAFE_RELEASE(tex->shaerResourceView);
	g_pD3DDevice->CreateShaderResourceView(
		tex->dynamicsTexture,
		&srDesc,
		&tex->shaerResourceView
		);
	//SAFE_RELEASE(g_pTexture);

	return true;
}

bool D3DUtil::drawString(float x, float y,float scaleX,float scaleY ,TCHAR* str,UINT size){
	for (UINT n = 0; n < size; n++){
		FontLoder::getCharGraph(&str[n]);
		loadFontTexture(&FontLoder::tex);
		drawGraph(&FontLoder::tex, x, y, scaleX, scaleY);
		x += FontLoder::tex.width*scaleX;
	}
	return true;
}

bool D3DUtil::drawString(float x, float y, float scaleX, float scaleY, std::wstring str){
	return drawString(x, y,scaleX,scaleY, (TCHAR*)str.c_str(), str.size());
}

int D3DUtil::getWindowY(){
	return windowHeight;
}

bool D3DUtil::changeScreenMode(){
	BOOL isFullScreen = false;
	g_pSwapChain->GetFullscreenState(&isFullScreen,0);
	if (isFullScreen){
		isFullScreen = false;
	}
	else{
		isFullScreen = true;
	}
	g_pSwapChain->SetFullscreenState(isFullScreen, 0);

	return true;
}

bool D3DUtil::getScreenMode(){
	BOOL isFullScreen = false;
	g_pSwapChain->GetFullscreenState(&isFullScreen, 0);
	return isFullScreen;
}

bool D3DUtil::changeSwapChain(){
	HRESULT hr;
	//�`��^�[�Q�b�g����������
	g_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	SAFE_RELEASE(g_pRenderTargetView);
	//�o�b�t�@�̕ύX
	hr = g_pSwapChain->ResizeBuffers(1,
		windowWidth,
		windowHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	if (FAILED(hr)){
		DebugBreak();
	}
	//initDeviceAndSwapChain();
	//�o�b�t�@�̍Đݒ�
	if (!initBackBuffer()){
		DebugBreak();
	}
	//�r���[�|�[�g�̍Đݒ�
	if (!initViewPort()){
		DebugBreak();
	}
	//�[�x�E�X�e���V���̍Đݒ�
	if (!initDepthAndStencil()){
		DebugBreak();
	}
	return true;
}

bool D3DUtil::changeDisplayMode(int x, int y){
	RECT clientRect;
	GetClientRect(*window, &clientRect);
	windowWidth = clientRect.right;
	windowHeight = clientRect.bottom;
	//�f�B�X�v���C���[�h�̕ύX
	DXGI_MODE_DESC desc;
	desc.Width = windowWidth;
	desc.Height = windowHeight;
	desc.RefreshRate.Numerator = 60;
	desc.RefreshRate.Denominator = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	HRESULT hr;
	hr = g_pSwapChain->ResizeTarget(&desc);
	if (FAILED(hr)){
		return false;
	}
}

bool D3DUtil::changeFullScreen(int x,int y){
	changeDisplayMode(x, y);
	//changeSwapChain(x, y);
	//changeScreenMode();
	return true;
}

HWND*						D3DUtil::window;
ID3D11Device*				D3DUtil::g_pD3DDevice;		//�f�o�C�X
ID3D11DeviceContext*		D3DUtil::g_pImmediateContext;	//�f�o�C�X�R���e�L�X�g
IDXGISwapChain*				D3DUtil::g_pSwapChain;		//�X���b�v�`�F�C��
ID3D11Texture2D*			D3DUtil::g_pDepthStencil;			//�[�x/�X�e���V���e�N�X�`�����󂯎��ϐ�
ID3D11DepthStencilView*		D3DUtil::g_pDepthStencilView;		//�[�x/�X�e���V���r���[

ID3D11VertexShader*			D3DUtil::g_pVertexShader;		//���_�V�F�[�_�I�u�W�F�N�g
ID3D11GeometryShader*		D3DUtil::g_pGeometryShader;	//�W�I���g���V�F�[�_�I�u�W�F�N�g
ID3D11GeometryShader*		D3DUtil::g_pAnimateGeometryShader;	//�����p�W�I���g���V�F�[�_
ID3D11PixelShader*			D3DUtil::g_pPixelShader;		//�s�N�Z���V�F�[�_�I�u�W�F�N�g

ID3DBlob*					D3DUtil::pBlobVS;				//���_�V�F�[�_�i�[�p�ϐ�
ID3DBlob*					D3DUtil::pBlobGS;				//�W�I���g���V�F�[�_�i�[�p�ϐ�
ID3DBlob*					D3DUtil::pBlobPS;				//�s�N�Z���V�F�[�_�\�i�[�p�ϐ�

//�萔�o�b�t�@�̃f�[�^
CbNeverChanges				D3DUtil::g_cbNeverChanges;	//�����ϊ��s��
CbChangesEveryFrame			D3DUtil::g_cbChangesEveryFrame;	//�r���[�ϊ��s��@�������W
CbChangesEveryObject		D3DUtil::g_cbChangesEveryObject;	//���[���h�ϊ��s��
ID3D11Buffer*				D3DUtil::g_pCBuffer[3]; //= { NULL, NULL, NULL };	//�萔�o�b�t�@
D3D11_MAPPED_SUBRESOURCE	D3DUtil::MappedResource;

//���X�^���C�U�X�e�[�g�I�u�W�F�N�g
ID3D11RasterizerState*		D3DUtil::g_pRasterizerState;

//�u�����h�X�e�[�g�I�u�W�F�N�g
ID3D11BlendState*			D3DUtil::g_pBlendState;

//�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g
ID3D11DepthStencilState*	D3DUtil::g_pDepthStencilState;

//���͗v�f�z��
D3D11_INPUT_ELEMENT_DESC*	D3DUtil::layout;

//���̓��C�A�E�g�I�u�W�F�N�g
ID3D11InputLayout*			D3DUtil::g_pInputLayout;

ID3D11Buffer*				D3DUtil::g_pVerBuffer[2];// = { NULL, NULL };	//���_�o�b�t�@�̃C���^�[�t�F�[�X
ID3D11Buffer*				D3DUtil::g_pIdxBuffer;		//�C���f�b�N�X�o�b�t�@�̃C���^�[�t�F�[�X

//�T���v���X�e�[�g�I�u�W�F�N�g
ID3D11SamplerState*			D3DUtil::g_pSampler;

//���_�o�b�t�@
XYZBUFFER*					D3DUtil::posVertex;
//�J���[�o�b�t�@
COLBUFFER*					D3DUtil::colVertex;
//�_�������W
XMFLOAT3					D3DUtil::g_vLightPos;

//�E�B���h�E�̑傫��
float						D3DUtil::windowWidth;
float						D3DUtil::windowHeight;

//�E�B���h�E���[�h
bool						D3DUtil::windowMode;

//�f�t�H���g�t�H���g
//FontLoder*					D3DUtil::defFont;

ID3D11RenderTargetView*		D3DUtil::g_pRenderTargetView;	//�`��^�[�Q�b�g�r���[