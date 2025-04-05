// PracticeD3D11

#pragma once

#include <vector>

class GeometryObject;
struct GeometryContainer;

class GeometryObjectManager
{
public:
    GeometryObjectManager() = default;
    ~GeometryObjectManager() = default;

    GeometryObject* Create(ID3D11Device* _D3DDevice, bool doInitialize = true);
    void Remove(GeometryObject*& geometryObj);

    void RemoveAll();

    const std::vector<GeometryObject*>& GetContainer() noexcept;

protected:
    std::vector<GeometryObject*> _container;
};
