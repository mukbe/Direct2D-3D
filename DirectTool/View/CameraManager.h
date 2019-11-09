#pragma once

#define ZOOM_MAX 2.5f
#define ZOOM_MIN 0.5f

class CameraManager
{
	SingletonHeader(CameraManager)
public:

	void Update();
	RECT GetRenderRect() { return renderRect; }
	void UpdateRenderRect();

	D3DXVECTOR2 GetPos() { return pos; }
	void SetPos(D3DXVECTOR2 p)
	{
		pos = p;
		UpdateRenderRect();
	}

	void SetZoom(float z)
	{
		zoom = z;
		if (zoom < ZOOM_MIN) zoom = ZOOM_MIN;
		if (zoom > ZOOM_MAX) zoom = ZOOM_MAX;
	}
	float GetZoom() { return zoom; }

	RECT GetRelativeRECT(RECT rc);
	POINT GetRelativePOINT(POINT pt);
	D3DXVECTOR2 GetRelativeVector2D(D3DXVECTOR2 vr);
	D3DXVECTOR2 GetZoomPos(D3DXVECTOR2 vr);

	POINT GetMousePos();

	BOOL IsCollision(D3DXVECTOR2 p);

private:
	D3DXVECTOR2 pos;
	RECT renderRect;
	float zoom;
	
	D3DXVECTOR2 pick;
	D3DXVECTOR2 picked;

};

#define CAMERA CameraManager::Get()