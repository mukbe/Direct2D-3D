#include "stdafx.h"
#include "ObjectTest.h"
#include "Bounding.h"



ObjectTest::ObjectTest(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
	: GameObject(name,pos,size,p)
{
	state = Idle;
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
	

	float len = D3DXVec2Length(&accelerate);
	if (len != 0)
	{
		state = Jump;
		velocity.y < 0 ? frameX = 0 : frameX = 1;
		
	}
	
}

void ObjectTest::Render(bool isRelative)
{
	if (bActive == false)return;

	Matrix2D world = *transform;

	if (isRelative)
	{
		world = world * CAMERA->GetView();
	}

	world.Bind();

	Texture* tex = sprites[state];
	if (tex == nullptr)
	{
		LOG->Print("None Texture");
		return;
	}
	tex->FrameRender(frameX, frameY, size, 1.f, pivot);



	bound->Render();

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
		velocity.x = 0.f;
		state = Idle;
		frameX = frameY = 0;
	}

	if (Keyboard::Get()->Press('D'))
	{
		state = Run;
		velocity.x = 150.f;
		scale.x > 0.f ? scale.x *= 1.f : scale.x *= -1.f;

	}
	else if (Keyboard::Get()->Press('A'))
	{
		state = Run;
		velocity.x = -150.f;
		scale.x < 0.f ? scale.x *= 1.f : scale.x *= -1.f;
	}

	if (Keyboard::Get()->Up(VK_SPACE))
	{
		velocity = { 0.f, -2000.f };
		accelerate = { 0.f,3000.f };


	}
}
