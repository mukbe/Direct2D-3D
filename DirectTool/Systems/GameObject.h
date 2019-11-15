#pragma once

class Matrix2D;
class GameObject	//�޼��� �������̽� �߰�
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

	//������ ���̾�
	//�浹��Ʈor��
	//������ ���� - �ұ�? ���� �ٸ������� �ѹ��� �ұ�?


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

