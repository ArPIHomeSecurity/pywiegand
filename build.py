import os
import shutil

from distutils.command.build_ext import build_ext
from distutils.core import Extension, Distribution
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


def build():
    distribution = Distribution({'name': 'extended', 'ext_modules': ext_modules})
    distribution.package_dir = 'extended'

    cmd = build_ext(distribution)
    cmd.ensure_finalized()
    cmd.run()

    # Copy built extensions back to the project
    for output in cmd.get_outputs():
        relative_extension = os.path.relpath(output, cmd.build_lib)
        shutil.copyfile(output, relative_extension)
        mode = os.stat(relative_extension).st_mode
        mode |= (mode & 0o444) >> 2
        os.chmod(relative_extension, mode)

if __name__ == '__main__':
    build()