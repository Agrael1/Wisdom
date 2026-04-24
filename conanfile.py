import os
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps
from conan.tools.files import copy, load


class WisdomConan(ConanFile):
    name = "wisdom"
    package_type = "library"

    license = "MIT"
    url = "https://github.com/Agrael1/Wisdom"
    description = "Lightweight C++20 graphics abstraction library over DX12 and Vulkan"
    topics = ("graphics", "vulkan", "directx12", "cmake", "cxx20")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_platform": [True, False],
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "build_platform": True,
    }

    # keep it for now, but remove when we are at CCI
    def set_version(self):
        version_file_path = os.path.join(self.recipe_folder, "version/VERSION")
        
        try:
            # Read the file and strip any whitespace/newlines
            self.version = load(self, version_file_path).strip()
        except Exception as e:
            # It is highly recommended to provide a fallback or clear error 
            # so the recipe doesn't cryptically crash if the file is missing
            self.output.warning(f"Could not read version file: {e}")
            self.version = "0.0.0"

    def requirements(self):
        # If windows platform support is enabled, we need to require the D3D12 Memory Allocator
        if self.settings.os == "Windows":
            self.requires("d3d12-memory-allocator/[>=3.0.1 <4]", transitive_headers=True)
        self.requires("vulkan-memory-allocator/3.3.0", transitive_headers=True)

    def export_sources(self):
        copy(
            self,
            "*",
            src=self.recipe_folder,
            dst=self.export_sources_folder,
            excludes=(
                ".git/*",
                ".github/*",
                ".gitignore",
                ".vs/*",
                "**/.vs/*",
                "out/*",
                "build/*",
                "install/*",
                "cmake-build-*/*",
                "_deps/*",
                "__pycache__/*",
            ),
        )

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        self.output.warning(
            "This recipe currently relies on the project's CPM/NuGet dependency loading during CMake configure. "
            "For Conan Center, those dependencies should be provided as Conan requirements or vendored sources."
        )

        tc = CMakeToolchain(self)
        tc.variables["WISDOM_BUILD_EXAMPLES"] = False
        tc.variables["WISDOM_BUILD_TESTS"] = False
        tc.variables["WISDOM_BUILD_DOCS"] = False
        tc.variables["WISDOM_BUILD_STATIC"] = not self.options.get_safe("shared")
        tc.variables["WISDOM_BUILD_SHARED"] = self.options.get_safe("shared")
        tc.variables["WISDOM_BUILD_PLATFORM"] = self.options.build_platform
        tc.variables["WISDOM_USE_AGILITY_SDK"] = False
        tc.variables["WISDOM_USE_CONAN"] = True
        tc.variables["WISDOM_DOWNLOAD_DXC"] = False
        tc.variables["CMAKE_UNITY_BUILD"] = True

        if self.settings.os == "Windows":
            tc.preprocessor_definitions["VK_USE_PLATFORM_WIN32_KHR"] = "1"

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        # The overarching file namespace (find_package(wisdom))
        self.cpp_info.set_property("cmake_file_name", "Wisdom")

        build_modules = ["lib/cmake/wisdom/functions.cmake"]
        self.cpp_info.set_property("cmake_build_modules", build_modules)


        # Targets:
        suffix = "d" if self.settings.build_type == "Debug" else ""
        if self.options.get_safe("shared"):
            # Core Shared
            self.cpp_info.components["core"].set_property("cmake_target_name", "wis::wisdom-shared")
            self.cpp_info.components["core"].libs = [f"wisdom-shared{suffix}"]

            # Platform Shared
            if self.options.build_platform:
                self.cpp_info.components["platform"].set_property("cmake_target_name", "wis::wisdom-platform-shared")
                self.cpp_info.components["platform"].requires = ["core"]
                self.cpp_info.components["platform"].libs = [f"wisdom-platform-shared{suffix}"]
        else:
            # Core Static
            self.cpp_info.components["core"].set_property("cmake_target_name", "wis::wisdom")
            self.cpp_info.components["core"].libs = [f"wisdom{suffix}", f"vkma{suffix}"]

            # Platform Static
            if self.options.build_platform:
                self.cpp_info.components["platform"].set_property("cmake_target_name", "wis::wisdom-platform")
                self.cpp_info.components["platform"].requires = ["core"]
                self.cpp_info.components["platform"].libs = [f"wisdom-platform{suffix}"]

        self.cpp_info.components["core"].requires = ["vulkan-memory-allocator::vulkan-memory-allocator"]
        if self.settings.os == "Windows":
            self.cpp_info.components["core"].defines.extend([
                "D3D12MA_USING_DIRECTX_HEADERS=1",
                "VK_USE_PLATFORM_WIN32_KHR=1",
            ])
            self.cpp_info.components["core"].requires.extend([
                "d3d12-memory-allocator::d3d12-memory-allocator"
            ])
            self.cpp_info.components["core"].system_libs.extend(["dxgi", "DXGUID"])