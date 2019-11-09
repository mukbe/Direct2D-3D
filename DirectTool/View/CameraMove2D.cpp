#include "stdafx.h"
#include "CameraMove2D.h"

#include "OrthoWindow.h"

CameraMove2D::CameraMove2D()
{
	nowSpeed = 3.0f;
	maxSpeed = 50.0f;
}


CameraMove2D::~CameraMove2D()
{
}

void CameraMove2D::Init()
{
}

void CameraMove2D::Releaes()
{
}

void CameraMove2D::Update()
{
	this->ControlCamera();
	this->UpdateView();

	CameraBase::Update();
}

void CameraMove2D::Render()
{
	CameraBase::Render();
}

void CameraMove2D::UIRender()
{
	ImGui::Text("Position : %f , %f , %f", pos.x, pos.y, pos.z);
}

void CameraMove2D::ControlCamera()
{
	if (Keyboard::Get()->Down(VK_RBUTTON))
	{
		int screenCenterX = WinSizeX / 2;
		int screenCenterY = WinSizeY / 2;

		Mouse::Get()->SetMousePos(screenCenterX, screenCenterY);
	}
	else if (Keyboard::Get()->Press(VK_RBUTTON))
	{
		float speed = 300.0f;
		D3DXVECTOR3 inputVector(0, 0, 0);

		maxSpeed = 50.0f;
		if (KeyCode->Press(VK_LSHIFT))
		{
			maxSpeed = 200.0f;
		}

		if (KeyCode->Press(VK_SPACE))
		{
			maxSpeed = 1000.f;
		}

		if (KeyCode->Press('W'))
			inputVector.y = speed;
		else if (KeyCode->Press('S'))
			inputVector.y = -speed;
		if (KeyCode->Press('A'))
			inputVector.x = -speed;
		else if (KeyCode->Press('D'))
			inputVector.x = speed;
		if (KeyCode->Press('E'))
			inputVector.z = speed;
		else if (KeyCode->Press('Q'))
			inputVector.z = -speed;

		if (VECTORZERO(inputVector) == false)
			D3DXVec3Normalize(&inputVector, &inputVector);

		D3DXVECTOR3 target = inputVector * maxSpeed;
		pos += target * DeltaTime;
		int screenCenterX = WinSizeX / 2;
		int screenCenterY = WinSizeY / 2;

		POINT mousePos;
		GetCursorPos(&mousePos);

		D3DDesc desc;
		DxRenderer::GetDesc(&desc);

		ScreenToClient(desc.Handle, &mousePos);

		float deltaX = (float)mousePos.x - (float)screenCenterX;
		float deltaY = (float)mousePos.y - (float)screenCenterY;


		GameMouse->SetMousePos(screenCenterX, screenCenterY);

	}
}

void CameraMove2D::UpdateView()
{
	D3DXMatrixLookAtLH(&matView, &pos, &(pos + D3DXVECTOR3(0.f,0.f,1.f)), &D3DXVECTOR3(0.f,1.f,0.f));
	ortho->SetView(matView);

}
