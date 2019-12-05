#pragma once
//회전변환은 생각하지않고 만약 회전이 빈번한 객체라면 원형으로 바운딩을 줄 것



class FloatRect;
class GameObject;
class Bounding
{
public:
	enum class Figure
	{
		Box,Circle
	};
	Bounding(Figure f);
	virtual~Bounding();

	//Pivot은 센터
	void SetGameObject(GameObject* obj, D3DXVECTOR2 offset );
	
	void Update();
	void Render();
	void SetOffset(D3DXVECTOR2 offset);
	FloatRect GetRect();

protected:
	Figure figure;
	GameObject* obj;
	FloatRect rc;
	FloatRect renderRc;
	D3DXVECTOR2 offset;
};

class BoundingBox : public Bounding
{
public:
	BoundingBox(GameObject* obj, D3DXVECTOR2 offset = D3DXVECTOR2(0.f, 0.f)) : Bounding(Figure::Box)
	{
		SetGameObject(obj, offset);
	}
	~BoundingBox() {}
};

class BoundingCircle : public Bounding
{
public:
	BoundingCircle(GameObject* obj, D3DXVECTOR2 offset = D3DXVECTOR2(0.f, 0.f)) : Bounding(Figure::Circle)
	{
		SetGameObject(obj, offset);
	}
	~BoundingCircle() {}
	
};