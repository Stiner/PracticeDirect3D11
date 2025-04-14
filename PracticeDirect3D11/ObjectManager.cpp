// PracticeD3D11

#include "PCH.h"
#include "ObjectManager.h"

#include "MeshObject.h"

MeshObject* ObjectManager::Create(ID3D11Device* D3DDevice, bool DoInitialize)
{
    MeshObject* newObject = new MeshObject();

    if (DoInitialize)
        newObject->Initialize(D3DDevice);

    _Container.push_back(newObject);

    return newObject;
}

void ObjectManager::Remove(MeshObject*& TargetObject)
{
    auto it = std::find(_Container.begin(), _Container.end(), TargetObject);
    if (it != _Container.end())
    {
        TargetObject->Release();
        MEM_DELETE(TargetObject);

        _Container.erase(it);
    }
}

void ObjectManager::RemoveAll()
{
    for (auto& Object : _Container)
    {
        Object->Release();
        MEM_DELETE(Object);
    }
    _Container.clear();
}
