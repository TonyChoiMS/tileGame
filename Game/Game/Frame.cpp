#include "Frame.h"
#include "GameSystem.h"
#include "Texture.h"
Frame::Frame()
{
	_frameTime = 0.3f;
	_rotate = 0.0f;
}
Frame::~Frame()
{

}

void Frame::Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameTime)
{
	//_spriteDX = spriteDX;
	//textureDX = textureDX;
	_texture = texture;
	_textureRect.left = x;									//
	_textureRect.right = _textureRect.left + width;	//
	_textureRect.top = y;									//
	_textureRect.bottom = _textureRect.top + height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);		// 이미지 색상 (만약 흰색이면, 원본 그대로 적용)
	_rotate = rotate;
	_width = width;
	_height = height;
	_frameTime = frameTime;
}
void Frame::Render()
{
	//_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
	// 크기 조정
	D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f);
	// 회전
	D3DXVECTOR2 center = D3DXVECTOR2(_width / 2.0f, _height / 2.0f);
	//float roate =0.0f;
	float roate = _rotate;
	// 위치 결정
	//float x = 1280.0f / 2.0f - _width / 2.0f;
	//float y = 800.0f / 2.0f - _height / 2.0f;

	float x = _x - _width / 2.0f;
	float y = _y - _height / 2.0f;
	D3DXVECTOR2 translate = D3DXVECTOR2(x, y);


	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,					// 연산 결과
		NULL,						// 스케일의 중심점
		0.0f,						// 확대 회전 요소. 
		&scale,						// 스케일 크기(확대 ,크기결정)
		&center,					// 회전 중심
		roate,						// 회전값,
		&translate					// 평행 이동-> 위치가 결정
	);

	GameSystem::GetInstance()->GetSpriteDX()->SetTransform(&matrix);
	//_spriteDX->SetTransform(&matrix);
	GameSystem::GetInstance()->GetSpriteDX()->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);

	//_spriteDX->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);

}
void Frame::Relase()
{

}

void Frame::Reset()
{
	//_spriteDX = spriteDX;
	//_textureDX = textureDX;
}
void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}


float Frame::GetFrameTime()
{
	//GameTimer time;
	return _frameTime;
}