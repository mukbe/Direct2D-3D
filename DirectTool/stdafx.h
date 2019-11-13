#pragma once

#include <windows.h>
#include <assert.h>

//STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
using namespace std;


//Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//다이렉트2D 라이브러리
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//다이렉트 Write 라이브러리 (텍스트)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC 라이브러리
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>



//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#include <imguiWin32.h>
#pragma comment(lib, "imgui.lib")

//Json Cpp
#include <json/json.h>
#pragma comment(lib, "jsoncpp.lib")
//#pragma warning(disable:4996)

//DirectXTex
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

#include "./Utilities/FloatRect.h"
#include "MacroFunction.h"

const wstring ShaderPath = L"_Shaders/";


#include "DefineHeader.h"
#include "./Systems/DxRenderer.h"
#include "./Systems/D2DRenderer.h"
#include "ModuleHeader.h"

#include "./Render/WorldBuffer.h"

#include "./Systems/GameObject.h"



