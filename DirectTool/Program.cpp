#include "stdafx.h"
#include "Program.h"

#include "./Scenes/SceneMain.h"
#include "./Systems/Object/GameObject.h"
#include "./Systems/Object/ObjectTest.h"
#include "./Systems/Object/Terrain.h"
#include "./Systems/Object/Bounding.h"

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
	Texture* tex3 = new Texture(L"../_Resources/Idle.png", 4, 1);
	Texture* tex4 = new Texture(L"../_Resources/Run_4.png", 10, 1);
	Texture* tex5 = new Texture(L"../_Resources/Jump.png", 2, 1);


	gameObject = new GameObject("", D3DXVECTOR2(WinSizeX / 2, WinSizeY / 2));
	//gameObject->SetTexture(tex);
	gameObject->SetTexture(tex2);

	gameObject2 = new ObjectTest("", D3DXVECTOR2(WinSizeX / 2, WinSizeY / 2));
	
	gameObject2->SetTexture(tex4);
	gameObject2->SetSprite(GameObject::State::Idle, tex3);
	gameObject2->SetSprite(GameObject::State::Run, tex4);
	gameObject2->SetSprite(GameObject::State::Jump, tex5);


	
	//objs.push_back(gameObject);
	objs.push_back(gameObject2);
	objs.push_back(new Terrain("", D3DXVECTOR2(WinSizeX / 2, WinSizeY - 200)));

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

	for (GameObject* obj : objs)
		obj->PreUpdate();

	//FloatRect player, terrain, temp;
	//player = objs[0]->GetBounding()->GetRect();
	//terrain = objs[1]->GetBounding()->GetRect();
	//temp = player;


	//if (Math::IsAABBInAABBReaction(&player, terrain) )
	//{
	//	objs[0]->SetSpeed();
 //		objs[0]->MovePos(D3DXVECTOR2(0.f, -(temp.bottom - player.bottom)));
	//}

}

void Program::Update(float tick)
{
	sm->Update(tick);

	for (GameObject* obj : objs)
		obj->Update(tick);


}

void Program::PostUpdate()
{
	sm->PostUpdate();

	for (GameObject* obj : objs)
		obj->PostUpdate();

}

void Program::Render()
{	
	for (GameObject* obj : objs)
		obj->Render();


	CAMERA->GetView().Bind();
	p2DRenderer->DrawLine(D3DXVECTOR2(-10000, 0), D3DXVECTOR2(10000, 0));
	p2DRenderer->DrawLine(D3DXVECTOR2(0, -10000), D3DXVECTOR2(0, 10000));

	wstring str;
	str += L"pos.x : " + to_wstring(CAMERA->GetMousePos().x).substr(0,6);
	str += L"pos.y : " + to_wstring(CAMERA->GetMousePos().y).substr(0,6);
	p2DRenderer->DrawText2D(Mouse::Get()->GetPosition().x-200, Mouse::Get()->GetPosition().y-20, str, 20);
}

void Program::PostRender()
{
	for (GameObject* obj : objs)
		obj->PostRender();

}

void Program::ImguiRender()
{
	static bool bLockFPS = true;
	static int fps = Time::Get()->GetLockFPS();
	ImGui::Begin("Info");
	ImGui::Text("FPS : %f", Time::Get()->FPS()); ImGui::SameLine();

	if (ImGui::Checkbox("FPS Lock", &bLockFPS) || ImGui::InputInt("FPS Value", &fps))
	{
		if(bLockFPS)
			Time::Get()->SetLockFPS(fps);
		else 
			Time::Get()->SetLockFPS(0.f);

	}




	ImGui::Text("Tick : %f", Time::Delta());
	ImGui::Text("PosX : %.2f, PosY : %.2f", CAMERA->GetPos().x,CAMERA->GetPos().y);
	ImGui::End();


	ImGui::Begin("TEST");

	D3DXVECTOR2 pos = objs[0]->Transform()->GetPos();
	
	ImGui::Text("pos.x : %.2f , pos.y : %.2f", pos.x, pos.y);
	ImGui::Text("rc.bottom : %.2f ", objs[0]->GetBounding()->GetRect().bottom);

	ImGui::End();




	for (GameObject* obj : objs)
		obj->ImguiRender();


}

void Program::ResizeScreen()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);
}

