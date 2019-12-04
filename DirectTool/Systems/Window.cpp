#include "stdafx.h"
#include "Window.h"
#include "../Program.h"

Program* Window::program = nullptr;

WPARAM Window::Run()
{
	MSG msg = { 0 };

	D3DDesc desc;
	DxRenderer::GetDesc(&desc);

	Logging::Create();
	LOG->Init();
	//=========Init Manager=================================
	DxRenderer::Create();
	pRenderer->CreateDevice();
	pRenderer->CreateSwapChain();
	D2DRenderer::Create();
	p2DRenderer->Create2DBuffer();

	Keyboard::Create();
	Mouse::Create();
	CameraManager::Create();

	Time::Create();
	Time::Get()->Start();
	BufferManager::Create();
	ShaderManager::Create();

	Matrix2D::SetRenderTarget();

	{//Imgui Init
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(desc.Handle);
		ImGui_ImplDX11_Init(Device, DeviceContext);
		ImGui::StyleColorsDark();
	}
	//=====================================================

	program = new Program();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Time::Get()->Update();

			//imgui UI위에 마우스 있으면 게임 화면에만 키보드하고 마우스 정보 안보냄 => imgui만 누르기 위해서
			if (ImGui::IsAnyWindowFocused() == false && ImGui::IsAnyWindowHovered() == false)
			{
				Keyboard::Get()->Update();
				Mouse::Get()->Update();
				CameraManager::Get()->Update();
			}
			
			program->PreUpdate();
			program->Update(Time::Delta());
			program->PostUpdate();

			{//imgui Update
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
			}

			{//랜더링 영역
				
				//TODO Program Class를 처리하고 다른 이름으로 부여 Render 순서 d2d에서 라이팅을 제외한 모든것을 처리
				//d3d에서 파티클과 라이팅을 처리함(ComputeShader 가져와야됨)

				//기본적인 Draw는 d2d
				p2DRenderer->BeginDraw();
				program->Render();
				p2DRenderer->EndDraw();

				//이펙트, ui처리는 d3d
				pRenderer->BeginDraw();
				program->PostRender();
				program->ImguiRender();
				LOG->ImShowData();

				//TODO 데모 추가한 dll빌드한거 만들어야됨
				//static bool bImguiWin = true;
				//ImGui::ShowDemoWindow(&bImguiWin);DDD

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				pRenderer->EndDraw();
			}
		}
	}
	SafeDelete(program);


	//================Release Manager===============================
	ImGui_ImplDX11_Shutdown();

	Time::Delete();
	Mouse::Delete();
	Keyboard::Delete();
	CameraManager::Delete();

	D2DRenderer::Delete();
	pRenderer->Delete();
	//=============================================================
	LOG->Release();
	Logging::Delete();

	return msg.wParam;
}

Window::Window()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = desc.Instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = desc.AppName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (desc.bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)desc.Width;
		devMode.dmPelsHeight = (DWORD)desc.Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	desc.Handle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, desc.AppName.c_str()
		, desc.AppName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, desc.Instance
		, NULL
	);
	assert(desc.Handle != NULL);
	DxRenderer::SetDesc(desc);


	RECT rect = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.Width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.Height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		desc.Handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(desc.Handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc.Handle);
	SetFocus(desc.Handle);

	ShowCursor(true);
}

Window::~Window()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);

	if (desc.bFullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(desc.Handle);

	UnregisterClass(desc.AppName.c_str(), desc.Instance);
}

LRESULT CALLBACK Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);

	//imgui_win32 헤더에 주석처리 되어있는걸 풀어주어야 인식함
	if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;

	if (message == WM_SIZE)
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
		{
			if (program != NULL)
			{
				float width = (float)LOWORD(lParam);
				float height = (float)HIWORD(lParam);

				if (pRenderer != NULL)

					//TODO
					//pRenderer->ResizeScreen(width, height);
					program->ResizeScreen();
			}
		}
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

