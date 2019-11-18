#include "stdafx.h"
#include "Program.h"

#include "./Scenes/SceneMain.h"

Program::Program()
{
	//게임정보저장 json or sql
	//jsonRoot = new Json::Value();
	//JsonHelper::ReadData(L"LevelEditor.json", jsonRoot);
	
	

	//camera = new CameraMove2D;
	
	sm = new SceneMain;

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

	tex = new Texture(L"../_Resources/Box.png");

	gameObject = new GameObject("", D3DXVECTOR2(WinSizeX / 2, WinSizeY / 2));
	gameObject->SetTexture(tex);

	gameObject2 = new GameObject("");

}

Program::~Program()
{
	SafeDelete(sm);
	//SafeDelete(camera);
	//JsonHelper::WriteData(L"LevelEditor.json", jsonRoot);
	//SafeDelete(jsonRoot);
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



	if (Keyboard::Get()->Down('T'))
		CAMERA->AddZoom(0.1f);
	if (Keyboard::Get()->Down('R'))
		CAMERA->AddZoom(-0.1f);
	gameObject->Update();

}

void Program::PostUpdate()
{
	sm->PostUpdate();
}

void Program::Render()
{	
	gameObject->Render(true);
	gameObject2->Render(true);



	//p2DRenderer->FillRectangle(play.GetRect(),DefaultBrush::red);

	
}

void Program::PostRender()
{
	gameObject->PostRender();
}

void Program::ImguiRender()
{
	ImGui::Begin("Camera");
	ImGui::Text("FPS : %f", Time::Get()->FPS());
	ImGui::Text("Tick : %f", Time::Delta());
	ImGui::Text("PosX : %.2f, PosY : %.2f", CAMERA->GetPos().x, CAMERA->GetPos().y);
	ImGui::Separator();
	ImGui::Text("Player Position");
	ImGui::Text("X : %f, Y : %f", play.left, play.right);
	ImGui::End();

	//camera->UIRender();
	sm->ImguiRender();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);
}

