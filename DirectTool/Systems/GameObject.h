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
	//������ ���̾�
	//�浹��Ʈor��
	//������ ���� - �ұ�? ���� �ٸ������� �ѹ��� �ұ�?


	UINT frameX;
	UINT frameY;

	//test
	FloatRect rc;
	D3DXVECTOR2 size;

private:
	WorldBuffer * worldBuffer;
	Shader* shader;


};

