#include "stdafx.h"
#include "ObjectManager.h"
#include "PhysicsWorld.h"
#include "GameObject.h"
#include "Shape.h"
#include "RigidBody.h"

ObjectManager::ObjectManager()
{

	world = new PhysicsWorld;

	GameObject* object = new GameObject("TEST1", D3DXVECTOR2(400, 300), D3DXVECTOR2(200, 200), ObjectType::Charactor,Pivot::CENTER);
	Circle shape(100.f);
	RigidBody * body = world->Add(object, RigidType::Dynamic, &shape);
	body->SetStatic();

	object = new GameObject("TEST2", D3DXVECTOR2(200, 400), D3DXVECTOR2(2000, 100), ObjectType::Charactor, Pivot::CENTER);
	PolygonShape poly;
	poly.SetBox(1000.0f, 50.0f);
	body = world->Add(object, RigidType::Dynamic, &poly);
	body->SetStatic();
	body->SetOrient(0);


}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::PreUpdate()
{
}

void ObjectManager::Update(float tick)
{
	if (Keyboard::Get()->Up('G'))
		world->bActive = !world->bActive;

	if (Mouse::Get()->Down(1))
	{
		D3DXVECTOR2 pos = CAMERA->GetMousePos();

		GameObject* object = new GameObject("TEST1", pos, D3DXVECTOR2(50, 50), ObjectType::Charactor, Pivot::CENTER);
		//Circle shape(25.f);
		PolygonShape shape;
		shape.SetBox(25.0f, 25.0f);
		RigidBody * body = world->Add(object, RigidType::Dynamic, &shape);
		body->SetMass(10);

	}


	world->Update();
}

void ObjectManager::PostUpdate()
{
}

void ObjectManager::Render()
{
	world->Render();
}

void ObjectManager::PostRender()
{
}

void ObjectManager::ImguiRender()
{
	ImGui::Begin("ObjectManager");
	if (world->GetBody().size() > 2)
	{
		D3DXVECTOR2 pos = world->GetBody()[2]->GetVelocity();
		string str;
		str += "velocity x : " + to_string(pos.x) + "velocity y :" + to_string(pos.y);
		ImGui::Text(str.c_str());
	}

	ImGui::End();
}

void ObjectManager::AddObject(GameObject * object)
{
}

