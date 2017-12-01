#include <Windows.h>
#include <d3dx9.h>

#include "GameTimer.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// windows�� H��� ���¸� �ٴ´� h�� (handle:�ڵ�) id����
// HINSTANCE hInstance : ���� �������α׷� �ν��Ͻ� �ڵ� ,  ���� �������α׷��� �����ؼ� ���� 
// HINSTANCE hPrevInstance : ���� �������α׷� �ν��Ͻ� �ڵ�(�Ⱦ���)
// PSTR pCmdLine : ���α׷��� �����ϴ� �� ���� �μ� ���ڿ�, add 5, 5
// int nCmdShow : â�� ��� ������ ���ΰ�? 

// ������ ���α׷����� ����
// 1. ���� �������α׷�(â)�� �����ϴ� ���� �ٽ� ( �׷��� ��Ƽ�½�ŷ�� ���� )
// 2. �����찡 '�ڿ�����'�� ���ش�. ( �ڿ��� ���� ����, ������ �ϸ� �ȵǰ�, '�����쿡�� ��û'�� �ؾ���)
// 3. �ٸ� �������α׷����� ���� �ʿ��� �޸𸮸� �̹� ����ϰ� ���� ���, �����쿡�� ������ ������.
// 4. �̺�Ʈ �ּ� ���α׷���
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
	bool isWindow = true;

	// ������ �������α׷�(â) �۵� ��Ÿ�� ����� ��� 
	// ����Ʈ����
	WNDCLASS wc;
	
	wc.style = CS_HREDRAW | CS_VREDRAW;						// â�� ��Ÿ�� ����
	wc.lpfnWndProc = WndProc;								// ������ ���ν���
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;								// �������α׷� �ν��Ͻ� �ڵ� ����
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				// ������
	wc.hCursor = LoadCursor(0, IDC_ARROW);					// Ŀ��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ���� ����
	wc.lpszMenuName = 0;									// ������ �޴� ����
	wc.lpszClassName = L"GameAppStyle";						// ������ â ��Ÿ�� �̸�

	if (!RegisterClass(&wc))
	{
		return 0;
	}

	// ������ â�� �����ϰ� �ڵ��� ����
	HWND hWnd = CreateWindow(
		L"GameAppStyle",					// �������α׷��� ��Ÿ�� (���ڿ��� �����쿡 ��ϵ� ��Ÿ��)
		L"2D Title Game",					// â�� ����
		WS_OVERLAPPEDWINDOW,				// ������ ��Ÿ��
		CW_USEDEFAULT, CW_USEDEFAULT,		// x ,y 
		CW_USEDEFAULT, CW_USEDEFAULT,		// �ʺ� , ����
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

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	int clientWidth = 1280;
	int clientHeight = 1024;
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
	
	/*
	// 1. �޽��� ���

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
	// ���̷�Ʈ X�� �׻� ����ۿ��� ����(����)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = clientWidth;		// ȭ�� ���� ����
	d3dpp.BackBufferHeight = clientHeight;		// ȭ�� ���� ����
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//	������ ����϶��� �������� ���� ���� ������ ���󰣴�.
	d3dpp.BackBufferCount = 1;			// ���۸� ���� (�� ������ ����)  * ����Ʈ ���� + ������� �����̹Ƿ� ���� ������۸�
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
		&device3d
	);

	if (FAILED(hr)) 
	{
		// 0 �� � ������� �������ʰ� ���ڴ�.
		MessageBox(0, L"Failed cCreateDevice", L"Error", MB_OK);
		return 0;
	}

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
			frameDuration += deltaTime;

			if (frameTime <= frameDuration)
			{
				frameDuration = 0;
				
				// DirectX ����̽��� �̿��ؼ� ȭ�鿡 ���� ä���
				// �����ش�.
				
				device3d->Clear(
					0,
					NULL,
					D3DCLEAR_TARGET,
					D3DCOLOR_XRGB(0, 128, 0), 
					0.0F,
					0);
				
				device3d->Present(NULL, NULL, NULL, NULL);
				// todo : GameUpdate
			}
		}
		
	}
	return 0;
}

// ������ ���ν��� �Լ� (������� ���õ� �������α׷��� ���������� ó��)
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
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

