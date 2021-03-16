@echo off

endlocal
setlocal

if not exist "tmp-win\" (
  mkdir tmp-win
)

cd tmp-win
rem cmake ^
rem  -DCMAKE_C_COMPILER="C:\mingw64\bin\gcc-nm.exe" ^
rem  -DCMAKE_CXX_COMPILER="C:\mingw64\bin\g++.exe" ^
rem  ..                       

cmake  -G "MinGW Makefiles" ..
mingw32-make.exe