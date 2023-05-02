//#pragma once
//#include <wisdom/wisdom.h>
//
//#include <dxc/dxcapi.h>
//
//
//namespace wis
//{
//	//unfinished for linux
//	class ShaderCompiler
//	{
//		constexpr static inline std::array dx_shader{
//			L"?",
//			L"v",
//			L"p",
//			L"g",
//			L"h",
//			L"d",
//			L"a",
//			L"m",
//			L"c",
//		};
//
//	public:
//		ShaderCompiler()
//		{
//			DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcutils));
//			DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc));
//		}
//		~ShaderCompiler()
//		{
//			dxc->Release();
//			dxcutils->Release();
//		}
//	public:
//		template<class ShaderTy = Shader>
//		[[nodiscard]] ShaderTy CompileFromHLSLCode(std::string_view code, ShaderType type, std::pair<uint16_t, uint16_t> version = {6,5})
//		{
//			IDxcBlob* blob = nullptr;
//			if constexpr (std::is_same_v<ShaderTy, DX12Shader>)
//			{
//				blob = CompileToDXIL(code, std::format(L"{}s_{}_{}", dx_shader[uint32_t(type)], version.first, version.second));
//				std::vector<std::byte> x{(std::byte*)blob->GetBufferPointer(), (std::byte*)blob->GetBufferPointer() + blob->GetBufferSize()};
//				blob->Release();
//				return ShaderTy{ std::move(x), type };
//			}
//			return{};
//		}
//	private:
//		IDxcBlob* Compile(std::string_view code, std::span<LPCWSTR> args)
//		{
//			DxcBuffer src{
//				.Ptr = code.data(),
//				.Size = code.size(),
//				.Encoding = 0
//			};
//
//			IDxcResult* result;
//			dxc->Compile(&src, args.data(), uint32_t(args.size()), nullptr, IID_PPV_ARGS(&result));
//
//			IDxcBlob* blob;
//			result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&blob), nullptr);
//			result->Release();
//
//			return blob;
//		}
//		IDxcBlob* CompileToSPV(std::string_view code)
//		{
//			return nullptr;
//		}
//		IDxcBlob* CompileToDXIL(std::string_view code, std::wstring_view type)
//		{
//			std::array args{
//				L"-Zpc",
//				L"-T",
//				type.data(),
//				L"-E",
//				L"main"
//			};
//			Compile(code, args);
//		}
//
//
//	private:
//		IDxcUtils* dxcutils = nullptr;
//		IDxcCompiler3* dxc = nullptr;
//	};
//}