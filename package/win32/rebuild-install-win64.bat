REM setup variables
setlocal
set WIN64_BUILD_PATH=build64
IF "%CMAKE_BUILD_TYPE%" == "" set CMAKE_BUILD_TYPE=Release
IF "%CMAKE_BUILD_TYPE%" == "Debug" set WIN64_BUILD_PATH=build64-debug
set MINGW64_PATH=%CD%\..\..\dependencies\windows\mingw64-x86_64-posix-sjlj-4.9.1\bin
set INSTALL_PATH=%1%

REM perform 64-bit build 
setlocal
set PATH=%MINGW64_PATH%;%PATH%
mkdir %WIN64_BUILD_PATH%
cd %WIN64_BUILD_PATH%
cmake -G"MinGW Makefiles" ^
      -DCMAKE_INSTALL_PREFIX:String=%INSTALL_PATH% ^
      -DRSTUDIO_TARGET=SessionWin64 ^
      -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
      -DRSTUDIO_PACKAGE_BUILD=1 ^
      ..\..\..
mingw32-make install
cd ..
endlocal


