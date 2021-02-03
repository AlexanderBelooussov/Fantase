#!/bin/bash

rm python/rlutilities -rf



# #for MSYS/GCC
# cd build_mingw
# cmake .. -G "MSYS Makefiles" -DWITH_TESTS=TRUE -DCMAKE_BUILD_TYPE=Release -DTARGET_LANGUAGE=both
# #cmake .. -G "MSYS Makefiles" -DWITH_TESTS=TRUE -DCMAKE_BUILD_TYPE=RelWithDebug -DTARGET_LANGUAGE=cpp
# make -j10

#for MSVC

mkdir python/rlutilities

mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -Ax64 -DWITH_TESTS=TRUE -DTARGET_LANGUAGE=python
cmake --build . --config Release
mv Release/rlutilities.*.pyd ../python/rlutilities/
cd ..


mkdir build37
cd build37
cmake .. -G "Visual Studio 16 2019" -Ax64 -DWITH_TESTS=TRUE -DTARGET_LANGUAGE=python -DPYTHON_EXECUTABLE="C:/Users/Kris/AppData/Local/Programs/Python/Python37/Python.exe"
cmake --build . --config Release
mv Release/rlutilities.*.pyd ../python/rlutilities/
cd ..


cd python/rlutilities
# stubs are generated based on the default python version, no reason why they shouldnt match..
py -m pybind11_stubgen rlutilities --no-setup -o=.. --root_module_suffix=
# __init__.py is hardcoded, this is fine as long as the dirs dont change
cp ../__init__.py __init__.py
cp ../../LICENSE.txt LICENSE.txt

read -p "$*"
