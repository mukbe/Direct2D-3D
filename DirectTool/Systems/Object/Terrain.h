#pragma once
class Terrain : public GameObject
{
public:
	Terrain(string name = "", D3DXVECTOR2 pos = D3DXVECTOR2(0.f, 0.f), D3DXVECTOR2 size = D3DXVECTOR2(2000.f, 70.f), Pivot p = Pivot::BOTTOM);
	~Terrain();

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
	virtual void Render(bool isRelative);
	//후처리 렌더링 -direct3D
	virtual void PostRender();
	//imgui debugUI render
	virtual void ImguiRender();
private:


};

