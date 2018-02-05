#pragma once
#include <d3dx9.h>
#include <string>
class Font
{
private:
	ID3DXFont* _fontDX;
	D3DCOLOR _color;
	RECT _rect;
	std::wstring _text;

	int _fontsize;
	std::wstring _fontName;
	int _width;
	int _height;
public:
	Font(std::wstring fontname, int fontSize, D3DCOLOR fontColor);
	~Font();

	void Render();
	void Release();
	void Reset();

	void SetPosition(int x, int y);
	void SetRect(int x, int y, int width, int height);
	void SetText(std::wstring text);
};