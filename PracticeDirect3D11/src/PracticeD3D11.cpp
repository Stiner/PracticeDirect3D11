// PracticeD3D11

#include "PCH.h"
#include "PracticeD3D11.h"

#include "MeshRendererObject.h"
#include "ObjectManager.h"
#include "CameraObject.h"

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow)
{
    return CPracticeD3D11(hInstance, TEXT("PracticeD3D11")).Run(lpCmdLine);
}

CPracticeD3D11::CPracticeD3D11(HINSTANCE hAppInstance, const TCHAR* szAppName)
    : CD3D11App(hAppInstance, szAppName)
{
    _ClientWidth = 1000;
    _ClientHeight = 1000;
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
    bool r = __super::Initialize();

    _CameraObject.reset(new CameraObject());
	_CameraObject->Initialize(_D3DDevice);
	_CameraObject->SetAspectRatio(GetAspectRatio());
    _CameraObject->SetPosition(0, 0, -1.f);

    _ObjectManager.reset(new ObjectManager());
    MeshRendererObject* newObject = _ObjectManager->Create(_D3DDevice);
    newObject->SetPosition(0.0f, 0.0f, 0.0f);
    newObject->SetScale(0.25f, 0.25f, 0.25f);

    return r;
}

void CPracticeD3D11::Release()
{
    _ObjectManager->RemoveAll();
    _ObjectManager.release();

    _CameraObject->Release();
	_CameraObject.release();

    __super::Release();
}

bool CPracticeD3D11::OnResize()
{
    return __super::OnResize();
}

void CPracticeD3D11::UpdateScene(float DeltaTime)
{
    _CameraObject->Update(DeltaTime);

    for (auto& Object : _ObjectManager->GetContainer())
    {
        Object->Update(DeltaTime);
    }
}

void CPracticeD3D11::DrawScene()
{
    _CameraObject->Draw(_D3DDeviceContext);

    for (auto& Object : _ObjectManager->GetContainer())
    {
        Object->Draw(_D3DDeviceContext);
    }
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
