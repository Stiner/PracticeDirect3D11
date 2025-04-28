// PracticeD3D11

#include "PCH.h"
#include "CPracticeD3D11.h"

#include "CD3DDevice.h"
#include "CMeshRendererObject.h"
#include "CCameraObject.h"
#include "CMeshCube.h"
#include "CMeshCubeTex.h"
#include "CMaterial.h"

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

HRESULT CPracticeD3D11::ProccessCmdLine(const TCHAR* szCmdLine)
{
    R_CHECK(__super::ProccessCmdLine(szCmdLine));

    return S_OK;
}

HRESULT CPracticeD3D11::Initialize()
{
    R_CHECK(__super::Initialize());
    R_CHECK(BuildScene());

    return S_OK;
}

void CPracticeD3D11::Release()
{
#define REMOVE_ALL(_expr_) { for (const auto e : _expr_) { if (e) e->Release(); } _expr_.clear(); }
    REMOVE_ALL(_ListMeshRendererObject);
    REMOVE_ALL(_ListMaterial);
    REMOVE_ALL(_ListMesh);
#undef REMOVE_ALL

    _CameraObject->Release();
    MEM_DELETE(_CameraObject);

    __super::Release();
}

HRESULT CPracticeD3D11::OnResize()
{
    R_CHECK(__super::OnResize());
}

void CPracticeD3D11::UpdateScene(float DeltaTime)
{
    _CameraObject->Update(DeltaTime);

    for (CMeshRendererObject* Object : _ListMeshRendererObject)
    {
        if (Object != nullptr) Object->Update(DeltaTime);
    }
}

void CPracticeD3D11::DrawScene()
{
    _CameraObject->Draw();

    for (CMeshRendererObject* Object : _ListMeshRendererObject)
    {
        if (Object != nullptr) Object->Draw();
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

HRESULT CPracticeD3D11::BuildScene()
{
    _CameraObject = new CCameraObject();
    _CameraObject->Initialize();

    _CameraObject->SetAspectRatio(GetAspectRatio());
    _CameraObject->SetPosition(0, 0, -5.f);
    _CameraObject->SetRotation(0, 0, 0);

    CMesh* newMesh = nullptr;
    CMaterial* newMaterial = nullptr;
    CMeshRendererObject* newObject = nullptr;

    {
        newMesh = new CMeshCube();
        newMesh->Initialize();
        _ListMesh.push_back(newMesh);

        newMaterial = new CMaterial();
        newMaterial->Initialize();
        _ListMaterial.push_back(newMaterial);

        newObject = new CMeshRendererObject();
        newObject->Initialize(newMesh, newMaterial);
        _ListMeshRendererObject.push_back(newObject);

        newObject->SetPosition(2, 0, 0);
    }
    {
        newMesh = new CMeshCube();
        newMesh->Initialize();
        _ListMesh.push_back(newMesh);

        newMaterial = new CMaterial();
        newMaterial->Initialize();
        _ListMaterial.push_back(newMaterial);

        newObject = new CMeshRendererObject();
        newObject->Initialize(newMesh, newMaterial);
        _ListMeshRendererObject.push_back(newObject);

        newObject->SetPosition(-2, 0, 0);
        newObject->SetScale(.5f, .5f, .5f);
    }

    return S_OK;
}
