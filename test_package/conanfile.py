import os
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain
from conan.tools.build import can_run

class WisdomTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        # Check if the wisdom package we are testing was built as shared
        is_shared = self.dependencies["wisdom"].options.shared
        # Pass that info to CMake!
        tc.variables["WISDOM_IS_SHARED"] = is_shared
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "test_app")
            self.run(cmd, env="conanrun")