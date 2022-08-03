@echo off

rem call script for devenv, msbuild, and cl
if "%INCLUDE%"=="" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
)

rem push old cwd and set it to the batch file's directory 
pushd "%~dp0"
set EXE=output\app.exe
set SRC=src\main.cpp
set LIBS=winmm.lib user32.lib gdi32.lib opengl32.lib glu32.lib glut\Win32_2010\Debug\glutstatic.lib 

if "%1"=="clean" (
    rmdir /s /q intermediate
    rmdir /s /q output
    if "%2"=="all" (
        msbuild glut\glut_2010.vcxproj /t:clean /p:Configuration=Debug /p:Platform=X86
    )
) 

if "%1"=="" (

    if not exist intermediate mkdir intermediate
    if not exist output mkdir output
    if not exist "glut\Win32_2010\Debug\glutstatic.lib" (
        rem TODO: how to /DGLUT_BUILDING_LIB in the command line
        msbuild.exe glut\glut_2010.vcxproj /p:Configuration=Debug /p:Platform=X86 /m
    )

    rem delete exe bc you don't shortcircuit in "make && output.exe"
    del %EXE% 2>NUL

    cl.exe -W4 -MP -Zi -Od %SRC% -Iglut\include\GL -Fo".\intermediate\\" -Fe"%EXE%" -link %LIBS%
)

if "%1"=="debug" (
    rem for /F "tokens=1,2" %%i in ('tasklist /FI "IMAGENAME eq %EXE%" /fo table /nh') do set pid=%%j
    rem echo %pid%
    call dev.bat
    devenv.exe %EXE%
) 

if "%1"=="tags" (
    ctags.exe -R --exclude=*.tex --c++-kinds=+p --fields=+iaS --extras=+q
)

rem restore from the cwd stack
popd
