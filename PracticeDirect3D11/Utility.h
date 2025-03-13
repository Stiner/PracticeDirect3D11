// Practice D3D11

#pragma once

class CUtility
{
public:
    static HRESULT ReadFileToBlob(const WCHAR* szFileName, ID3DBlob** ppBlobOut);
    static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};
