#include "PCH.h"
#include "PracticeD3D11.h"
#include "GeometryObject.h"
#include <DirectXMath.h>
#include <vector>

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow)
{
    return CPracticeD3D11(hInstance, TEXT("PracticeD3D11")).Run(lpCmdLine);
}

class ListGeometry
{
public:
    ListGeometry()
    {
    }

    ~ListGeometry()
    {
    }

    void Add(GeometryObject* GeometryObject)
    {
    }

    void Update()
    {
    }

protected:
    std::vector<std::unique_ptr<GeometryObject>> _list;
};

CPracticeD3D11::CPracticeD3D11(HINSTANCE hAppInstance, const TCHAR* szAppName)
    : CD3D11App(hAppInstance, szAppName)
{
    _ClientWidth = 1024;
    _ClientHeight = 768;
}

CPracticeD3D11::~CPracticeD3D11()
{
}

bool CPracticeD3D11::ProccessCmdLine(const TCHAR* szCmdLine)
{
    return __super::ProccessCmdLine(szCmdLine);
}

bool CPracticeD3D11::Initialize()
{
    _listGeometry.reset(new ListGeometry());

    return __super::Initialize();
}

void CPracticeD3D11::Release()
{
    __super::Release();

    _listGeometry.release();
}

bool CPracticeD3D11::OnResize()
{
    return __super::OnResize();
}

void CPracticeD3D11::UpdateScene(float dt)
{
    _listGeometry->Update();

    using namespace DirectX;

    XMVECTOR vecEyePos = { 0, 0, -10, 0 };
    XMVECTOR vecLookAt = { 0, 0, 0, 0 };
    XMVECTOR vecEyeUp = { 0, 1, 0, 0 };
    XMMATRIX matView = XMMatrixLookAtLH(vecEyePos, vecLookAt, vecEyeUp);

    XMMATRIX matProjection = XMMatrixPerspectiveFovLH(90.f, GetAspectRatio(), 1.f, 10.f);
}

void CPracticeD3D11::DrawScene()
{
}

LRESULT CPracticeD3D11::MsgProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
    return __super::MsgProc(hwnd, msg, wParam, lParam);
}

void CPracticeD3D11::OnMouseDown(WPARAM btnState, int x, int y)
{
    __super::OnMouseDown(btnState, x, y);
}

void CPracticeD3D11::OnMouseUp(WPARAM btnState, int x, int y)
{
    __super::OnMouseUp(btnState, x, y);
}

void CPracticeD3D11::OnMouseMove(WPARAM btnState, int x, int y)
{
    __super::OnMouseMove(btnState, x, y);
}
