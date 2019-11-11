#pragma once

class SceneMain;
class Program
{

	class PositionBuffer : public ShaderBuffer
	{
	private:
		struct Struct
		{
			D3DXVECTOR2 Pos;
			D3DXVECTOR2 padding;
		}data;
	public:
		PositionBuffer()
			:ShaderBuffer(&data, sizeof Struct)
		{
			
		}
		void SetPosition(D3DXVECTOR2 p) { data.Pos = p; }
		
	};

public:
	Program();
	~Program();

	void PreUpdate();
	void Update(float tick);
	void PostUpdate();

	void Render();

	void PostRender();
	void ImguiRender();

	void ResizeScreen();
private:
	//class Json::Value* jsonRoot;
	//class DeferredRenderer* deferred;
	//class ShadowRenderer* shadow;
	class SceneMain *sm;

	class CameraMove2D* camera;
	Shader* shader;
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	WorldBuffer* worldBuffer;

	FloatRect play;
	PositionBuffer *buffer;
	float angle;

};