// PracticeD3D11

#include "PCH.h"
#include "PracticeD3D11.h"

#include "MeshRendererObject.h"
#include "CameraObject.h"
#include "MeshCube.h"
#include "MeshCubeTex.h"
#include "Material.h"

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
    if (!__super::Initialize())
        return false;

    _CameraObject = new CameraObject();
	_CameraObject->Initialize(_D3DDevice);
	_CameraObject->SetAspectRatio(GetAspectRatio());
    _CameraObject->SetPosition(0, 0, -5.f);
    _CameraObject->SetRotation(0, 0, 0);

    Mesh* newMesh = nullptr;
    Material* newMaterial = nullptr;
    MeshRendererObject* newObject = nullptr;

    {
        newMesh = new MeshCube();
        newMesh->Initialize();
        _ListMesh.push_back(newMesh);

        newMaterial = new Material();
        newMaterial->Initialize();
        _ListMaterial.push_back(newMaterial);

        newObject = new MeshRendererObject();
        newObject->Initialize(_D3DDevice, newMesh, newMaterial);
        _ListMeshRendererObject.push_back(newObject);

        newObject->SetPosition(2, 0, 0);
    }
    {
        newMesh = new MeshCube();
        newMesh->Initialize();
        _ListMesh.push_back(newMesh);

        newMaterial = new Material();
        newMaterial->Initialize();
        _ListMaterial.push_back(newMaterial);

        newObject = new MeshRendererObject();
        newObject->Initialize(_D3DDevice, newMesh, newMaterial);
        _ListMeshRendererObject.push_back(newObject);

        newObject->SetPosition(-2, 0, 0);
        newObject->SetScale(.5f, .5f, .5f);
    }

    return true;
}

template<class T> void RemoveAll(std::vector<T*> list)
{
    for (T* e : list)
    {
        if (e != nullptr) e->Release();
        MEM_DELETE(e);
    }
    list.clear();
}

void CPracticeD3D11::Release()
{
    RemoveAll(_ListMesh);
    RemoveAll(_ListMaterial);
    RemoveAll(_ListMeshRendererObject);

    _CameraObject->Release();
    MEM_DELETE(_CameraObject);

    __super::Release();
}

bool CPracticeD3D11::OnResize()
{
    return __super::OnResize();
}

void CPracticeD3D11::UpdateScene(float DeltaTime)
{
    _CameraObject->Update(DeltaTime);

    for (MeshRendererObject* Object : _ListMeshRendererObject)
    {
        if (Object != nullptr) Object->Update(DeltaTime);
    }
}

void CPracticeD3D11::DrawScene()
{
    _CameraObject->Draw(_D3DDeviceContext);

    for (MeshRendererObject* Object : _ListMeshRendererObject)
    {
        if (Object != nullptr) Object->Draw(_D3DDeviceContext);
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
