// PracticeD3D11

#pragma once

#include <vector>

class GeometryObject;
struct GeometryContainer;

class GeometryObjectFactory
{
public:
    GeometryObjectFactory() = default;
    ~GeometryObjectFactory() = default;

    GeometryObject* Create(ID3D11Device* _D3DDevice, bool doInitialize = true);
    void Remove(GeometryObject*& geometryObj);

    const std::vector<GeometryObject*>& GetContainer() noexcept;

protected:
    std::vector<GeometryObject*> _container;
};
