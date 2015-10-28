#pragma once
#include "FpsController.h"
#include "KeybordNumber.h"
#include "FontLoder.h"
#include "KeybordInput.h"
#include "MouseInput.h"
using namespace DirectX;
	//�������[�h
	enum DX11UTIL_BLEND{MIX,ADDITION};

	//�萔�o�b�t�@�̃f�[�^��`�@�ύX���Ȃ��f�[�^
	typedef struct cbNeverChanged{
		XMFLOAT4X4 Projection;
	}CbNeverChanges;

	//�萔�o�b�t�@�̃f�[�^��`�@�ύX�p�x�̒Ⴂ�f�[�^
	typedef struct cbChangesEveryFrame{
		XMFLOAT4X4 View;	//�r���[�ϊ��s��
		XMFLOAT3 Light;		//�������W�i���[���h���W�n�j
		FLOAT dummy;		//�萔�o�b�t�@�̃T�C�Y��16�̔{���ɂ��邽�߂̃_�~�[
	}CbChangesEveryFrame;

	//�萔�o�b�t�@�̃f�[�^��`�@�ύX�p�x�̍����f�[�^
	typedef struct cbChangesEveryObject{
		XMFLOAT4X4 World;	//���[���h�ϊ��s��
		XMFLOAT4	Seek;	//�e�N�X�`���V�[�N
	}CbChangesEveryObject;

	//���_�o�b�t�@�i���_�j�Ɏ��߂�f�[�^�`��
	typedef struct XYZBuffer{
		XMFLOAT3 Position;	//���W�l
	}XYZBUFFER;

	//���_�o�b�t�@�i�F�j�Ɏ��߂�f�[�^�`��
	typedef struct ColBuffer{
		XMFLOAT3 Color;
	}COLBUFFER;

	class DX11Util
	{
	protected:
		static MSG						msg;				//���b�Z�[�W
		static HWND*					window;
		static ID3D11Device*			g_pD3DDevice;		//�f�o�C�X
		static ID3D11DeviceContext*		g_pImmediateContext;	//�f�o�C�X�R���e�L�X�g
		static IDXGISwapChain*			g_pSwapChain;		//�X���b�v�`�F�C��
		static ID3D11Texture2D*			g_pDepthStencil;			//�[�x/�X�e���V���e�N�X�`�����󂯎��ϐ�
		static ID3D11DepthStencilView*	g_pDepthStencilView;		//�[�x/�X�e���V���r���[

		static ID3D11VertexShader*		g_pVertexShader;		//���_�V�F�[�_�I�u�W�F�N�g
		static ID3D11GeometryShader*	g_pGeometryShader;	//�W�I���g���V�F�[�_�I�u�W�F�N�g
		static ID3D11GeometryShader*	g_pAnimateGeometryShader;	//�����p�W�I���g���V�F�[�_
		static ID3D11PixelShader*		g_pPixelShader;		//�s�N�Z���V�F�[�_�I�u�W�F�N�g

		static ID3DBlob*				pBlobVS;				//���_�V�F�[�_�i�[�p�ϐ�
		static ID3DBlob*				pBlobGS;				//�W�I���g���V�F�[�_�i�[�p�ϐ�
		static ID3DBlob*				pBlobPS;				//�s�N�Z���V�F�[�_�\�i�[�p�ϐ�

		//�萔�o�b�t�@�̃f�[�^
		static CbNeverChanges			g_cbNeverChanges;	//�����ϊ��s��
		static CbChangesEveryFrame		g_cbChangesEveryFrame;	//�r���[�ϊ��s��@�������W
		static CbChangesEveryObject		g_cbChangesEveryObject;	//���[���h�ϊ��s��
		static ID3D11Buffer*			g_pCBuffer[3]; //= { NULL, NULL, NULL };	//�萔�o�b�t�@
		static D3D11_MAPPED_SUBRESOURCE MappedResource;

		//���X�^���C�U�X�e�[�g�I�u�W�F�N�g
		static ID3D11RasterizerState*	g_pRasterizerState;

		//�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g
		static ID3D11DepthStencilState* g_pDepthStencilState;

		//���͗v�f�z��
		static D3D11_INPUT_ELEMENT_DESC* layout;

		//���̓��C�A�E�g�I�u�W�F�N�g
		static ID3D11InputLayout*		g_pInputLayout;

		static ID3D11Buffer*			g_pVerBuffer[2];// = { NULL, NULL };	//���_�o�b�t�@�̃C���^�[�t�F�[�X
		static ID3D11Buffer*			g_pIdxBuffer;		//�C���f�b�N�X�o�b�t�@�̃C���^�[�t�F�[�X

		//�T���v���X�e�[�g�I�u�W�F�N�g
		static ID3D11SamplerState*		g_pSampler;

		//���_�o�b�t�@
		static XYZBUFFER*				posVertex;
		//�J���[�o�b�t�@
		static COLBUFFER*				colVertex;
		//�_�������W
		static XMFLOAT3					g_vLightPos;

		//�E�B���h�E�̑傫��
		static float					windowWidth;
		static float					windowHeight;

		//�E�B���h�E���[�h
		static bool						windowMode;

		//�f�t�H���g�t�H���g
		static FontLoder*				defFont;

		static ID3D11RenderTargetView*	g_pRenderTargetView;	//�`��^�[�Q�b�g�r���[

		//�������[�h
		static ID3D11BlendState*		g_pAdditionBlendState;	//���Z
		static ID3D11BlendState*		g_pMixBlendState;		//�~�b�N�X

	public:
		/*******Directx11������******/
		static bool initDX11Util(HWND* targetWindow, float l_windowWidth, float l_windowHeight, bool fullScreen);
		/*******�t�H���g�n*******/
		static bool initFont(LPCWSTR fontName, UINT fontSize);
		/******��ʍX�V�n********/
		static bool refreshScreen();
		static void clearScreen();
		static bool changeScreenMode();
		static bool changeDisplayMode(int x, int y);
		static bool changeSwapChain();
		static bool changeFullScreen(int x, int y);
		static bool getScreenMode();
		/**********�A���t�@�u�����h�n*********/
		static bool setBlendState(DX11UTIL_BLEND blend);
		/**************************/
		static bool update(MSG *msg);
		/**************************/
		static bool resizeBuffer();
		static int getWindowY();
		static int getWindowX();
		/***********�I������************/
		static void CleanupDirect3D();

	protected:
		static bool initDeviceAndSwapChain();
		static bool initBackBuffer();
		static bool initViewPort();
		static bool initDepthAndStencil();
		static void initDispray();
		static bool isDeviceDisappearance();
		static bool InitDirect3D();
		static bool initShaderAndConstantBuffer();
		static bool initConstantBuffer();
		static bool initVertexBuffer();
		static bool initInputAssembler();
		static bool initRasterizer();
		static bool initOutputMerger();
		static bool initSampler();
	protected:
		DX11Util();
		~DX11Util();
	};