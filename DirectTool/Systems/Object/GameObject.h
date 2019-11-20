#pragma once

class Matrix2D;
class GameObject	//메세지 인터페이스 추가
{
public:
	GameObject(string name = "",D3DXVECTOR2 pos = D3DXVECTOR2(0.f,0.f),D3DXVECTOR2 size = D3DXVECTOR2(100.f,100.f),Pivot p = Pivot::BOTTOM);
	virtual~GameObject();

	//초기화
	virtual void Init();
	//메모리 해제
	virtual void Release();
	//카메라에 관한 최초업데이트 - 사용 안해두됨
	virtual void PreUpdate();
	//메인 루틴
	virtual void Update();
	//이동관련 계산
	virtual void PostUpdate();

	//이미지 렌더링 - direct2D
	virtual void Render(bool isRelative);
	//후처리 렌더링 -direct3D
	virtual void PostRender();
	//imgui debugUI render
	virtual void ImguiRender() {}

	//위치관련 함수들
	Matrix2D* Transform() { return transform; }
	void SetPos(const D3DXVECTOR2& p) 
	{
		pos = p; 
		transform->SetPos(pos);
	}
	const D3DXVECTOR2 GetPos() { return pos; }
	void SetVelocity(const D3DXVECTOR2& v) { velocity = v; }
	const D3DXVECTOR2& GetVelocity() { return velocity; }
	void SetRotate(const float& degree)
	{
		rotate = degree;
		transform->SetRotate(rotate);
	}
	const float& GetRotate() { return rotate; }

	
	const string& Name() { return name; }
	void SetName(const string& s) { name = s; }

	const bool& IsActive() { return bActive; }
	void SetActive(const bool& b) { bActive = b; }

	void SetAlpha(const float& a)
	{
		a > 0.f ? (a > 1.f ? alpha = 1.f : alpha = a) : alpha = 0.f;
	}
	const float& GetAlpha() { return alpha; }

	void SetSize(const D3DXVECTOR2& s) 
	{
		size = s;
	}
	const D3DXVECTOR2& GetSize() { return size; }

	void SetPivot(const Pivot& p) { pivot = p; }
	const Pivot& GetPivot() { return pivot; }


	void SetTextureFilePath(wstring file);
	void SetTexture(Texture* tex);
	//void SetTexture(const string key);

	Texture* GetTexture() { return defaultTexture; }

protected:
	string name;
	Matrix2D* transform;
	bool bActive;
	float alpha;
	//랜더링이나 바운딩 박스의 크기 scale말고 이걸 사용하기 바람
	D3DXVECTOR2 size;
	Pivot pivot;

	Texture* defaultTexture;
	UINT frameX;
	UINT frameY;

	//이놈의 좌표
	D3DXVECTOR2 pos;
	//이놈의 스캐일
	D3DXVECTOR2 scale;
	//회전스캐일 Degree
	float rotate;
	//이놈의 이동속도
	D3DXVECTOR2 velocity;
	//이놈의 가속도
	D3DXVECTOR2 accelerate;


	//렌더링 레이어
	//충돌렉트or원
	//데이터 저장 - 할까? 말고 다른곳에서 한번에 할까?



	//test
	class Bounding* bound;

private:
	WorldBuffer * worldBuffer;
	Shader* shader;


};

