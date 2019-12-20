#pragma once
#include "GameObject.h"

struct Music
{
	string Name;
	Texture* TitleImage;
	Music() {}
	Music(string name, Texture* image)
		:Name(name), TitleImage(image) {}
};

class Player : public  GameObject
{
public:
	Player(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, ObjectType type, Pivot p = Pivot::CENTER);
	~Player();

	//�ʱ�ȭ
	virtual void Init();
	//�޸� ����
	virtual void Release();
	//���� ��ƾ
	virtual void Update(float tick);

	virtual void ImguiRender();
private:
	vector<Music*> files;
	int select;
	int current;
	bool bPlay;
	float soundValue;


	Music* currentMusic;
	function<void(wstring)> addSoundFunc;


};

