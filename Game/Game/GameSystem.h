#pragma once

class GameSystem
{
	// Singleton
private:
	static GameSystem* _instance;

public:
	static GameSystem* GetInstance();

private:
	GameSystem() {}
	~GameSystem() {}

	// Window
private:
	float _clientWidth;
	float _clientHeight;

public:
	float GetClientWidth() { return _clientWidth;  }
	void SetClientWidth(float width) { _clientWidth = width; }
	float GetClientHeight() { return _clientHeight;  }
	void SetClientHeight(float height) { _clientHeight = height; }
};