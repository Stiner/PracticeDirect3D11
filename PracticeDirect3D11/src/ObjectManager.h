// PracticeD3D11

#pragma once

#include <vector>

class MeshRendererObject;

class ObjectManager
{
public:
    ObjectManager() = default;
    ~ObjectManager() = default;

    MeshRendererObject* Create(ID3D11Device* _D3DDevice, bool doInitialize = true);
    void Remove(MeshRendererObject*& geometryObj);

    void RemoveAll();

    inline const std::vector<MeshRendererObject*>& GetContainer() noexcept
    {
        return _Container;
    };

protected:
    std::vector<MeshRendererObject*> _Container;
};
