// PracticeD3D11

#include "PCH.h"
#include "GeometryObjectFactory.h"
#include "GeometryObject.h"

GeometryObject* GeometryObjectFactory::Create(ID3D11Device* _D3DDevice, bool doInitialize)
{
    GeometryObject* newGeometry = new GeometryObject();

    if (doInitialize)
        newGeometry->Initialize(_D3DDevice);

    _container.push_back(newGeometry);

    return newGeometry;
}

void GeometryObjectFactory::Remove(GeometryObject*& geometryObj)
{
    auto it = std::find(_container.begin(), _container.end(), geometryObj);
    if (it != _container.end())
    {
        _container.erase(it);

        geometryObj->Release();

        MEM_DELETE(geometryObj);
    }
}

const std::vector<GeometryObject*>& GeometryObjectFactory::GetContainer() noexcept
{
    return _container;
}
