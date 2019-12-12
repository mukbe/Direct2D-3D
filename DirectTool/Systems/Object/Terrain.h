#pragma once
class Terrain : public GameObject
{
public:
	Terrain(string name , D3DXVECTOR2 pos , D3DXVECTOR2 size , ObjectType type,  Pivot p = Pivot::BOTTOM);
	~Terrain();

	//�ʱ�ȭ
	virtual void Init();
	//�޸� ����
	virtual void Release();
	//ī�޶� ���� ���ʾ�����Ʈ - ��� ���صε�
	virtual void PreUpdate();
	//���� ��ƾ
	virtual void Update(float tick);
	//�̵����� ���
	virtual void PostUpdate();

	//�̹��� ������ - direct2D
	virtual void Render(bool isRelative);
	//��ó�� ������ -direct3D
	virtual void PostRender();
	//imgui debugUI render
	virtual void ImguiRender();
private:


};

