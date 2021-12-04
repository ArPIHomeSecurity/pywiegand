from distutils.command.build_ext import build_ext
from distutils.core import Extension
from distutils.errors import CCompilerError, DistutilsExecError, DistutilsPlatformError

ext_modules = [
    Extension(
        "pywiegand.extension.pywiegand_adapter",
        include_dirs=["pywiegand/extension"],
        sources=["pywiegand/extension/pywiegand_adapter.cpp"],
        libraries=["wiringPi", "pthread", "rt"],
        extra_compile_args=[
            "-lpthread",
            "-lrt",
            "-I/usr/local/include",
            "-L/usr/local/lib",
            "-lwiringPi",
        ]
    )
]


class BuildFailed(Exception):
    pass


class ExtBuilder(build_ext):

    def run(self):
        try:
            build_ext.run(self)
        except (DistutilsPlatformError, FileNotFoundError):
            raise BuildFailed('File not found. Could not compile C extension.')

    def build_extension(self, ext):
        try:
            build_ext.build_extension(self, ext)
        except (CCompilerError, DistutilsExecError, DistutilsPlatformError, ValueError):
            raise BuildFailed('Could not compile C extension.')


def build(setup_kwargs):
    """
    This function is mandatory in order to build the extensions.
    """
    setup_kwargs.update({
        "ext_modules": ext_modules,
        "cmdclass": {
            "build_ext": ExtBuilder
        }
    })