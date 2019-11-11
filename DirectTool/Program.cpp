#include "stdafx.h"
#include "Program.h"

#include "./Scenes/SceneMain.h"
#include "./View/CameraMove2D.h"

Program::Program()
{
	//게임정보저장 json or sql
	//jsonRoot = new Json::Value();
	//JsonHelper::ReadData(L"LevelEditor.json", jsonRoot);
	

	

	//camera = new CameraMove2D;
	
	sm = new SceneMain;
	shader = new Shader(L"./Shaders/Color.hlsl");

	worldBuffer = new WorldBuffer;
	vector<VertexColor> vertexData;
	vector<UINT> indexData;
	
	VertexColor temp(D3DXVECTOR3(-1.f, 1, 0),D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	vertexData.push_back(temp);
	temp.position = D3DXVECTOR3(1, 1, 0);
	vertexData.push_back(temp);
	temp.position = D3DXVECTOR3(-1, -1, 0);
	vertexData.push_back(temp);
	temp.position = D3DXVECTOR3(1, -1, 0);
	vertexData.push_back(temp);

	for (VertexColor& vc : vertexData)
		vc.position *= 60.f;

	indexData.push_back(0);
	indexData.push_back(1);
	indexData.push_back(2);
	indexData.push_back(1);
	indexData.push_back(3);
	indexData.push_back(2);

	UINT vertexCount = vertexData.size();
	UINT indexCount = indexData.size();
	Buffer::CreateVertexBuffer(&vertexBuffer, vertexData.data(), sizeof(VertexColor)*vertexCount);
	Buffer::CreateIndexBuffer(&indexBuffer, indexData.data(), sizeof(UINT)* indexCount);

	
	play = FloatRect(D3DXVECTOR2(WinSizeX / 2, WinSizeY / 2), 70, true);
	buffer = new PositionBuffer;


	angle = 45;
}

Program::~Program()
{
	SafeDelete(shader);
	SafeDelete(sm);
	//SafeDelete(camera);
	//JsonHelper::WriteData(L"LevelEditor.json", jsonRoot);
	//SafeDelete(jsonRoot);

	SafeDelete(buffer);
}

void Program::PreUpdate()
{
	//camera->Update();
	sm->PreUpdate();
}

void Program::Update(float tick)
{
	sm->Update(tick);

	if (Keyboard::Get()->Press('W'))
		play += D3DXVECTOR2(0, -40.f)*Time::Delta();
	if (Keyboard::Get()->Press('S'))
		play += D3DXVECTOR2(0, 40.f)*Time::Delta();
	if (Keyboard::Get()->Press('D'))
		play += D3DXVECTOR2(40.f, 0)*Time::Delta();
	if (Keyboard::Get()->Press('A'))
		play += D3DXVECTOR2(-40.f, 0)*Time::Delta();

	if (Keyboard::Get()->Press(VK_LEFT))
		angle += 20 * Time::Delta();
	if (Keyboard::Get()->Press(VK_RIGHT))
		angle -= 20 * Time::Delta();


}

void Program::PostUpdate()
{
	sm->PostUpdate();
}

void Program::Render()
{	

	RECT rc = { -50,-50,50,50 };




	Matrix2D world, local;
	D2D1::Matrix3x2F  result;
	world.SetPos(WinSizeX / 2, WinSizeY / 2);
	local.SetRotate(angle, D3DXVECTOR2(0,0));
	result = local.GetResult() *  world.GetResult();

	p2DRenderer->GetRenderTarget()->SetTransform(result);


	p2DRenderer->FillRectangle(rc);
	RECT rc2 = { 150,50,250,150 };
	p2DRenderer->SetWorld(D2D1::Matrix3x2F::Identity());
	p2DRenderer->FillRectangle(rc2);


	p2DRenderer->FillRectangle(play.GetRect(),DefaultBrush::red);

	
}

void Program::PostRender()
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	DeviceContext->IASetInputLayout(nullptr);
	DeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	shader->Render();

	//camera->Render();
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	//worldBuffer->SetMatrix(mat);
	//worldBuffer->SetVSBuffer(9);
	//DeviceContext->DrawIndexed(6, 0, 0);

	//States::SetBlend(States::BLENDING_ON);
	//States::SetBlend(States::BLENDING_OFF);
	buffer->SetPosition(D3DXVECTOR2(play.left + (play.right - play.left) / 2, play.top + (play.bottom - play.top) / 2));
	buffer->SetVSBuffer(1);
	buffer->SetPSBuffer(1);
	pRenderer->TurnOnAlphaBlend();
	DeviceContext->Draw(4, 0);

	
	sm->Render();
}

void Program::ImguiRender()
{
	ImGui::Begin("Camera");
	ImGui::Text("FPS : %f", Time::Get()->FPS());
	ImGui::Text("Tick : %f", Time::Delta());
	ImGui::Text("PosX : %f, PosY : %f", play.left, play.right);
	ImGui::End();

	//camera->UIRender();
	sm->ImguiRender();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);
}

