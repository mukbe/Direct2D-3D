#include "stdafx.h"
#include "Program.h"

#include "./Scenes/SceneMain.h"
#include "./Systems/Object/GameObject.h"
#include "./Systems/Object/ObjectTest.h"
#include "./Systems/Object/Bounding.h"
#include "./Systems/Object/ObjectManager.h"

#include "./Systems/Object/Player.h"

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

	
	objManager = new ObjectManager;

	player = new Player("mp3", D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0),ObjectType::UI);
	player->Init();
}

Program::~Program()
{
	SafeDelete(sm);
	player->Release();
	SafeDelete(player);
	//SafeDelete(camera);
	//JsonHelper::WriteData(L"LevelEditor.json", jsonRoot);
	//SafeDelete(jsonRoot);
}

void Program::PreUpdate()
{

	sm->PreUpdate();

	for (GameObject* obj : objs)
		obj->PreUpdate();


	objManager->PreUpdate();
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

	objManager->Update(tick);

	player->Update(tick);
}

void Program::PostUpdate()
{
	sm->PostUpdate();

	for (GameObject* obj : objs)
		obj->PostUpdate();

	objManager->PostUpdate();
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

	objManager->Render();
}

void Program::PostRender()
{
	for (GameObject* obj : objs)
		obj->PostRender();
	objManager->PostRender();
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




	



	player->ImguiRender();

	for (GameObject* obj : objs)
		obj->ImguiRender();


	objManager->ImguiRender();

}

void Program::ResizeScreen()
{
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);
}

