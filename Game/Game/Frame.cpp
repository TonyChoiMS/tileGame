#include "Frame.h"

Frame::Frame()
{

}

Frame::~Frame()
{

}

void Frame::Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX, int x, int y, int width, int height)
{
	_spriteDX = spriteDX;
	_textureDX = textureDX;

	// ȭ�鿡 ��µ� �̹����� ������ ����
	_textureRect.left = x;
	_textureRect.right = _textureRect.left + width;
	_textureRect.top = y;
	_textureRect.bottom = _textureRect.top + height;
	// �ؽ��Ŀ� ����� �÷�( ���͸��� ���� )
	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);			// ������� �ϸ� �����̹��� �״�� ���
}

void Frame::Render()
{
	_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
}

void Frame::Release()
{

}

void Frame::Reset()
{
	
}