from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps

class EvolutionSimConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("raylib/5.5")
        self.requires("spdlog/1.12.0")
        self.requires("entt/3.11.1")
        self.requires("nlohmann_json/3.11.2")