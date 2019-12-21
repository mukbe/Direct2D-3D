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

	object = new GameObject("TEST2", D3DXVECTOR2(200, 400), D3DXVECTOR2(4000, 100), ObjectType::Charactor, Pivot::CENTER);
	PolygonShape poly;
	poly.SetBox(2000.0f, 50.0f);
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

		GameObject* object = new GameObject("TEST1", pos, D3DXVECTOR2(300, 300), ObjectType::Charactor, Pivot::CENTER);
		PolygonShape shape;
		shape.SetBox(150.0f, 150.0f);
		RigidBody * body = world->Add(object, RigidType::Dynamic, &shape);
		body->SetMass(500.f);
		body->SetInertiaStatic();
	}

	if (Keyboard::Get()->Down('H'))
	{
		D3DXVECTOR2 pos = CAMERA->GetMousePos();

		GameObject* object = new GameObject("TEST1", pos, D3DXVECTOR2(100, 100), ObjectType::Charactor, Pivot::CENTER);
		Circle shape(50.f);
		RigidBody * body = world->Add(object, RigidType::Dynamic, &shape);
		body->SetMass(5.f);
		body->SetInertiaStatic();
	}

	if (Keyboard::Get()->Down(VK_SPACE))
	{
		float mass = world->GetBody()[2]->GetMass();
		world->GetBody()[2]->SetForce(D3DXVECTOR2(0, mass * -10000 * 2));

	}

	if (Keyboard::Get()->Press(VK_LEFT))
	{
		float mass = world->GetBody()[2]->GetMass();

		world->GetBody()[2]->SetForce(D3DXVECTOR2(-mass * 10000 * 2,0) * Time::Get()->Tick());

	}
	if (Keyboard::Get()->Press(VK_RIGHT))
	{
		float mass = world->GetBody()[2]->GetMass();
		world->GetBody()[2]->SetForce(D3DXVECTOR2(mass * 10000 * 2, 0) * Time::Get()->Tick());

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

		ImGui::Text("angle : ", world->GetBody()[2]->Transform()->GetRotation() );

	}

	ImGui::End();
}

void ObjectManager::AddObject(GameObject * object)
{
}

