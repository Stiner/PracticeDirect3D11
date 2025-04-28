// PracticeD3D11

#pragma once

#include "CD3DApp.h"

class CMesh;
class CMaterial;
class CSceneObject;

class CPracticeD3D11 : public CD3D11App
{
public:
    CPracticeD3D11(HINSTANCE hAppInstance, const TCHAR* szAppName);
    virtual ~CPracticeD3D11() override;

public:
    virtual HRESULT ProccessCmdLine(const TCHAR* szCmdLine) override;
    virtual HRESULT Initialize() override;
    virtual void Release() override;
    virtual HRESULT OnResize() override;
    virtual void UpdateScene(float dt) override;
    virtual void DrawScene() override;
    virtual LRESULT MsgProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam) override;
    virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

protected:
    virtual HRESULT BuildScene();

protected:
    std::vector<CMesh*> _ListMesh;
    std::vector<CMaterial*> _ListMaterial;

    std::vector<CSceneObject*> _ListSceneObject;
    //std::vector<CMeshRendererObject*> _ListMeshRendererObject;

    //CCameraObject* _CameraObject = nullptr;
};
