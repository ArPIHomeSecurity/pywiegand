from setuptools import find_packages, setup
from distutils.core import Extension

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

module = Extension(
    "pywiegand",
    sources=["pywiegand.cpp"],
    libraries=["wiringPi", "pthread", "rt"],
    extra_compile_args=[
        "-lpthread",
        "-lrt",
        "-I/usr/local/include",
        "-L/usr/local/lib",
        "-lwiringPi",
    ],
)
setup(
    name="pywiegand",
    version="0.1.0",
    author="Gábor Kovács",
    author_email="gkovacs81@gmail.com",
    license="MIT",
    description="Wiegand protocol on Raspberry PI",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/ArPIHomeSecurity/pywiegand",
    project_urls={
        "Bug Tracker": "https://github.com/ArPIHomeSecurity/pywiegand/issues",
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    package_dir={"": "src"},
    packages=find_packages(where="src"),
    python_requires=">=3.6",
    ext_modules=[module],
    install_requires=["wiringPi"],
)
