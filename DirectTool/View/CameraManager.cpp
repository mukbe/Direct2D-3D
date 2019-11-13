#include "stdafx.h"
#include "CameraManager.h"
#include "./Utilities/Matrix2D.h"


SingletonCpp(CameraManager)

CameraManager::CameraManager()
{
	//pos = D3DXVECTOR2(-WinSizeX/2 ,-WinSizeY/2);
	pos = D3DXVECTOR2(0.f,0.f);
	zoom = 1.f;
	view = Matrix2D();
	UpdateMatrix();
	UpdateRenderRect();
}


CameraManager::~CameraManager()
{
}

void CameraManager::Update()
{
	if (Mouse::Get()->Down(0))
	{
		memcpy(&picked, Mouse::Get()->GetPosition(), sizeof(D3DXVECTOR2));
		//TODO 이렇게 해도 되는가 피드백필요
		//while (ShowCursor(FALSE) >= 0);

	
	}
	if (Mouse::Get()->Up(0))
	{
		//while (ShowCursor(TRUE) <= 0);
		ClipCursor(NULL);
	}

	if (Mouse::Get()->Press(0))
	{
		
		RECT cR;
		D3DDesc desc;
		DxRenderer::GetDesc(&desc);
		POINT p1, p2;

		GetClientRect(desc.Handle, &cR);
		memcpy(&p1, &cR.left, sizeof(POINT));
		memcpy(&p2, &cR.right, sizeof(POINT));
		ClientToScreen(desc.Handle, &p1);
		ClientToScreen(desc.Handle, &p2);
		memcpy(&cR.left, &p1, sizeof(POINT));
		memcpy(&cR.right, &p2, sizeof(POINT));
		ClipCursor(&cR);

		memcpy(&pick, Mouse::Get()->GetPosition(), sizeof(D3DXVECTOR2));

		D3DXVECTOR2 delta = picked - pick;
		pos += delta;

		memcpy(&picked, Mouse::Get()->GetPosition(), sizeof(D3DXVECTOR2));

		UpdateMatrix();
	}
}

void CameraManager::UpdateRenderRect()
{
	renderRect = { (LONG)pos.x,(LONG)pos.y, (LONG)(pos.x + WinSizeX) , (LONG)(pos.y + WinSizeY) };
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

D3DXVECTOR2 CameraManager::GetZoomPos(D3DXVECTOR2 vr)
{
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

void CameraManager::UpdateMatrix()
{
	view.SetPos(-pos);
	view.SetScale(zoom);
}
