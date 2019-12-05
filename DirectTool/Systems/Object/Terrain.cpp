#include "stdafx.h"
#include "Terrain.h"





Terrain::Terrain(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, Pivot p)
	:GameObject(name, pos, size, p)
{

}

Terrain::~Terrain()
{
}

void Terrain::Init()
{
}

void Terrain::Release()
{
}

void Terrain::PreUpdate()
{
}

void Terrain::Update(float tick)
{
}

void Terrain::PostUpdate()
{
}

void Terrain::Render(bool isRelative)
{
	GameObject::Render(isRelative);
}

void Terrain::PostRender()
{
}

void Terrain::ImguiRender()
{
}
