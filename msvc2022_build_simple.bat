@echo off

endlocal
setlocal enabledelayedexpansion

pushd %~dp0
set SCRIPT_DIR=%CD%
popd

echo Current dir: %SCRIPT_DIR%
set WIN_BIN=%SCRIPT_DIR%\win-bin\
echo win bin: %WIN_BIN%

set WIN_BIN_RELEASE=%SCRIPT_DIR%\win-bin-release\
echo win-bin-release: %WIN_BIN_RELEASE%

:: downloads win-bin folder (with 3rd-party) build and release

IF NOT EXIST "%WIN_BIN%" (
  echo Creating %WIN_BIN% folder
  mkdir "%WIN_BIN%"
)

IF NOT EXIST "%WIN_BIN_RELEASE%" (
  echo Creating %WIN_BIN_RELEASE% folder
  mkdir "%WIN_BIN_RELEASE%"
)

:: https://www.libsdl.org/download-2.0.php
:: devel level
set WIN_SDL2_VER=2.0.14
set WIN_SDL2_DIR=%WIN_BIN%SDL2-%WIN_SDL2_VER%
set WIN_SDL2_ZIP=%WIN_BIN%SDL2-devel-%WIN_SDL2_VER%-VC.zip
set WIN_SDL2_URL=https://www.libsdl.org/release/SDL2-devel-%WIN_SDL2_VER%-VC.zip
set WIN_SDL2_RUNTIME_DIR=%WIN_BIN_RELEASE%SDL2-%WIN_SDL2_VER%
set WIN_SDL2_RUNTIME_ZIP=%WIN_BIN_RELEASE%SDL2-%WIN_SDL2_VER%-win32.zip
set WIN_SDL2_RUNTIME_URL=https://www.libsdl.org/release/SDL2-%WIN_SDL2_VER%-win32-x64.zip
IF NOT EXIST "%WIN_SDL2_DIR%" (
  ::sets errorlevel to 0
  if NOT EXIST "%WIN_SDL2_ZIP%" (
    echo Downloading %WIN_SDL2_URL%  
    powershell -Command "(New-Object Net.WebClient).DownloadFile('%WIN_SDL2_URL%', '%WIN_SDL2_ZIP%');"
    if NOT EXIST "%WIN_SDL2_ZIP%" (
      call echo Could not download
      EXIT /B 1
    )
  ) else (
    echo Already downloaded %WIN_SDL2_ZIP%
  )
  echo Unzip %WIN_SDL2_ZIP%
  powershell -nologo -noprofile -command "& { $shell = New-Object -COM Shell.Application; $target = $shell.NameSpace('%WIN_BIN%'); $zip = $shell.NameSpace('%WIN_SDL2_ZIP%'); $target.CopyHere($zip.Items(), 16); }"
  if not exist %WIN_SDL2_DIR% (
    call echo Could not unzip
    EXIT /B 1
  )
  echo Done
) else (
  echo Found %WIN_SDL2_DIR%
)
:: runtime binaries
IF NOT EXIST "%WIN_SDL2_RUNTIME_DIR%" (
  if NOT EXIST "%WIN_SDL2_RUNTIME_ZIP%" (
    echo Downloading %WIN_SDL2_RUNTIME_URL%
    powershell -Command "(New-Object Net.WebClient).DownloadFile('%WIN_SDL2_RUNTIME_URL%', '%WIN_SDL2_RUNTIME_ZIP%')"
    if NOT EXIST "%WIN_SDL2_RUNTIME_ZIP%" (
      call echo Could not download
      EXIT /B 1
    )
  ) else (
    echo Already downloaded %WIN_SDL2_RUNTIME_ZIP%
  )
  echo Unzip %WIN_SDL2_RUNTIME_ZIP%
  mkdir %WIN_SDL2_RUNTIME_DIR%
  powershell -nologo -noprofile -command "& { $shell = New-Object -COM Shell.Application; $target = $shell.NameSpace('%WIN_SDL2_RUNTIME_DIR%'); $zip = $shell.NameSpace('%WIN_SDL2_RUNTIME_ZIP%'); $target.CopyHere($zip.Items(), 16); }"
  if not exist %WIN_SDL2_RUNTIME_DIR%\SDL2.dll (
    call echo Not found %WIN_SDL2_RUNTIME_DIR%\SDL2.dll 
    EXIT /B 1
  )
  echo Done
)

:: https://www.libsdl.org/projects/SDL_image/
set WIN_SDL2_IMAGE_VER=2.0.5
set WIN_SDL2_IMAGE_DIR=%WIN_BIN%SDL2_image-%WIN_SDL2_IMAGE_VER%
set WIN_SDL2_IMAGE_ZIP=%WIN_BIN%SDL2_image-devel-%WIN_SDL2_IMAGE_VER%-VC.zip
set WIN_SDL2_IMAGE_URL=https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-%WIN_SDL2_IMAGE_VER%-VC.zip
set WIN_SDL2_IMAGE_RUNTIME_DIR=%WIN_BIN_RELEASE%SDL2_image-%WIN_SDL2_IMAGE_VER%
set WIN_SDL2_IMAGE_RUNTIME_ZIP=%WIN_BIN_RELEASE%SDL2_image-devel-%WIN_SDL2_IMAGE_VER%-VC.zip
set WIN_SDL2_IMAGE_RUNTIME_URL=https://www.libsdl.org/projects/SDL_image/release/SDL2_image-%WIN_SDL2_IMAGE_VER%-win32-x64.zip
IF NOT EXIST "%WIN_SDL2_IMAGE_DIR%" (
  if NOT EXIST "%WIN_SDL2_IMAGE_ZIP%" (
    echo Downloading %WIN_SDL2_IMAGE_URL%
    powershell -Command "(New-Object Net.WebClient).DownloadFile('%WIN_SDL2_IMAGE_URL%', '%WIN_SDL2_IMAGE_ZIP%')"
    if NOT EXIST "%WIN_SDL2_IMAGE_ZIP%" (
      call echo Could not download
      EXIT /B 1
    )
  ) else (
    echo Already downloaded %WIN_SDL2_IMAGE_ZIP%
  )
  echo Unzip %WIN_SDL2_IMAGE_ZIP%
  powershell -nologo -noprofile -command "& { $shell = New-Object -COM Shell.Application; $target = $shell.NameSpace('%WIN_BIN%'); $zip = $shell.NameSpace('%WIN_SDL2_IMAGE_ZIP%'); $target.CopyHere($zip.Items(), 16); }"
  if not exist %WIN_SDL2_IMAGE_DIR% (
    call echo Could not unzip
    EXIT /B 1
  )
  echo Done
) else (
  echo Found %WIN_SDL2_IMAGE_DIR%
)
:: runtime binaries
IF NOT EXIST "%WIN_SDL2_IMAGE_RUNTIME_DIR%" (
  if NOT EXIST "%WIN_SDL2_IMAGE_RUNTIME_ZIP%" (
    echo Downloading %WIN_SDL2_IMAGE_RUNTIME_URL%
    powershell -Command "(New-Object Net.WebClient).DownloadFile('%WIN_SDL2_IMAGE_RUNTIME_URL%', '%WIN_SDL2_IMAGE_RUNTIME_ZIP%')"
    if NOT EXIST "%WIN_SDL2_IMAGE_RUNTIME_ZIP%" (
      call echo Could not download
      EXIT /B 1
    )
  ) else (
    echo Already downloaded %WIN_SDL2_IMAGE_RUNTIME_ZIP%
  )
  echo Unzip %WIN_SDL2_IMAGE_RUNTIME_ZIP%
  mkdir %WIN_SDL2_IMAGE_RUNTIME_DIR%
  powershell -nologo -noprofile -command "& { $shell = New-Object -COM Shell.Application; $target = $shell.NameSpace('%WIN_SDL2_IMAGE_RUNTIME_DIR%'); $zip = $shell.NameSpace('%WIN_SDL2_IMAGE_RUNTIME_ZIP%'); $target.CopyHere($zip.Items(), 16); }"
  if not exist %WIN_SDL2_IMAGE_RUNTIME_DIR%\SDL2_image.dll (
    call echo Could not unzip
    EXIT /B 1
  )
  echo Done
)

:: https://www.libsdl.org/projects/SDL_mixer/
set WIN_SDL2_MIXER_VER=2.0.4
set WIN_SDL2_MIXER_DIR=%WIN_BIN%SDL2_mixer-%WIN_SDL2_MIXER_VER%
set WIN_SDL2_MIXER_ZIP=%WIN_BIN%SDL2_mixer-devel-%WIN_SDL2_MIXER_VER%-VC.zip
set WIN_SDL2_MIXER_URL=https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-%WIN_SDL2_MIXER_VER%-VC.zip
set WIN_SDL2_MIXER_RUNTIME_DIR=%WIN_BIN_RELEASE%SDL2_mixer-%WIN_SDL2_MIXER_VER%
set WIN_SDL2_MIXER_RUNTIME_ZIP=%WIN_BIN_RELEASE%SDL2_mixer-%WIN_SDL2_MIXER_VER%-win32-x64.zip
set WIN_SDL2_MIXER_RUNTIME_URL=https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-%WIN_SDL2_MIXER_VER%-win32-x64.zip
IF NOT EXIST "%WIN_SDL2_MIXER_DIR%" (
  if NOT EXIST "%WIN_SDL2_MIXER_ZIP%" (
    echo Downloading %WIN_SDL2_MIXER_ZIP%
    powershell -Command "(New-Object Net.WebClient).DownloadFile('%WIN_SDL2_MIXER_URL%', '%WIN_SDL2_MIXER_ZIP%')"
    if NOT EXIST "%WIN_SDL2_MIXER_ZIP%" (
      call echo Could not download
      EXIT /B 1
    )
  ) else (
    echo Already downloaded %WIN_SDL2_MIXER_ZIP%
  )
  echo Unzip %WIN_SDL2_MIXER_ZIP%
  powershell -nologo -noprofile -command "& { $shell = New-Object -COM Shell.Application; $target = $shell.NameSpace('%WIN_BIN%'); $zip = $shell.NameSpace('%WIN_SDL2_MIXER_ZIP%'); $target.CopyHere($zip.Items(), 16); }"
  if not exist %WIN_SDL2_MIXER_DIR% (
    call echo Could not unzip
    EXIT /B 1
  )
  echo Done
) else (
  echo Found %WIN_SDL2_MIXER_DIR%
)
:: runtime binaries
IF NOT EXIST "%WIN_SDL2_MIXER_RUNTIME_DIR%" (
  if NOT EXIST "%WIN_SDL2_MIXER_RUNTIME_ZIP%" (
    echo Downloading %WIN_SDL2_MIXER_RUNTIME_URL%
    powershell -Command "(New-Object Net.WebClient).DownloadFile('%WIN_SDL2_MIXER_RUNTIME_URL%', '%WIN_SDL2_MIXER_RUNTIME_ZIP%')"
    if NOT EXIST "%WIN_SDL2_MIXER_RUNTIME_ZIP%" (
      call echo Could not download
      EXIT /B 1
    )
  ) else (
    echo Already downloaded %WIN_SDL2_MIXER_RUNTIME_ZIP%
  )
  echo Unzip %WIN_SDL2_MIXER_RUNTIME_ZIP%
  mkdir %WIN_SDL2_MIXER_RUNTIME_DIR%
  powershell -nologo -noprofile -command "& { $shell = New-Object -COM Shell.Application; $target = $shell.NameSpace('%WIN_SDL2_MIXER_RUNTIME_DIR%'); $zip = $shell.NameSpace('%WIN_SDL2_MIXER_RUNTIME_ZIP%'); $target.CopyHere($zip.Items(), 16); }"
  if not exist %WIN_SDL2_MIXER_RUNTIME_DIR%\SDL2_mixer.dll (
    call echo Could not unzip
    EXIT /B 1
  )
  echo Done
)

rem call "%VS140COMNTOOLS%..\..\VC\bin\x64_amd64\vcvarsx64_amd64.bat"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo ====== msbuild version ======
msbuild -version
echo =============================

set SDL2_DIR=%WIN_SDL2_DIR%
set SDL2DIR=%WIN_SDL2_DIR%
set SDL2IMAGEDIR=%WIN_SDL2_IMAGE_DIR%
set SDL2TTFDIR=%WIN_SDL2_TTF_DIR%
set SDL2MIXERDIR=%WIN_SDL2_MIXER_DIR%

echo CMake configuring...
cmake -H. -Btmp\windows -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
rem > win-build.log 2>&1
if %ERRORLEVEL% GEQ 1 (
    echo Failed cmake prepare configuration. Log of build look in file win-build.log
    EXIT /B 1
)
echo Done.

echo Building...
cmake --build tmp\windows --config Release 
rem >> win-build.log 2>&1
if %ERRORLEVEL% GEQ 1 (
    echo Failed cmake build. Log of build look in file win-build.log
    EXIT /B 1
)
echo Done.

if not exist Release\SDL2.dll (
  xcopy /D /Y %WIN_SDL2_RUNTIME_DIR%\*.dll Release\
)

if not exist Release\SDL2_image.dll (
  xcopy /D /Y %WIN_SDL2_IMAGE_RUNTIME_DIR%\*.dll Release\
)

if not exist Release\SDL2_mixer.dll (
  xcopy /D /Y %WIN_SDL2_MIXER_RUNTIME_DIR%\*.dll Release\
)

xcopy /e /k /h /i /D /Y res Release\res

endlocal