#include <Windows.h>
#include <d3dx9.h>

#include "GameTimer.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// windows는 H라는 형태를 붙는다 h는 (handle:핸들) id개념
// HINSTANCE hInstance : 현재 응용프로그램 인스턴스 핸들 ,  현재 응용프로그램에 접속해서 관리 
// HINSTANCE hPrevInstance : 이전 응용프로그램 인스턴스 핸들(안쓰임)
// PSTR pCmdLine : 프로그램을 실행하는 데 쓰인 인수 문자열, add 5, 5
// int nCmdShow : 창을 어떻게 보여줄 것인가? 

// 윈도우 프로그래밍의 본질
// 1. 여러 응용프로그램(창)을 관리하는 것이 핵심 ( 그래야 멀티태스킹이 가능 )
// 2. 윈도우가 '자원관리'를 해준다. ( 자원을 임의 생성, 삭제를 하면 안되고, '윈도우에게 요청'을 해야함)
// 3. 다른 응용프로그램에서 내가 필요한 메모리를 이미 사용하고 있을 경우, 윈도우에서 접근을 제한함.
// 4. 이벤트 주소 프로그래밍
//	- 내가 직접 하는 것이 아닌, 윈도우가 던져주는 "이벤트 메시지에 대한 처리"를 하는 것
//	- 이벤트 처리를 하고 싶지 않을 경우 idle (아무것도 안함)
//	- 모든 이벤트는 윈도우가 관리
//	- 게임은 이벤트를 받지 않아도 처리를 해야되는 부분이 많기 때문에, 윈도우는 게임과 어울리지 않는다는 이야기가 있음.

// Direct X -> 윈도우를 거치지 않고, 바로 하드웨어로 접근하기 위해 제공되는 API
// 대체할 수 있는 것은 OpenGL이 있음.(게임 포함, 그래픽을 다루는 전산업계가 만든 표준) - 모든 플랫폼에서 다룰 수 있지만,
// 게임만으로 봤을 때는 Direct X가 더 성능이 좋음.

// 윈도우 프로그램의 구조
// Direct X가 어떻게 하드웨어에 접근하는가 ?

// 진입점 윈도우 에다가 전달
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	bool isWindow = true;

	// 윈도우 응용프로그램(창) 작동 스타일 만들고 등록 
	// 프론트버퍼
	WNDCLASS wc;
	
	wc.style = CS_HREDRAW | CS_VREDRAW;						// 창의 스타일 지정
	wc.lpfnWndProc = WndProc;								// 윈도우 프로시저
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;								// 응용프로그램 인스턴스 핸들 연결
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				// 아이콘
	wc.hCursor = LoadCursor(0, IDC_ARROW);					// 커서
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 벼경 색깔
	wc.lpszMenuName = 0;									// 윈도우 메뉴 생성
	wc.lpszClassName = L"GameAppStyle";						// 윈도우 창 스타일 이름

	if (!RegisterClass(&wc))
	{
		return 0;
	}

	// 윈도우 창을 생성하고 핸들을 얻음
	HWND hWnd = CreateWindow(
		L"GameAppStyle",					// 응용프로그램의 스타일 (문자열은 윈도우에 등록된 스타일)
		L"2D Title Game",					// 창의 제목
		WS_OVERLAPPEDWINDOW,				// 윈도우 스타일
		CW_USEDEFAULT, CW_USEDEFAULT,		// x ,y 
		CW_USEDEFAULT, CW_USEDEFAULT,		// 너비 , 높이
		0,									// 부모 창이 있을 시, 부모창의 핸들
		0,									// 메뉴 핸들
		hInstance,							// 만들어지는 윈도우 창을 사용할 응용 프로그램 핸들 세팅
		0									
	);

	if (NULL == hWnd)
	{
		return 0;
	}
	//  응용프로그램을 가리키는 윈도우를 보여주고, 업데이트한다..

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	int clientWidth = 1280;
	int clientHeight = 1024;
	// 창모드이면 클라이언트 크기를 조정
	if (true == isWindow)
	{
		// 윈도우 크기를 조정해서,
		// 지정한 영역이 실제 게임 영역이 되도록 한다.
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd,
			0, 0,
			clientWidth + (clientWidth - clientRect.right),
			clientHeight + (clientHeight - clientRect.bottom),
			TRUE			//window, x, y, width, height, boolean 윈도우를 다시 부르겠다
		);
	}
	
	/*
	// 1. 메시지 배분

	MSG msg;
	while (-1 != GetMessage(&msg, 0, 0, 0))	// 큐에서 메시지를 가져온다.
	{
		TranslateMessage(&msg);				// 키코드문자 메세지 변화
		DispatchMessage(&msg);				// 메시지를 창으로 배분.
	}
	*/

	HRESULT hr;
	// DirectX
	// Direct 3D 객체를 생성
	LPDIRECT3D9 direct3d;
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == direct3d)
	{
		return 0;
	}

	// 어떤 방식으로 보여줄 것인지 설정
	// 다이렉트 X는 항상 백버퍼에서 실행(메인)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = clientWidth;		// 화면 가로 길이
	d3dpp.BackBufferHeight = clientHeight;		// 화면 세로 길이
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//	윈도우 모드일때는 윈도우의 현재 색상 설정을 따라간다.
	d3dpp.BackBufferCount = 1;			// 버퍼링 갯수 (백 버퍼의 갯수)  * 프론트 버퍼 + 백버퍼의 갯수이므로 현재 더블버퍼링
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = isWindow;		// 윈도우 모드인가
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// 인터벌을 가능한한 빨리 바꿔달라.

	// DirectX 디바이스를 생성
	// 디바이스를 통해서 하드웨어에 직접 접근이 가능
	LPDIRECT3DDEVICE9 device3d;
	// 파라메터에 들어간 정보를 이용해서 디바이스를 만들어줄 것을 요청
	hr = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,					// 하드웨어 가속을 이용하겠다.
		hWnd,							// 핸들을 넘겨주겠다.
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// 하드웨어 가속
		//D3DCREATE_SOFTWARE_VERTEXPROCESSING,		// 소프트웨어 가속 
		&d3dpp,							// Direct 3d를 표현해줄 떄 어떻게 표현해줄지를 결정하는 Params
		&device3d
	);

	if (FAILED(hr)) 
	{
		// 0 은 어떤 윈도우든 가리지않고 띄우겠다.
		MessageBox(0, L"Failed cCreateDevice", L"Error", MB_OK);
		return 0;
	}

	// FPS 결정 ( 60 fps )
	float frameTime = 1.0f / 60.0f;
	float frameDuration = 0.0f;

	// Game Timer
	GameTimer gameTimer;
	gameTimer.Init();
	// 메세지 배분
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		// 메세지 큐에 메시지가 있으면 처리
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);					// 키코드문자 메세지 변화
			DispatchMessage(&msg);					// 메시지를 창으로 배분.
		}
		else
		{
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTime();
			// 없으면, game update
			frameDuration += deltaTime;

			if (frameTime <= frameDuration)
			{
				frameDuration = 0;
				
				// DirectX 디바이스를 이용해서 화면에 색을 채우고
				// 보여준다.
				
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

// 윈도우 프로시저 함수 (윈도우와 관련된 응용프로그램을 직접적으로 처리)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 처리
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

