#ifndef ShaderCompiler_h__
#define ShaderCompiler_h__
#include "stdHeaders.h"

class CShaderCompiler
{
public:
	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);
};






#endif // ShaderCompiler_h__
