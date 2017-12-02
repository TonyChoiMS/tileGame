#include <d3dx9.h>

class Frame
{
private:
	ID3DXSprite* _spriteDX;
	IDirect3DTexture9* _textureDX;

	//Texture 생성을 위한 변수
	RECT _textureRect;
	D3DCOLOR _textureColor;

public:
	Frame();
	~Frame();

	void Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX, int x, int y, int width, int height);
	void Render();
	void Release();
	void Reset();
};