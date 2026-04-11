from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import collect_libs, copy


class WisdomConan(ConanFile):
    name = "wisdom"
    version = "0.7.0"
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
        "shared": False,
        "fPIC": True,
        "build_platform": True,
    }

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
        self.output.warning(
            "This recipe currently relies on the project's CPM/NuGet dependency loading during CMake configure. "
            "For Conan Center, those dependencies should be provided as Conan requirements or vendored sources."
        )

        tc = CMakeToolchain(self)
        tc.generator = "Ninja"
        tc.variables["WISDOM_BUILD_EXAMPLES"] = False
        tc.variables["WISDOM_BUILD_TESTS"] = False
        tc.variables["WISDOM_BUILD_DOCS"] = False
        tc.variables["WISDOM_BUILD_STATIC"] = not self.options.shared
        tc.variables["WISDOM_BUILD_SHARED"] = self.options.shared
        tc.variables["WISDOM_BUILD_PLATFORM"] = self.options.build_platform
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "wisdom")
        self.cpp_info.builddirs.append("lib/cmake/wisdom")

        all_libs = collect_libs(self)

        core_target = "wis::wisdom-shared" if self.options.shared else "wis::wisdom"
        core_lib_hints = {"wisdom-shared", "wisdom"}
        core_libs = [lib for lib in all_libs if any(
            h in lib for h in core_lib_hints)]
        platform_libs = [lib for lib in all_libs if "platform" in lib]

        self.cpp_info.components["headers"].set_property(
            "cmake_target_name", "wis::wisdom-headers")

        self.cpp_info.components["core"].set_property(
            "cmake_target_name", core_target)
        self.cpp_info.components["core"].requires = ["headers"]
        self.cpp_info.components["core"].libs = core_libs

        self.cpp_info.components["platform_headers"].set_property(
            "cmake_target_name", "wis::wisdom-platform-headers"
        )
        self.cpp_info.components["platform_headers"].requires = ["headers"]

        self.cpp_info.components["platform"].set_property(
            "cmake_target_name", "wis::wisdom-platform")
        self.cpp_info.components["platform"].requires = [
            "core", "platform_headers"]
        self.cpp_info.components["platform"].libs = platform_libs
