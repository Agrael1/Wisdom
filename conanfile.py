import os
from conan import ConanFile
from conan.tools.cmake import CMake
from conan.tools.cmake import cmake_layout
from conan.tools.cmake import CMakeToolchain
from conan.tools.files import collect_libs
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
        "header_only": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_platform": True,
        "header_only": False,
    }

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
        if self.options.shared or self.options.header_only:
            self.options.rm_safe("fPIC")

        if self.options.header_only:
            self.options.rm_safe("shared")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        self.output.warning(
            "This recipe currently relies on the project's CPM/NuGet dependency loading during CMake configure. "
            "For Conan Center, those dependencies should be provided as Conan requirements or vendored sources."
        )

        is_header_only = self.options.get_safe("header_only")

        tc = CMakeToolchain(self)
        tc.variables["WISDOM_BUILD_EXAMPLES"] = False
        tc.variables["WISDOM_BUILD_TESTS"] = False
        tc.variables["WISDOM_BUILD_DOCS"] = False
        tc.variables["WISDOM_BUILD_STATIC"] = not self.options.get_safe("shared") and not is_header_only
        tc.variables["WISDOM_BUILD_SHARED"] = self.options.get_safe("shared") and not is_header_only
        tc.variables["WISDOM_BUILD_PLATFORM"] = self.options.build_platform
        tc.variables["WISDOM_USE_AGILITY_SDK"] = False
        tc.variables["CMAKE_UNITY_BUILD"] = True
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
        self.cpp_info.set_property("cmake_file_name", "wisdom")

        build_modules = ["lib/cmake/wisdom/functions.cmake"]
        self.cpp_info.set_property("cmake_build_modules", build_modules)

        # ---------------------------------------------------------
        # 1. HEADER-ONLY TARGETS (Always Available)
        # ---------------------------------------------------------
        
        # Core Headers (wis::wisdom-headers)
        self.cpp_info.components["headers"].set_property("cmake_target_name", "wis::wisdom-headers")
        self.cpp_info.components["headers"].bindirs = []
        self.cpp_info.components["headers"].libdirs = []

        # Platform Headers (wis::wisdom-platform-headers)
        if self.options.build_platform:
            self.cpp_info.components["platform_headers"].set_property("cmake_target_name", "wis::wisdom-platform-headers")
            self.cpp_info.components["platform_headers"].requires = ["headers"]
            self.cpp_info.components["platform_headers"].bindirs = []
            self.cpp_info.components["platform_headers"].libdirs = []

        # If header_only is True, we stop here. No compiled libs are added.
        if self.options.get_safe("header_only"):
            return

        # ---------------------------------------------------------
        # 2. COMPILED TARGETS (Static OR Shared)
        # ---------------------------------------------------------
        suffix = "d" if self.settings.build_type == "Debug" else ""
        if self.options.get_safe("shared"):
            # Core Shared
            self.cpp_info.components["core"].set_property("cmake_target_name", "wis::wisdom-shared")
            self.cpp_info.components["core"].requires = ["headers"]
            self.cpp_info.components["core"].libs = [f"wisdom-shared{suffix}"]

            # Platform Shared
            if self.options.build_platform:
                self.cpp_info.components["platform"].set_property("cmake_target_name", "wis::wisdom-platform-shared")
                self.cpp_info.components["platform"].requires = ["core", "platform_headers"]
                self.cpp_info.components["platform"].libs = [f"wisdom-platform-shared{suffix}"]
        else:
            # Core Static
            self.cpp_info.components["core"].set_property("cmake_target_name", "wis::wisdom")
            self.cpp_info.components["core"].requires = ["headers"]
            self.cpp_info.components["core"].libs = [f"wisdom{suffix}"]

            # Platform Static
            if self.options.build_platform:
                self.cpp_info.components["platform"].set_property("cmake_target_name", "wis::wisdom-platform")
                self.cpp_info.components["platform"].requires = ["core", "platform_headers"]
                self.cpp_info.components["platform"].libs = [f"wisdom-platform{suffix}"]