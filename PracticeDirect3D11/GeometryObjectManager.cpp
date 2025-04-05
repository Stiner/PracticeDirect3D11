// PracticeD3D11

#include "PCH.h"
#include "GeometryObjectManager.h"
#include "GeometryObject.h"

GeometryObject* GeometryObjectManager::Create(ID3D11Device* _D3DDevice, bool doInitialize)
{
    GeometryObject* newGeometry = new GeometryObject();

    if (doInitialize)
        newGeometry->Initialize(_D3DDevice);

    _container.push_back(newGeometry);

    return newGeometry;
}

void GeometryObjectManager::Remove(GeometryObject*& geometryObj)
{
    auto it = std::find(_container.begin(), _container.end(), geometryObj);
    if (it != _container.end())
    {
        _container.erase(it);

        geometryObj->Release();

        MEM_DELETE(geometryObj);
    }
}

void GeometryObjectManager::RemoveAll()
{
    for (auto& geometryObj : _container)
    {
        geometryObj->Release();
        MEM_DELETE(geometryObj);
    }
    _container.clear();
}

const std::vector<GeometryObject*>& GeometryObjectManager::GetContainer() noexcept
{
    return _container;
}
