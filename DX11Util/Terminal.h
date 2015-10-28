#pragma once
#pragma comment(lib,"d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx11d.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif
#pragma comment(lib,"dxerr.lib")

#pragma comment(lib, "winmm.lib")	//timeGetTime()�p

#define SAFE_RELEASE(x){if (x){(x)->Release();(x)=NULL;}}	//�C���^�[���[�X����悤�}�N��

#include <stdlib.h>
#include <Windows.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <xnamath.h>
#include <string>
#include "D3DUtil.h"
#include "Texture.h"
#include "FpsController.h"
#include "FontLoder.h"
#include "Button.h"