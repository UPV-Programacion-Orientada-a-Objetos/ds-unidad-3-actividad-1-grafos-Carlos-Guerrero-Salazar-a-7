from setuptools import setup, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        "neuronet",
        sources=["src/main.pyx", "src/GrafoDisperso.cpp"],
        include_dirs=["src"],
        language="c++",
        extra_compile_args=["-std=c++11"],
    )
]

setup(
    name="neuronet",
    ext_modules=cythonize(extensions),
)
