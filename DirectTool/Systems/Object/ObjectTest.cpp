#include "stdafx.h"
#include "ObjectTest.h"
#include "Bounding.h"



ObjectTest::ObjectTest(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, ObjectType type, Pivot p)
	: GameObject(name,pos,size,type, p)
{
	frequency = 0.127f;
}


ObjectTest::~ObjectTest()
{
}

void ObjectTest::Init()
{
	GameObject::Init();
}

void ObjectTest::Release()
{
	GameObject::Release();
}

void ObjectTest::PreUpdate()
{
	GameObject::PreUpdate();

}

void ObjectTest::Update(float tick)
{
	GameObject::Update(tick);

	KeyControl();

}

void ObjectTest::PostUpdate()
{
	GameObject::PostUpdate();
	

	//float len = D3DXVec2Length(&accelerate);
	//if (len != 0)
	//{
	//	state = Jump;
	//	velocity.y < 0 ? frameX = 0 : frameX = 1;
	//	
	//}
}

void ObjectTest::Render(bool isRelative)
{
	GameObject::Render(isRelative);
}

void ObjectTest::PostRender()
{
	GameObject::PostRender();

}

void ObjectTest::ImguiRender()
{
	//ImGui::Begin("Object Test");
	//ImGui::SliderFloat("Frame Frequency", &frequency, 0.01f, 0.5f);
	//ImGui::Image((ImTextureID)defaultTexture->GetSRV(), ImVec2(defaultTexture->GetWidth(), defaultTexture->GetHeight()));

	//ImGui::End();

}

void ObjectTest::KeyControl()
{
	if (Keyboard::Get()->Down('D'))
	{
	}
	else if (Keyboard::Get()->Down('A'))
	{
	}
	if (Keyboard::Get()->Up('D') || Keyboard::Get()->Up('A'))
	{
	}

	if (Keyboard::Get()->Press('D'))
	{
	//	scale.x > 0.f ? scale.x *= 1.f : scale.x *= -1.f;

	}
	else if (Keyboard::Get()->Press('A'))
	{
	//	scale.x < 0.f ? scale.x *= 1.f : scale.x *= -1.f;
	}

	if (Keyboard::Get()->Up(VK_SPACE))
	{


	}
}
