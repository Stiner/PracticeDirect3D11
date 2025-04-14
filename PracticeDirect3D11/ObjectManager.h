// PracticeD3D11

#pragma once

#include <vector>

class MeshObject;

class ObjectManager
{
public:
    ObjectManager() = default;
    ~ObjectManager() = default;

    MeshObject* Create(ID3D11Device* _D3DDevice, bool doInitialize = true);
    void Remove(MeshObject*& geometryObj);

    void RemoveAll();

    inline const std::vector<MeshObject*>& GetContainer() noexcept
    {
        return _Container;
    };

protected:
    std::vector<MeshObject*> _Container;
};
