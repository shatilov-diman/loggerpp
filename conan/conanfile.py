
from conans import ConanFile, CMake, tools

class LoggerConan(ConanFile):
	name="loggerpp"
	version="1.0"
	description = "charivari's logger framework"
	url = "https://github.com/shatilov-diman/loggerpp"
	license = "GPL v3"

	settings = "os", "compiler", "build_type", "arch"
	options = {"shared": [True, False]}
	default_options = "shared=False"
	generators = "cmake"
	requires = "gtest/master@conan/dev"

	def configure(self):
		self.settings.compiler["gcc"].version = "7"
		self.settings.compiler["Visual Studio"].version = "15"

	def source(self):
		self.run("git clone https://github.com/shatilov-diman/loggerpp.git .")

	def test(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

		self.run(os.path.join(self.build_folder, "bin", "tests"))

	def package(self):
		self.copy("*.h")
		self.copy("*.lib", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.so", dst="lib", keep_path=False)
		self.copy("*.dylib", dst="lib", keep_path=False)
		self.copy("*.a", dst="lib", keep_path=False)

