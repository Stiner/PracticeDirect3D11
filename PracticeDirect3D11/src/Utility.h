// Practice D3D11

#pragma once

namespace Const
{
    namespace Vector
    {
        XMGLOBALCONST XMVECTOR BaseForward = { 0, 0, 1, 0 };
        XMGLOBALCONST XMVECTOR BaseUp      = { 0, 1, 0, 0 };
    }
}

namespace Colors
{
    XMGLOBALCONST DirectX::XMFLOAT4 White   = { 1.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Black   = { 0.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Red     = { 1.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Green   = { 0.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST DirectX::XMFLOAT4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
}

namespace Shader
{
    HRESULT ReadFileToBlob(const WCHAR* szFileName, ID3DBlob** ppBlobOut);
    HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
}
