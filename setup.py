# -*- coding: utf-8 -*-
"""
Setup script used to build and install this package.

Adapted from the pybind cmake_example template.
https://github.com/pybind/cmake_example/blob/master/setup.py
"""
import os
import re
import subprocess
import sys
from pathlib import Path

from setuptools import find_packages, setup, Extension
from setuptools.command.build_ext import build_ext

from pdb import set_trace

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


class CMakeExtension(Extension):
    """
    An extended Extension-Class for CMake. We need this as we have to
    pass a source dir instead of a file_list to get the source-code to build

    The name must be _single_ output extension from the CMake build. If you need
    multiple extensions, see scikit-build
    """

    def __init__(self, name: str, sourcedir: str = "") -> None:
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        set_trace()
        # Must be in this form due to bug in .resolve() only fixed in Python 3.10+
        ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
        extdir = ext_fullpath.parent.resolve()

        # Using this requires trailing slash for auto-detection & inclusion of
        # auxiliary "native" libs

        debug = int(os.environ.get("DEBUG", 0)) if self.debug is None else self.debug
        cfg = "Debug" if debug else "Release"

        # CMake lets you override the generator - we need to check this.
        # Can be set with Conda-Build, for example.
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}{os.sep}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
            # Make sure we handle RPATH correctly when installing
            "-DCMAKE_INSTALL_RPATH=$ORIGIN",
            "-DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=ON",
            "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=OFF",
        ]
        build_args = []

        # Adding CMake arguments set as environment variable (needed e.g. to
        # build for ARM OSx on conda-forge). Notice they are space separated
        if "CMAKE_ARGS" in os.environ:
            cmake_args += [item for item in os.environ["CMAKE_ARGS"].split(" ") if item]

        # Add additional CMake arguments required for setting up this project
        cmake_args += [
            "-DRENDERER_BUILD_IMGUI=ON",
            "-DRENDERER_BUILD_LOGS=ON",
            "-DRENDERER_BUILD_PROFILING=OFF",
            "-DRENDERER_BUILD_PYTHON_BINDINGS=ON",
            "-DRENDERER_BUILD_EXAMPLES=OFF",
            "-DRENDERER_BUILD_TESTS=OFF",
            "-DRENDERER_BUILD_DOCS=OFF"
        ]

        ## # By default, place every generated artifact into the same install path
        ## library_outdir = extdir
        ## archive_outdir = extdir
        ## runtime_outdir = extdir

        if self.compiler.compiler_type != "msvc":
            # Using Ninja-build since it a) is available as a wheel and b)
            # multithreads automatically. MSVC would require all variables be
            # exported for Ninja to pick it up, which is a little tricky to do.
            # Users can override the generator with CMAKE_GENERATOR in CMake
            # 3.15+.
            if not cmake_generator:
                try:
                    import ninja

                    ninja_executable_path = Path(ninja.BIN_DIR) / "ninja"
                    cmake_args += [
                        "-GNinja",
                        f"-DCMAKE_MAKE_PROGRAM:FILEPATH={ninja_executable_path}"
                    ]
                except ImportError:
                    pass

            ## # Send all generated artifacts to the same install location
            ## cmake_args += [
            ##     f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={library_outdir}",
            ##     f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={archive_outdir}",
            ##     f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={runtime_outdir}",
            ## ]
        else:
            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in ["NMake", "Ninja"])

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in ["ARM", "Win64"])

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                cmake_args += [
                    f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
                    f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
                    f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}"
                ]
                build_args += ["--config", cfg]

        if sys.platform.startswith("darwin"):
            # Cross-compile support for macOS - respect ARCHFLAGS if set
            archs = re.findall(r"-arch (\S+)", os.environ.get("ARCHFLAGS", ""))
            if archs:
                cmake_args += ["-DCMAKE_OSX_ARCHITECTURE={}".format(";".join(archs))]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only
                build_args += ["-j{}".format(self.parallel)]

        build_temp = Path(self.build_temp) / ext.name
        if not build_temp.exists():
            build_temp.mkdir(parents=True)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )

long_description = ""
if os.path.exists("README.md"):
    with open("README.md", "r", encoding="utf-8") as fh:
        long_description = fh.read()

required_packages = []
if os.path.exists("requirements.txt"):
    with open("requirements.txt", "r", encoding="utf-8") as fh:
        required_packages = [line.replace("\n", "") for line in fh.readlines()]

setup(
    name='renderer',
    version="0.2.2",
    author='Wilbert Santos Pumacay Huallpa',
    author_email='wpumacay@gmail.com',
    description='A minimal renderer for prototyping 3D applications',
    long_description=long_description,
    long_description_content_type='text/markdown',
    license='MIT License',
    url='https://github.com/wpumacay/loco_renderer',
    keywords='graphics opengl',
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux"
    ],
    zip_safe=False,
    install_requires=required_packages,
    ext_modules=[CMakeExtension('renderer')],
    cmdclass={"built_ext": CMakeBuild}
)
