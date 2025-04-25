// PracticeD3D11

#pragma once

#include "CD3DApp.h"

class CMesh;
class CMaterial;
class CMeshRendererObject;
class CameraObject;

class CPracticeD3D11 : public CD3D11App
{
public:
    CPracticeD3D11(HINSTANCE hAppInstance, const TCHAR* szAppName);
    virtual ~CPracticeD3D11() override;

public:
    virtual bool ProccessCmdLine(const TCHAR* szCmdLine);
    virtual bool Initialize() override;
    virtual void Release() override;
    virtual bool OnResize() override;
    virtual void UpdateScene(float dt) override;
    virtual void DrawScene() override;
    virtual LRESULT MsgProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam) override;
    virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

protected:
    std::vector<CMesh*> _ListMesh;
    std::vector<CMaterial*> _ListMaterial;
    std::vector<CMeshRendererObject*> _ListMeshRendererObject;

    CameraObject* _CameraObject = nullptr;
};
