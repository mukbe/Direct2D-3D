#pragma once



//3d파이프라인은 바인딩
//2d파이프라인은 랜더링
//텍스쳐를 동시에 로딩하여 srv와 bitmap으로 가지고 있음
//필요시에 CS에 바인딩하여 사용가능하고
//빈 텍스쳐를 생성하여 저장도 가능
class Texture
{
public:

	static void SaveToFile(wstring fileFullPath, ID3D11ShaderResourceView* srv);

private:
	enum ShaderSlot : short
	{
		None = 0,
		VS = 1,
		HS = 1 << 1,
		DS = 1 << 2,
		GS = 1 << 3,
		PS = 1 << 4,
		CS = 1 << 5,
	};
public:
	Texture(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	Texture(wstring file, UINT frameX = 1, UINT frameY = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	//Texture(wstring file, int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~Texture();

	HRESULT SetTexture(wstring file);
	wstring GetFilePath() { return file; }

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	void SetWidth(int val) { width = val; }
	void SetHeight(int val) { height = val; }
	const D3DXVECTOR2& GetFrameSize() { return frameSize; }
	void SetFrameSize(const D3DXVECTOR2& size) { frameSize = size; }
	const D3DXVECTOR2& GetMaxFrame() { return D3DXVECTOR2((float)maxFrameX, (float)maxFrameY); }

	const ID2D1Bitmap* GetBitmap() { return bitmap; }



	DXGI_FORMAT GetFormat() { return format; }

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	ID3D11Texture2D* GetTexture() { return texture; }

	void SetPixel(vector<D3DXCOLOR>& colors, int w, int h);
	void SetPixel32(vector<D3DXCOLOR>& colors, int w, int h);
	void GetPixel(vector<vector<D3DXCOLOR>>& colors);
	void GetPixel(vector<D3DXCOLOR>& colors);
	void GetPixel32(vector<vector<D3DXCOLOR>>& colors);

	void SavePNG(wstring saveFile);
	wstring GetFile()const { return file; }

	//PS에만 바인딩
	void SetShaderResource(UINT slot);
	void SetCSResource(UINT slot);

	//바인딩 해제
	void ReleaseResource();


	void Render(D3DXVECTOR2 size = D3DXVECTOR2(0,0), float alpha = 1.f ,Pivot pivot = Pivot::LEFT_TOP);
	void FrameRender( UINT frameX = 0, UINT frameY = 0, D3DXVECTOR2 size = D3DXVECTOR2(0, 0), float alpha = 1.f, Pivot pivot = Pivot::LEFT_TOP);
	//void AniRender(int x, int y, animation* ani, Pivot pivot = Pivot::LEFT_TOP, bool sizeSet = FALSE);

	//const bool& operator==(const Texture* other);

private:
	void Initialize();
	ID3D11Texture2D* GetReadBuffer();
	const D2D1_RECT_F CalculatePivot(D3DXVECTOR2 size, Pivot pivot);


private:
	wstring file;
	//이미지 파일의 너비높이
	int width;
	int height;
	ID2D1Bitmap* bitmap;
	vector<WICRect>	frameInfo;
	int	maxFrameX;
	int	maxFrameY;
	//프레임 하나의 높이 너비
	D3DXVECTOR2 frameSize;


	DXGI_FORMAT format;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	int lastSlot;
	UINT lastBindFlag;

	int isSlot;
};
