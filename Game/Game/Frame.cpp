#include "Frame.h"
#include "Texture.h"

Frame::Frame()
{
	_frameTime = 0.0f;
}

Frame::~Frame()
{

}

void Frame::Init(ID3DXSprite* spriteDX, Texture* texture, int x, int y, int width, int height, float frameTime)
{
	_spriteDX = spriteDX;
	//_textureDX = textureDX;
	_texture = texture;

	// 화면에 출력될 이미지의 영역을 선택
	_textureRect.left = x;
	_textureRect.right = _textureRect.left + width;
	_textureRect.top = y;
	_textureRect.bottom = _textureRect.top + height;
	// 텍스쳐에 적용될 컬러( 필터링의 개념 )
	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);			// 흰색으로 하면 원본이미지 그대로 출력

	_frameTime = frameTime;

	_width = width;
	_height = height;
}

void Frame::Render()
{
	// 크기 조정
	D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f);

	// 회전
	D3DXVECTOR2 center = D3DXVECTOR2(_width / 2.0f, _height / 2.0f);
	float rotate = 0.0f;

	// 위치 결정
	float x = _x - _width / 2.0f;
	float y = _y - _height / 2.0f;
	D3DXVECTOR2 translate = D3DXVECTOR2(x, y);

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,				// 선언한 Matrix( 연산결과가 저장될 곳)
		NULL,					// Scale의 중심점
		0.0f,					// 확대 회전 요소.
		&scale,					// 확대. 크기 결정
		&center,				// 회전중심
		rotate,					// 회전 값
		&translate				// 평행 이동 -> 위치가 결정
	);

	_spriteDX->SetTransform(&matrix);
	//_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
	_spriteDX->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);
}

void Frame::Release()
{

}

//void Frame::Reset(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX)
void Frame::Reset(ID3DXSprite* spriteDX)
{
	_spriteDX = spriteDX;
	//_textureDX = textureDX;
}

void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}
