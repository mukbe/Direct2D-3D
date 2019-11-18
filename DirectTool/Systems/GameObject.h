#pragma once

class Matrix2D;
class GameObject	//메세지 인터페이스 추가
{
public:
	GameObject(string name = "",D3DXVECTOR2 pos = D3DXVECTOR2(0.f,0.f));
	virtual~GameObject();


	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(bool isRelative);
	virtual void PostRender();
	virtual void ImguiRender() {}

	
	Matrix2D* Transform() { return transform; }
	
	const string& Name() { return name; }
	void SetName(const string& s) { name = s; }

	const bool& IsActive() { return bActive; }
	void SetActive(const bool& b) { bActive = b; }

	void SetTextureFilePath(wstring file);
	void SetTexture(Texture* tex);
	//void SetTexture(const string key);

	Texture* GetTexture() { return defaultTexture; }

protected:
	D3DXVECTOR2 pos;
	Matrix2D* transform;
	string name;
	bool bActive;
	Texture* defaultTexture;
	//렌더링 레이어
	//충돌렉트or원
	//데이터 저장 - 할까? 말고 다른곳에서 한번에 할까?


	UINT frameX;
	UINT frameY;

	//test
	FloatRect rc;
	D3DXVECTOR2 size;

private:
	WorldBuffer * worldBuffer;
	Shader* shader;


};

