#pragma once

#include "framework.h"

class MyUtility
{
public:
    static HRESULT ReadFileToBlob(const WCHAR* szFileName, ID3DBlob** ppBlobOut);
    static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};