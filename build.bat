cd Bot

del build\\Release\\my_bot.exe
del build\\bin\\Release\\rlutilities.dll

cmake -DTARGET_LANGUAGE=cpp -G "Visual Studio 16 2019" -A x64 -B build
cmake --build build --config Release

del release\\my_bot.exe
del release\\rlutilities.dll

copy build\\Release\\my_bot.exe release
copy build\\bin\\Release\\rlutilities.dll release

cd ..