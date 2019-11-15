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

	

protected:
	Matrix2D* transform;
	string name;
	bool bActive;

	//렌더링 레이어
	//충돌렉트or원
	//데이터 저장 - 할까? 말고 다른곳에서 한번에 할까?


	//test
	FloatRect rc;


private:
//	class PositionBuffer : public ShaderBuffer
//	{
//	private:
//		struct Struct
//		{
//			D3DXVECTOR2 Pos;
//			D3DXVECTOR2 padding;
//		}data;
//	public:
//		PositionBuffer()
//			:ShaderBuffer(&data, sizeof Struct)
//		{
//
//		}
//		void SetPosition(D3DXVECTOR2 p) { data.Pos = p; }
//	};
//private:
//	PositionBuffer *buffer;
	WorldBuffer * worldBuffer;
	Shader* shader;


};

