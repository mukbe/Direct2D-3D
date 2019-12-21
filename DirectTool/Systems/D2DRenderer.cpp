#include "stdafx.h"
#include "D2DRenderer.h"


SingletonCpp(D2DRenderer)

D2DRenderer::D2DRenderer()
{
}


D2DRenderer::~D2DRenderer()
{
}

void D2DRenderer::Create2DBuffer()
{
	HRESULT hr;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);

	//WIC 팩토리 생성
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	IDXGISurface* dxgiBuffer;
	hr = SwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBuffer));
	
	FLOAT dpiX, dpiY;
	d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT
		, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
	
	hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgiBuffer, prop, &d2dRenderTarget);

	IDXGIDevice * dxgiDevice;

	HResult(Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));


	d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);

	SafeRelease(dxgiBuffer);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwFactory), (IUnknown**)(&dwFactory));
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &dwDefaultBrush[DefaultBrush::black]);
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &dwDefaultBrush[DefaultBrush::white]);
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &dwDefaultBrush[DefaultBrush::yello]);
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 1.0f), &dwDefaultBrush[DefaultBrush::red]);
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 1.0f), &dwDefaultBrush[DefaultBrush::blue]);
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &dwDefaultBrush[DefaultBrush::green]);
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &dwDefaultBrush[DefaultBrush::gray]);


	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &dwDefaultBrush[DefaultBrush::white]);

	AddTextFormat(L"맑은고딕", 20);

}

void D2DRenderer::BeginDraw()
{
	//d2d
	d2dRenderTarget->BeginDraw();
	D2D1_COLOR_F temp;
	temp.r = temp.g = temp.b = 1.f;
	temp.a = 1.0f;
	d2dRenderTarget->Clear(temp);
	d2dRenderTarget->SetTransform(D2D1::IdentityMatrix());
}

void D2DRenderer::EndDraw()
{
	d2dRenderTarget->EndDraw();
}

HRESULT D2DRenderer::AddTextFormat(wstring fontname, float size)
{
	HRESULT hr;
	IDWriteTextFormat* format = NULL;

	hr = dwFactory->CreateTextFormat(
		fontname.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		&format
	);

	this->dwTextFormats.insert(make_pair(fontname, format));

	if (FAILED(hr))
		return hr;

	return hr;
}

ID2D1Bitmap * D2DRenderer::CreateD2DBitmapFromFile(wstring file)
{

	//1. 디코더를 생성한다.
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	if (FAILED(wicFactory->CreateDecoderFromFilename(file.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &ipDecoderPtr)))
	{
		LOG->Error(__FILE__, __LINE__, "Does not exist bitmapfile");
	}

	//2. 디코더에서 프레임을 얻는다.
	IWICBitmapFrameDecode*	ipFramePtr = nullptr;
	ipDecoderPtr->GetFrame(0, &ipFramePtr);

	//이미지 로드에 사용
	IWICFormatConverter* convertedSrcBmp;

	//3. 프레임을 기반으로 포맷 컨버터를 만든다.
	wicFactory->CreateFormatConverter(&convertedSrcBmp);

	convertedSrcBmp->Initialize(ipFramePtr,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);

	//4. 컨버트된 데이터를 기반으로 실제 Direct2D용 비트맵을 만든다.
	ID2D1Bitmap* ipResult = nullptr;
	d2dRenderTarget->CreateBitmapFromWicBitmap(convertedSrcBmp,
		nullptr,
		&ipResult);

	SafeRelease(convertedSrcBmp);
	SafeRelease(ipDecoderPtr);
	SafeRelease(ipFramePtr);

	return ipResult;
}


void D2DRenderer::DrawText2D(int x, int y, wstring text, int size, DefaultBrush::Enum defaultBrush, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	//텍스트 레이아웃 생성
	
	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormats[font],
		text.length()*size,
		size,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize(size, range);

	dwLayout->SetTextAlignment(align);
	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(x, y), dwLayout, dwDefaultBrush[defaultBrush]);

	dwLayout->Release();
}

void D2DRenderer::DrawText2D(int x, int y, wstring text, COLORREF rgb, float alpha, int size, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	//텍스트 레이아웃 생성
	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormats[font],
		text.length()*size,
		size,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize(size, range);
	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	//브러쉬 생성
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(rgb, alpha), &brush);

	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(x, y), dwLayout, brush);
	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	brush->Release();
	dwLayout->Release();
}

void D2DRenderer::DrawTextField(int x, int y, wstring text, int size, int width, int height, DefaultBrush::Enum defaultBrush, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormats[font],
		width,
		height,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize(size, range);

	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(x, y), dwLayout, dwDefaultBrush[defaultBrush]);
	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	dwLayout->Release();
}

void D2DRenderer::DrawTextField(int x, int y, wstring text, COLORREF rgb, int size, int width, int height, float alpha, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormats[font],
		width,
		height,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize(size, range);

	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	//브러쉬 생성
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(rgb, alpha), &brush);

	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(x, y), dwLayout, brush);
	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	brush->Release();
	dwLayout->Release();
}

//============================================================================
// ## 선, 도형 렌더링 ##
//============================================================================

void D2DRenderer::DrawLine(D2D1::ColorF::Enum color, float alpha, D3DXVECTOR2 start, D3DXVECTOR2 end, bool isRelativePos, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);



	d2dRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), brush, strokeWidth);


	brush->Release();
}
void D2DRenderer::DrawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	d2dRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), dwDefaultBrush[defaultBrush], strokeWidth);
}

void D2DRenderer::DrawRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);


	d2dRenderTarget->DrawRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), brush, strokeWidth);

	brush->Release();
}
void D2DRenderer::DrawRectangle(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	d2dRenderTarget->DrawRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), dwDefaultBrush[defaultBrush], strokeWidth);
}

void D2DRenderer::DrawEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);



	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	brush->Release();
}

void D2DRenderer::DrawEllipse(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->DrawEllipse(&ellipse, dwDefaultBrush[defaultBrush], strokeWidth);
}

void D2DRenderer::FillRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);


	d2dRenderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), brush);

	brush->Release();
}

//테스트한거
void D2DRenderer::FillRectangle(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	d2dRenderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), dwDefaultBrush[defaultBrush]);

}

void D2DRenderer::FillEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);


	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->FillEllipse(&ellipse, brush);

	brush->Release();
}
void D2DRenderer::FillEllipse(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->FillEllipse(&ellipse, dwDefaultBrush[defaultBrush]);
}

void D2DRenderer::DrawRectangle(FloatRect rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);


	d2dRenderTarget->DrawRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), brush, strokeWidth);

	brush->Release();
}

void D2DRenderer::DrawRectangle(FloatRect rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	d2dRenderTarget->DrawRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), dwDefaultBrush[defaultBrush], strokeWidth);
}

void D2DRenderer::DrawEllipse(FloatRect rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);



	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	brush->Release();
}

void D2DRenderer::DrawEllipse(FloatRect rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->DrawEllipse(&ellipse, dwDefaultBrush[defaultBrush], strokeWidth);
}

void D2DRenderer::FillRectangle(FloatRect rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);


	d2dRenderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), brush);

	brush->Release();
}

void D2DRenderer::FillRectangle(FloatRect rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	d2dRenderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), dwDefaultBrush[defaultBrush]);

}

void D2DRenderer::FillEllipse(FloatRect rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);


	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->FillEllipse(&ellipse, brush);

	brush->Release();
}

void D2DRenderer::FillEllipse(FloatRect rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	d2dRenderTarget->FillEllipse(&ellipse, dwDefaultBrush[defaultBrush]);
}
