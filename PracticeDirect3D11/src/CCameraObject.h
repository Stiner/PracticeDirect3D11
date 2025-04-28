// PracticeD3D11

#pragma once

#include "CSceneObject.h"

class CCameraObject : public CSceneObject
{
public:
    CCameraObject();
    virtual ~CCameraObject() = default;

	virtual void Initialize() override;

	virtual void Update(float DeltaTime) override;
    void Draw();

    void Release();

    void SetFov(float fov) noexcept;
    void SetAspectRatio(float aspectRatio) noexcept;
    void SetNear(float nearZ) noexcept;
    void SetFar(float farZ) noexcept;

protected:
    virtual void UpdateMatrix() override;

protected:
    float _fov         = 0;
    float _aspectRatio = 0;
    float _nearZ       = 0;
    float _farZ        = 0;
};
