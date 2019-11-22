#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/Matrix2D.h"
#include "Bounding.h"

GameObject::GameObject(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
	:defaultTexture(nullptr)
	, pos(pos), name(name), bActive(true)
	, rotate(0.f), velocity(D3DXVECTOR2(0, 0)), accelerate(D3DXVECTOR2(0, 0))
	, alpha(1.f), size(size), pivot(p)
{
	worldBuffer = new WorldBuffer;
	shader = new Shader(L"./Shaders/Color.hlsl");

	transform = new Matrix2D();
	transform->SetPos(pos);
	
	bActive = true;

	scale.x = scale.y = 1;
	frameX = frameY = 0;

	bound = new BoundingBox(this);
	frequency = 0.07f;
}


GameObject::~GameObject()
{
	Release();
	SafeDelete(transform);

	SafeDelete(shader);
	SafeDelete(worldBuffer);
}

void GameObject::Init()
{
}

void GameObject::Release()
{
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{

	if (Keyboard::Get()->Down(VK_F5))
		bActive = !bActive;

	if (bActive == false) return;

	if (Keyboard::Get()->Press('G'))
		size += D3DXVECTOR2(50, 50) * Time::Delta();
	else if (Keyboard::Get()->Press('F'))
		size += D3DXVECTOR2(-50, -50) * Time::Delta();




	//if (Keyboard::Get()->Press('W'))
	//	pos += D3DXVECTOR2(0, -40.f)*Time::Delta();
	//else if (Keyboard::Get()->Press('S'))
	//	pos += D3DXVECTOR2(0, 40.f)*Time::Delta();
	//if (Keyboard::Get()->Press('D'))
	//	pos += D3DXVECTOR2(40.f, 0)*Time::Delta();
	//else if (Keyboard::Get()->Press('A'))
	//	pos += D3DXVECTOR2(-40.f, 0)*Time::Delta();

	//if (frameY >= 12 && frameX >= 10)
	//{
	//	frameX = 0;
	//	frameY = 0;
	//}



}

void GameObject::PostUpdate()
{

	static float time = 0.f;
	time += Time::Delta();

	if (sprites.size() != 0)
	{
		if (time >= frequency)
		{
			frameX++;
			time -= frequency;
			if (frameX >= sprites[state]->GetMaxFrame().x)
			{
				frameY++;
				frameX = 0;


				if (frameY >= sprites[state]->GetMaxFrame().y)
				{
					frameY = 0;
				}
			}
		}

	}

	transform->SetScale(scale);
	transform->SetPos(pos);
	transform->SetRotate(rotate);

	pos += velocity * Time::Delta();
	velocity += accelerate * Time::Delta();

	bound->Update();
}

//뷰행렬 계산을 하면 TRUE
void GameObject::Render(bool isRelative)
{
	if (bActive == false)return;

	Matrix2D world = *transform;

	if (isRelative)
	{
		world = world * CAMERA->GetView();
	}

	world.Bind();

	if (defaultTexture != nullptr)
	{
		//defaultTexture->Render();
		defaultTexture->FrameRender(frameX, frameY, size, 1.f, pivot);
	}
	bound->Render();
	//p2DRenderer->FillEllipse(rc.GetRect());
	
}

void GameObject::PostRender()
{

	CAMERA->CameraDataBind();
	worldBuffer->Setting(transform->GetResult());
	worldBuffer->SetPSBuffer(1);
	shader->Render();

	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	DeviceContext->IASetInputLayout(nullptr);
	DeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	DeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pRenderer->TurnOnAlphaBlend();
	DeviceContext->Draw(4, 0);
	pRenderer->TurnOffAlphaBlend();

}

void GameObject::SetTextureFilePath(wstring file)
{
	//TODO 리소스관리에서 확인후 있으면 포인터 반환 없으면 생성해서 반환

}

void GameObject::SetTexture(Texture * tex)
{
	if (defaultTexture != nullptr)
	{
		LOG->Warning(__FILE__, __LINE__, "Texture Confirm");
	}
	
	defaultTexture = tex;


}

void GameObject::SetSprite(State state, Texture * tex)
{
	if (sprites[state] != nullptr)
		LOG->Warning(__FILE__, __LINE__, "Sprite Confirm");

	sprites[state] = tex;
	

}
