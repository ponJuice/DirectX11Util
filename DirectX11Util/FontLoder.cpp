#include "FontLoder.h"

FontLoder::FontLoder(HWND hWnd,LPCWSTR fontName,UINT fontSize)
{
	
}

bool FontLoder::changeFont(LPCWSTR fontName,int fontSize){
	// �t�H���g�̐���
	hFont = ::CreateFont(fontSize, 0, 0, 0,
		FW_DONTCARE,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		fontName);
	if (hFont == NULL){
		return false;
	}
	oldFont = (HFONT)SelectObject(hdc, hFont);
	if (oldFont == NULL){
		return false;
	}
}

bool FontLoder::prevFont(){
	oldFont = (HFONT)SelectObject(hdc, oldFont);
	if (oldFont == NULL){
		return false;
	}
}

bool FontLoder::getCharGraph(TCHAR* c){
	//���Ƀf�[�^�����݂��Ă���ꍇ�͉������
	if (bitmapData != NULL){
		delete bitmapData;
	}
	UINT code = 0;
#if _UNICODE
	code = (UINT)*c;
#else
	if (IsDBCSLeadByte(*c))
		code = (BYTE)c[0] << 8 | (BYTE)c[1];
	else
		code = c[0];
#endif

	CONST MAT2 Mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
	
	GetTextMetrics(hdc, &tm);
	
	//�o�b�t�@�T�C�Y�擾
	DWORD bufferSize = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &gm, 0, NULL, &Mat);
	if (bufferSize == GDI_ERROR){
		//���s
		OutputDebugString(L"�t�H���g�f�[�^�̓ǂݍ��݂Ɏ��s\n");
		return false;
	}
	//�������̈�m��
	//bitmapData = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);
	bitmapData = new BYTE[bufferSize];
	//��������bitmap�f�[�^����������
	GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &gm, bufferSize, bitmapData, &Mat);

	return true;
}

bool FontLoder::initFont(HWND hWnd, LPCWSTR fontName, UINT fontSize){
	hwnd = hWnd;
	hdc = GetDC(hwnd);

	if (hdc == NULL){
		OutputDebugString(L"�f�o�C�X�R���e�L�X�g�n���h���擾���s\n");
		return false;
	}

	// �t�H���g�̐���
	hFont = ::CreateFont(fontSize, 0, 0, 0,
		FW_DONTCARE,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		fontName);
	if (hFont == NULL){
		OutputDebugString(L"�t�H���g�̎擾�Ɏ��s�B���C���I�����[�h���܂�\n");
		hFont = ::CreateFont(fontSize, 0, 0, 0,
			FW_DONTCARE,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			L"���C���I");
	}

	oldFont = (HFONT)SelectObject(hdc, hFont);
	if (oldFont == NULL){
		OutputDebugString(L"�f�t�H���g�̃t�H���g�擾�Ɏ��s\n");
		return false;
	}
	return true;
}

FontLoder::~FontLoder()
{
	tex.releaseTexture();
	delete bitmapData;
	ReleaseDC(hwnd,hdc);
}

FontLoder::FontLoder(){

}

HFONT FontLoder::hFont;
HFONT FontLoder::oldFont;
HDC FontLoder::hdc;
GLYPHMETRICS FontLoder::gm;
TEXTMETRIC FontLoder::tm;
HWND FontLoder::hwnd;
BYTE* FontLoder::bitmapData;
Texture FontLoder::tex;