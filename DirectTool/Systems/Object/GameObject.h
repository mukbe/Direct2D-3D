#pragma once

class Matrix2D;
class Bounding;
class GameObject	//메세지 인터페이스 추가
{
public:
	enum State
	{
		Idle = 0, Run, Jump
	};

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
	virtual void Update(float tick);
	//이동관련 계산
	virtual void PostUpdate();

	//이미지 렌더링 - direct2D
	virtual void Render(bool isRelative = true);
	//후처리 렌더링 -direct3D
	virtual void PostRender();
	//imgui debugUI render
	virtual void ImguiRender() {}

	//위치관련 함수들
	Matrix2D* Transform() { return transform; }

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

	void SetTextureFilePath(wstring file);
	void SetTexture(Texture* tex);
	//void SetTexture(const string key);
	Texture* GetTexture() { return defaultTexture; }
	void SetSprite(State state, Texture* tex);

	Bounding* GetBounding() { return bound; }

	void SetObjectType(ObjectType t) { objectType = t; }
	ObjectType GetObjectType() { return objectType; }
	void SetVelocity(const D3DXVECTOR2& v) { velocity = v; }
	const D3DXVECTOR2& GetVelocity() { return velocity; }
	void SetSpeed()
	{
		velocity = { 0,0 };
		accelerate = { 0,0 };
	}

protected:
	string name;
	ObjectType objectType;
	Matrix2D* transform;
	bool bActive;
	float alpha;
	float lifeTiem;
	float frameTime;


	UINT frameX;
	UINT frameY;
	float frequency;

	D3DXVECTOR2 size;

	Texture* defaultTexture;
	unordered_map<State, Texture*> sprites;
	State state;

	//이놈의 이동속도
	D3DXVECTOR2 velocity;
	//이놈의 가속도
	D3DXVECTOR2 accelerate;
	D3DXVECTOR2 gravity;


	//렌더링 레이어
	//충돌렉트or원
	//데이터 저장 - 할까? 말고 다른곳에서 한번에 할까?



	//test
	class Bounding* bound;

private:
	WorldBuffer * worldBuffer;
	Shader* shader;


};

