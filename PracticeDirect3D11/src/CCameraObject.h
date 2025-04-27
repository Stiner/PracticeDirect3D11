// PracticeD3D11

#pragma once

#include "CSceneObject.h"

class CameraObject : public CSceneObject
{
public:
    CameraObject();
    virtual ~CameraObject() = default;

	virtual void Initialize() override;

	virtual void Update(float DeltaTime) override;
    void Draw();

    void Release();

    void SetPosition(float x, float y, float z) noexcept;
    void SetRotation(float pitch, float yaw, float roll) noexcept;
    void SetScale(float x, float y, float z) noexcept;

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
