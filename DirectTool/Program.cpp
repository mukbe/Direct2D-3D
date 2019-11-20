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

	
	
	tex = new Texture(L"../_Resources/Box.png");
	Texture* tex2 = new Texture(L"../_Resources/blueHorse.png",20,13);


	gameObject = new GameObject("", D3DXVECTOR2(WinSizeX / 2, WinSizeY / 2));
	//gameObject->SetTexture(tex);
	gameObject->SetTexture(tex2);

	gameObject2 = new GameObject("");
	gameObject2->SetActive(false);
	gameObject2->SetTexture(tex2);

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

	sm->PreUpdate();
	gameObject->PreUpdate();
	gameObject2->PreUpdate();

}

void Program::Update(float tick)
{
	sm->Update(tick);


	gameObject->Update();
	gameObject2->Update();

}

void Program::PostUpdate()
{
	sm->PostUpdate();
	gameObject->PostUpdate();
	gameObject2->PostUpdate();
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
	gameObject2->PostRender();
}

void Program::ImguiRender()
{
	CAMERA->ImguiRender();

	sm->ImguiRender();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);
}

