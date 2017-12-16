#include <Windows.h>
#include <d3dx9.h>
#include "ResourceManager.h"
#include "GameTimer.h"
#include "Sprite.h"
#include "Map.h"
#include "GameSystem.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Map* gMap = 0;

// windows�� H��� ���¸� �ٴ´� h�� (handle:�ڵ�) id����
// HINSTANCE hInstance : ���� �������α׷� �ν��Ͻ� �ڵ� ,  ���� �������α׷��� �����ؼ� ���� 
// HINSTANCE hPrevInstance : ���� �������α׷� �ν��Ͻ� �ڵ�(�Ⱦ���)
// PSTR pCmdLine : ���α׷��� �����ϴ� �� ���� �μ� ���ڿ�, add 5, 5
// int nCmdShow : â�� ��� ������ ���ΰ�? 

// ������ ���α׷����� ����
// 1. ���� �������α׷�(â)�� �����ϴ� ���� �ٽ� ( �׷��� ��Ƽ�½�ŷ�� ���� )
// 2. �����찡 '�ڿ�����'�� ���ش�. ( �ڿ��� ���� ����, ������ �ϸ� �ȵǰ�, '�����쿡�� ��û'�� �ؾ���)
// 3. �ٸ� �������α׷����� ���� �ʿ��� �޸𸮸� �̹� ����ϰ� ���� ���, �����쿡�� ������ ������.
// 4. �̺�Ʈ �ֵ� ���α׷���
//	- ���� ���� �ϴ� ���� �ƴ�, �����찡 �����ִ� "�̺�Ʈ �޽����� ���� ó��"�� �ϴ� ��
//	- �̺�Ʈ ó���� �ϰ� ���� ���� ��� idle (�ƹ��͵� ����)
//	- ��� �̺�Ʈ�� �����찡 ����
//	- ������ �̺�Ʈ�� ���� �ʾƵ� ó���� �ؾߵǴ� �κ��� ���� ������, ������� ���Ӱ� ��︮�� �ʴ´ٴ� �̾߱Ⱑ ����.

// Direct X -> �����츦 ��ġ�� �ʰ�, �ٷ� �ϵ����� �����ϱ� ���� �����Ǵ� API
// ��ü�� �� �ִ� ���� OpenGL�� ����.(���� ����, �׷����� �ٷ�� ������谡 ���� ǥ��) - ��� �÷������� �ٷ� �� ������,
// ���Ӹ����� ���� ���� Direct X�� �� ������ ����.

// ������ ���α׷��� ����
// Direct X�� ��� �ϵ��� �����ϴ°� ?

// ������ ������ ���ٰ� ����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	// ���� �κ� �޴����� ������ ��
	int clientWidth = 1280;
	int clientHeight = 800;

	bool isWindow = true;

	// ����Ʈ����
	// ������ �������α׷�(â) �۵� ��Ÿ�� ����� ��� 
	WNDCLASS wc;
	
	wc.style = CS_HREDRAW | CS_VREDRAW;						// â�� ��Ÿ�� ����
	wc.lpfnWndProc = WndProc;								// ������ ���ν���
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;								// �������α׷� �ν��Ͻ� �ڵ� ����
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				// ������
	wc.hCursor = LoadCursor(0, IDC_ARROW);					// Ŀ��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ��� ����
	wc.lpszMenuName = 0;									// ������ �޴� ����
	wc.lpszClassName = L"GameAppStyle";						// ������ â ��Ÿ�� �̸�
	// �����쿡 ����ش޶�� ��û(RegisterClass)
	if (!RegisterClass(&wc))
	{
		// ����� �ȵ� ��� ����
		return 0;
	}

	// ������ ��Ÿ��( â���� Ǯ��ũ������ �ɼ� ���� �ٸ�)
	DWORD style = isWindow ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	if (isWindow)
	{
		style = WS_OVERLAPPEDWINDOW;					// ��� â �ɼ��� �� ������.
	}
	else
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	// ������ â�� �����ϰ� �ڵ��� ���� (id)
	HWND hWnd = CreateWindow(
		// ���ڿ� ���� L�� ���ڿ��� �����ڵ�� �������ִ� Ű����
		L"GameAppStyle",					// �������α׷��� ��Ÿ�� (���ڿ��� �����쿡 ��ϵ� ��Ÿ��)
		L"2D Title Game",					// â�� ����
		style,								// ������ ��Ÿ��
		CW_USEDEFAULT, CW_USEDEFAULT,		// x ,y 
		clientWidth, clientHeight,			// �ʺ� , ����
		0,									// �θ� â�� ���� ��, �θ�â�� �ڵ�
		0,									// �޴� �ڵ�
		hInstance,							// ��������� ������ â�� ����� ���� ���α׷� �ڵ� ����
		0									
	);

	if (NULL == hWnd)
	{
		return 0;
	}

	//  �������α׷��� ����Ű�� �����츦 �����ְ�, ������Ʈ�Ѵ�..
	// ������ â�� �����Ǵ� ��, �ѹ��� �����Ű���.(show, update �Ѵ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// â����̸� Ŭ���̾�Ʈ ũ�⸦ ����
	if (true == isWindow)
	{
		// ������ ũ�⸦ �����ؼ�,
		// ������ ������ ���� ���� ������ �ǵ��� �Ѵ�.
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd,
			0, 0,
			clientWidth + (clientWidth - clientRect.right),
			clientHeight + (clientHeight - clientRect.bottom),
			TRUE			//window, x, y, width, height, boolean �����츦 �ٽ� �θ��ڴ�
		);
	}
	///////////////////������ â�� ���� ���� �ڵ� /////////////////////////////////////
	
	/*
	// 1. �޽��� ���
	���ӿ��� ������� �ʴ� �ڵ�
	MSG msg;
	while (-1 != GetMessage(&msg, 0, 0, 0))	// ť���� �޽����� �����´�.
	{
		TranslateMessage(&msg);				// Ű�ڵ幮�� �޼��� ��ȭ
		DispatchMessage(&msg);				// �޽����� â���� ���.
	}
	*/

	HRESULT hr;
	// DirectX
	// Direct 3D ��ü�� ����
	LPDIRECT3D9 direct3d;
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == direct3d)
	{
		return 0;
	}

	// � ������� ������ ������ ����
	// Ǯ��ũ���� ��� format�� ���� �߿�.
	D3DFORMAT format;
	if (isWindow)
	{
		format = D3DFMT_UNKNOWN;
	}
	else
	{
		format = D3DFMT_X8R8G8B8;
	}
	// ���̷�Ʈ X�� �׻� ����ۿ��� ����(����)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = clientWidth;		// ȭ�� ���� ����
	d3dpp.BackBufferHeight = clientHeight;		// ȭ�� ���� ����
	d3dpp.BackBufferFormat = format;			//	������ ����϶��� �������� ���� ���� ������ ���󰣴�.
	d3dpp.BackBufferCount = 1;					// ���۸� ���� (�� ������ ����)  * ����Ʈ ���� + ������� �����̹Ƿ� ���� ������۸�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = isWindow;		// ������ ����ΰ�
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// ���͹��� �������� ���� �ٲ�޶�.

	// DirectX ����̽��� ����
	// ����̽��� ���ؼ� �ϵ��� ���� ������ ����
	LPDIRECT3DDEVICE9 device3d;
	// �Ķ���Ϳ� �� ������ �̿��ؼ� ����̽��� ������� ���� ��û
	hr = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,					// �ϵ���� ������ �̿��ϰڴ�.
		hWnd,							// �ڵ��� �Ѱ��ְڴ�.
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// �ϵ���� ����
		//D3DCREATE_SOFTWARE_VERTEXPROCESSING,		// ����Ʈ���� ���� 
		&d3dpp,							// Direct 3d�� ǥ������ �� ��� ǥ���������� �����ϴ� Params
		&device3d						// ������� Direct3d
	);

	if (FAILED(hr)) 
	{
		// 0 �� � ������� �������ʰ� ���ڴ�.
		MessageBox(0, L"Failed cCreateDevice", L"Error", MB_OK);
		return 0;
	}

	// Spirte COM �������̽� ���
	// X�� ���� ���̺귯���� �ƴ�.
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(device3d, &spriteDX);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed D3DXCreateSprite", L"Error", MB_OK);
		return 0;
	}

	GameSystem::GetInstance()->SetClientWidth(clientWidth);
	GameSystem::GetInstance()->SetClientHeight(clientHeight);

	gMap = new Map(L"Map");
	gMap->Init(device3d, spriteDX);
	
	// https://opengameart.org/
	// �̹��� ���Ͽ��� �ؽ��� �ε�
	// �̵�, ȸ��, ������ ����� ���
	//Sprite* testSprite = new Sprite(device3d, spriteDX);
	//testSprite->Init(L"character_sprite.png", L"TestScript.json");

	// FPS ���� ( 60 fps )
	float frameTime = 1.0f / 60.0f;
	float frameDuration = 0.0f;

	// Game Timer
	GameTimer gameTimer;
	gameTimer.Init();
	// �޼��� ���
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		// �޼��� ť�� �޽����� ������ ó��
		// PeekMessage -> GetMessage ��ü �Լ� (while���� ������ �ʾƵ� �̺�Ʈ�� �߻��� ��� ĳġ)
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);					// Ű�ڵ幮�� �޼��� ��ȭ
			DispatchMessage(&msg);					// �޽����� â���� ���.
		}
		else
		{
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTime();
			// ������, game update
			gMap->Update(deltaTime);
			
			//testSprite->Update(deltaTime);
			frameDuration += deltaTime;
			if (frameTime <= frameDuration)
			{
				frameDuration = 0;
				
				// DirectX ����̽��� �̿��ؼ� ȭ�鿡 ���� ä���
				// �����ش�.
				device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 128, 0),  0.0F, 0);
				{
					device3d->BeginScene();			// ���ݺ��� Scene�� �׸��� ����
					{
						// �׷����� ��� �̹����� �� �������� ���(2D/3D ���)
						// 2D�� �׸� ���� ���� ������ ����� ����
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);
						// Color key�� ������ ���� ����� �ƿ� ���ϴµ� ����,
						// Alphablend�� ����. ���İ��� ������ �ش� ���İ��� �̿� (color key�� ������ ���� �ٸ�)

							// 2D�� �� �������� �׷���
							//testSprite->Render();
							//float startX = 100.0f;
							//float startY = 100.0f;
						{
							gMap->Render();
							
						}
						spriteDX->End();
					}
					device3d->EndScene();			// Scene �׸��� ����
				}
				// ����� �Ǳ� ���� Lost Device�� ó��
				// ��� ���ǵǴ��� �� �� ���� ������, ������Ʈ ���� ��� ���� Ȯ���� �ѹ� �Ѵ�.
				// Device�� ���ǵǾ�����, �ٽ� �������༭ ��������� �ǵ��� ����� �Ѵ�.
				{
					// Device ���¸� Ȯ��
					hr = device3d->TestCooperativeLevel();
					if (FAILED(hr))	// Device ���¿� ���� �߻�
					{
						// Device ���� ���� Ȯ��( Lost Device )
						if (D3DERR_DEVICELOST == hr)	// ���� �Ұ��� ����
						{
							// �������� �ƴ� ���� ������ �Ұ����� ����
							// ��ٸ��� ���� ���� ���°� ��
							// ���� ������ ���·� �ٲ� ������ ��ٸ�
							// CPU�� ������ �� �ֵ���, �ٸ� �۾��� �Ͻ� ������
							Sleep(100);
						}
						else if (D3DERR_DEVICENOTRESET == hr)	// Lost ����������, ���� ���� ����
						{
							// ����, ��õ�
							// ���� ��� : Texture, device3d
							// �޸� ���� �� ��õ�

							//������ ������ ó��
							gMap->Release();
							

							direct3d = Direct3DCreate9(D3D_SDK_VERSION);
							if (NULL != direct3d)
							{
								hr = direct3d->CreateDevice(
									D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,					
									hWnd,							
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&device3d
								);
								if (SUCCEEDED(hr))
								{
									hr = D3DXCreateSprite(device3d, &spriteDX);
									if (SUCCEEDED(hr))
									{
										gMap->Reset(device3d, spriteDX);
										
									}
								}
							}
							//testSprite->Reset(device3d, spriteDX);
						}
					}
				}
				device3d->Present(NULL, NULL, NULL, NULL);		// ���
				// TODO : GameUpdate
			}
		}
	}
	// ���α׷��� ������ ����, ����ߴ� �ڿ��� �����Ѵ�.
	//delete testSprite;
	gMap->Deinit();
	delete gMap;
	gMap = NULL;
	

	ResourceManager::GetInstance()->RemoveAllTexture();
	if (spriteDX)
	{
		spriteDX->Release();
		spriteDX = NULL;
	}
	if (device3d)
	{
		device3d->Release();
		device3d = NULL;
	}
	if (direct3d)
	{
		direct3d->Release();
		direct3d = NULL;
	}

	return 0;
}

// **������ ���ν��� �Լ� (������� ���õ� �������α׷��� ���������� ó��)**
// ������ ���α׷��� �����Ű�� �ٽ� �ڵ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �޽��� ó��
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
		{
			DestroyWindow(hWnd);
		}

		if (VK_LEFT == wParam) gMap->MoveLeft();
		if (VK_RIGHT == wParam) gMap->MoveRight();
		if (VK_UP == wParam) gMap->MoveUp();
		if (VK_DOWN == wParam) gMap->MoveDown();

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	// ������ Ű�� �ƴ� ��� Window OS���� �˾Ƽ� ó���ش޶�� �Լ�
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

