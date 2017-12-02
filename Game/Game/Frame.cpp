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

	// 화면에 출력될 이미지의 영역을 선택
	_textureRect.left = x;
	_textureRect.right = _textureRect.left + width;
	_textureRect.top = y;
	_textureRect.bottom = _textureRect.top + height;
	// 텍스쳐에 적용될 컬러( 필터링의 개념 )
	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);			// 흰색으로 하면 원본이미지 그대로 출력
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