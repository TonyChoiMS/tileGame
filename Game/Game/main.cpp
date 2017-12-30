#include <Windows.h>
#include <d3dx9.h>
#include "InputSystem.h"
#include "ResourceManager.h"
#include "ComponentSystem.h"
#include "GameSystem.h"
#include "GameTimer.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Sprite.h"

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
// 4. 이벤트 주도 프로그래밍 
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
	// 일정 부분 메뉴까지 포함한 값
	int clientWidth = 1280;
	int clientHeight = 800;

	bool isWindow = true;

	// 프론트버퍼
	// 윈도우 응용프로그램(창) 작동 스타일 만들고 등록 
	WNDCLASS wc;
	
	wc.style = CS_HREDRAW | CS_VREDRAW;						// 창의 스타일 지정
	wc.lpfnWndProc = WndProc;								// 윈도우 프로시저
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;								// 응용프로그램 인스턴스 핸들 연결
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				// 아이콘
	wc.hCursor = LoadCursor(0, IDC_ARROW);					// 커서
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 배경 색깔
	wc.lpszMenuName = 0;									// 윈도우 메뉴 생성
	wc.lpszClassName = L"GameAppStyle";						// 윈도우 창 스타일 이름
	// 윈도우에 등록해달라고 요청(RegisterClass)
	if (!RegisterClass(&wc))
	{
		// 등록이 안될 경우 종료
		return 0;
	}

	// 윈도우 스타일( 창모드와 풀스크린에서 옵션 값이 다름)
	DWORD style = isWindow ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	if (isWindow)
	{
		style = WS_OVERLAPPEDWINDOW;					// 모든 창 옵션을 다 가져옴.
	}
	else
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	// 윈도우 창을 생성하고 핸들을 얻음 (id)
	HWND hWnd = CreateWindow(
		// 문자열 앞의 L은 문자열을 유니코드로 변경해주는 키워드
		L"GameAppStyle",					// 응용프로그램의 스타일 (문자열은 윈도우에 등록된 스타일)
		L"2D Title Game",					// 창의 제목
		style,								// 윈도우 스타일
		CW_USEDEFAULT, CW_USEDEFAULT,		// x ,y 
		clientWidth, clientHeight,			// 너비 , 높이
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
	// 윈도우 창이 유지되는 한, 한번만 실행시키면됨.(show, update 둘다)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


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
	///////////////////윈도우 창을 띄우기 위한 코드 /////////////////////////////////////
	
	/*
	// 1. 메시지 배분
	게임에는 권장되지 않는 코드
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
	// 풀스크린일 경우 format의 값이 중요.
	D3DFORMAT format;
	if (isWindow)
	{
		format = D3DFMT_UNKNOWN;
	}
	else
	{
		format = D3DFMT_X8R8G8B8;
	}
	// 다이렉트 X는 항상 백버퍼에서 실행(메인)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = clientWidth;		// 화면 가로 길이
	d3dpp.BackBufferHeight = clientHeight;		// 화면 세로 길이
	d3dpp.BackBufferFormat = format;			//	윈도우 모드일때는 윈도우의 현재 색상 설정을 따라간다.
	d3dpp.BackBufferCount = 1;					// 버퍼링 갯수 (백 버퍼의 갯수)  * 프론트 버퍼 + 백버퍼의 갯수이므로 현재 더블버퍼링
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
		&device3d						// 만들어진 Direct3d
	);

	if (FAILED(hr)) 
	{
		// 0 은 어떤 윈도우든 가리지않고 띄우겠다.
		MessageBox(0, L"Failed cCreateDevice", L"Error", MB_OK);
		return 0;
	}

	// Spirte COM 인터페이스 얻기
	// X는 정식 라이브러리가 아님.
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(device3d, &spriteDX);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed D3DXCreateSprite", L"Error", MB_OK);
		return 0;
	}

	GameSystem::GetInstance()->SetClientWidth(clientWidth);
	GameSystem::GetInstance()->SetClientHeight(clientHeight);
	GameSystem::GetInstance()->SetDeviceDX(device3d);
	GameSystem::GetInstance()->SetSpriteDX(spriteDX);

	Map* map = new Map(L"Map");
	map->Init(L"MapSprite.png", L"MapData.csv");

	// 1. 캐릭터 생성
	Player* character = new Player(L"player");
	character->Init(L"character_sprite.png", L"player");
	
	map->SetViewer(character);

	// https://opengameart.org/
	// 이미지 파일에서 텍스쳐 로드
	// 이동, 회전, 스케일 행렬을 사용

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
		// PeekMessage -> GetMessage 대체 함수 (while문을 돌리지 않아도 이벤트가 발생할 경우 캐치)
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
			map->Update(deltaTime);
			// 캐릭터 업데이트
			character->Update(deltaTime);
			
			frameDuration += deltaTime;
			if (frameTime <= frameDuration)
			{
				frameDuration = 0;
				
				// DirectX 디바이스를 이용해서 화면에 색을 채우고
				// 보여준다.
				device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 128, 0),  0.0F, 0);
				{
					device3d->BeginScene();			// 지금부터 Scene을 그리기 시작
					{
						// 그려야할 모든 이미지는 이 영역에서 출력(2D/3D 모두)
						// 2D를 그릴 때는 따로 선언을 해줘야 가능
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);
						// Color key는 지정된 값은 출력을 아예 안하는데 반해,
						// Alphablend는 투명도. 알파값이 있으면 해당 알파값을 이용 (color key와 개념이 조금 다름)
						// 2D는 이 영역에서 그려줌
						{
							map->Render();
							//character->Render();
							
						}
						spriteDX->End();
					}
					device3d->EndScene();			// Scene 그리기 종료
				}
				// 출력이 되기 전에 Lost Device를 처리
				// 어디서 유실되는지 알 수 없기 때문에, 업데이트 이후 출력 전에 확인을 한번 한다.
				// Device가 유실되었으면, 다시 복구해줘서 정상출력이 되도록 해줘야 한다.
				{
					// Device 상태를 확인
					hr = device3d->TestCooperativeLevel();
					if (FAILED(hr))	// Device 상태에 문제 발생
					{
						// Device 유실 상태 확인( Lost Device )
						if (D3DERR_DEVICELOST == hr)	// 복구 불가인 상태
						{
							// 영원히가 아닌 지금 복구가 불가능인 상태
							// 기다리면 복구 가능 상태가 됨
							// 복구 가능한 상태로 바뀔 때까지 기다림
							// CPU가 복구할 수 있도록, 다른 작업은 일시 중지함
							Sleep(100);
						}
						else if (D3DERR_DEVICENOTRESET == hr)	// Lost 상태이지만, 복구 가능 상태
						{
							// 복구, 재시동
							// 복구 대상 : Texture, device3d
							// 메모리 해제 후 재시동

							//망가진 데이터 처리
							map->Release();
							character->Release();		// 캐릭터 릴리즈

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
										map->Reset();
										character->Reset();
									}
								}
							}
						}
					}
				}
				device3d->Present(NULL, NULL, NULL, NULL);		// 출력
				// TODO : GameUpdate
			}
		}
	}
	// 프로그램이 끝나기 전에, 사용했던 자원을 해제한다.
	ComponentSystem::GetInstance()->RemoveAllComponents();
	/*map->Deinit();
	delete map;
	map = NULL;

	character->Deinit();
	delete character;
	character = NULL;*/
	
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

// **윈도우 프로시저 함수 (윈도우와 관련된 응용프로그램을 직접적으로 처리)**
// 윈도우 프로그램을 실행시키는 핵심 코드
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 처리
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		InputSystem::GetInstance()->KeyDown(wParam);
		break;
	case WM_KEYUP:
		if (VK_ESCAPE == wParam)
		{
			DestroyWindow(hWnd);
		}
		InputSystem::GetInstance()->KeyUp(wParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	// 지정한 키가 아닐 경우 Window OS에게 알아서 처리해달라는 함수
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

