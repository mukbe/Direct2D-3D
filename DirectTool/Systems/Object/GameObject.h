#pragma once

class Matrix2D;
class GameObject	//�޼��� �������̽� �߰�
{
public:
	GameObject(string name = "",D3DXVECTOR2 pos = D3DXVECTOR2(0.f,0.f),D3DXVECTOR2 size = D3DXVECTOR2(100.f,100.f),Pivot p = Pivot::BOTTOM);
	virtual~GameObject();

	//�ʱ�ȭ
	virtual void Init();
	//�޸� ����
	virtual void Release();
	//ī�޶� ���� ���ʾ�����Ʈ - ��� ���صε�
	virtual void PreUpdate();
	//���� ��ƾ
	virtual void Update();
	//�̵����� ���
	virtual void PostUpdate();

	//�̹��� ������ - direct2D
	virtual void Render(bool isRelative);
	//��ó�� ������ -direct3D
	virtual void PostRender();
	//imgui debugUI render
	virtual void ImguiRender() {}

	//��ġ���� �Լ���
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
	//�������̳� �ٿ�� �ڽ��� ũ�� scale���� �̰� ����ϱ� �ٶ�
	D3DXVECTOR2 size;
	Pivot pivot;

	Texture* defaultTexture;
	UINT frameX;
	UINT frameY;

	//�̳��� ��ǥ
	D3DXVECTOR2 pos;
	//�̳��� ��ĳ��
	D3DXVECTOR2 scale;
	//ȸ����ĳ�� Degree
	float rotate;
	//�̳��� �̵��ӵ�
	D3DXVECTOR2 velocity;
	//�̳��� ���ӵ�
	D3DXVECTOR2 accelerate;


	//������ ���̾�
	//�浹��Ʈor��
	//������ ���� - �ұ�? ���� �ٸ������� �ѹ��� �ұ�?



	//test
	class Bounding* bound;

private:
	WorldBuffer * worldBuffer;
	Shader* shader;


};

