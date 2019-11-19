#include "stdafx.h"
#include "CameraManager.h"
#include "./Utilities/Matrix2D.h"
#include "./Render/ShaderBuffer.h"

SingletonCpp(CameraManager)

CameraManager::CameraManager()
{

	pos = D3DXVECTOR2(0.f,0.f);
	zoom = 1.f;
	view = Matrix2D();
	buffer = new CameraBuffer;

	UpdateMatrix();
	UpdateRenderRect();
}


CameraManager::~CameraManager()
{
	SafeDelete(buffer);
}

void CameraManager::Update()
{
	if (Mouse::Get()->Down(0))
	{
		memcpy(&picked, Mouse::Get()->GetPosition(), sizeof(D3DXVECTOR2));

		while (ShowCursor(FALSE) >= 0);
	}
	if (Mouse::Get()->Up(0))
	{
		while (ShowCursor(TRUE) <= 0);
		ClipCursor(NULL);
	}

	if (Mouse::Get()->Press(0))
	{
		
		ClipMouse();

		memcpy(&pick, Mouse::Get()->GetPosition(), sizeof(D3DXVECTOR2));

		D3DXVECTOR2 delta = picked - pick;
		pos += delta * zoom;

		memcpy(&picked, Mouse::Get()->GetPosition(), sizeof(D3DXVECTOR2));

		UpdateMatrix();
	}
}

void CameraManager::ImguiRender()
{
	ImGui::Begin("Camera");
	ImGui::Text("FPS : %f", Time::Get()->FPS());
	ImGui::Text("Tick : %f", Time::Delta());
	ImGui::Text("PosX : %.2f, PosY : %.2f", pos.x, pos.y);
	ImGui::End();
}

void CameraManager::UpdateRenderRect()
{
	renderRect = { (LONG)pos.x,(LONG)pos.y, (LONG)(pos.x + WinSizeX) , (LONG)(pos.y + WinSizeY) };
}

void CameraManager::AddZoom(float value)
{

	zoom += value;

	if (zoom < ZOOM_MIN) zoom = ZOOM_MIN;
	if (zoom > ZOOM_MAX) zoom = ZOOM_MAX;

	UpdateMatrix();
}

RECT CameraManager::GetRelativeRECT(RECT rc)
{
	RECT temp = rc;
	MoveRect(&temp, -pos.x, pos.y);
	return std::move(temp);
}

POINT CameraManager::GetRelativePOINT(POINT pt)
{
	pt.x -= pos.x;
	pt.y -= pos.y;

	return std::move(pt);
}

D3DXVECTOR2 CameraManager::GetRelativeVector2D(D3DXVECTOR2 vr)
{
	vr.x -= pos.x;
	vr.y -= pos.y;
	vr *= zoom;
	return std::move(vr);
}


POINT CameraManager::GetMousePos()
{
	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();
	POINT pt = { mouse.x / zoom + pos.x, mouse.y / zoom + pos.y };
	return POINT();
}

BOOL CameraManager::IsCollision(D3DXVECTOR2 p)
{
	FloatRect rc(p.x, p.y, WinSizeX / zoom, WinSizeY / zoom);

	if (rc.left <= p.x &&
		rc.right >= p.x &&
		rc.top <= p.y &&
		rc.bottom >= p.y)
	{
		return true;
	}
	return false;
}

void CameraManager::CameraDataBind()
{
	//���̴����� ����� ī�޶��� ����� ���ε��ϸ� ���̴��� �׻� ��


	buffer->Setting(view.GetResult());
	buffer->SetPSBuffer(0);
	buffer->SetVSBuffer(0);

}

void CameraManager::UpdateMatrix()
{
	view.SetPos(-pos);
	view.SetScale(zoom);

}

void CameraManager::ClipMouse()
{
	RECT cR;
	D3DDesc desc;
	DxRenderer::GetDesc(&desc);
	POINT p1, p2;
	POINT mouseScreenPos;

	GetCursorPos(&mouseScreenPos);

	GetClientRect(desc.Handle, &cR);
	memcpy(&p1, &cR.left, sizeof(POINT));
	memcpy(&p2, &cR.right, sizeof(POINT));
	ClientToScreen(desc.Handle, &p1);
	ClientToScreen(desc.Handle, &p2);
	memcpy(&cR.left, &p1, sizeof(POINT));
	memcpy(&cR.right, &p2, sizeof(POINT));

	if (PtInRect(&cR, mouseScreenPos))
		ClipCursor(&cR);
}
