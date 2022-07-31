@echo off
subst w: .
rem subst w: /D
set EXE=w:\output\app.exe
set SRC=w:\src\3DSage_Starter.cpp
set LIBS=winmm.lib user32.lib gdi32.lib opengl32.lib glu32.lib w:\glut\Win32_2010\Debug\glutstatic.lib 

if "%1"=="clean" (
    del .\intermediate\* /Q
    del .\output\* /Q
    rem msbuild glut\glut_2010.vcxproj /t:clean /p:Configuration=Debug /p:Platform=X86
) 

if "%1"=="" (
    rem TODO: how to /DGLUT_BUILDING_LIB in msbuild line
    rem msbuild w:\glut\glut_2010.vcxproj /p:Configuration=Debug /p:Platform=X86 /m
    call dev.bat
    pushd intermediate
    cl -MP -Zi %SRC% -Iw:\glut\include\GL -Fe"%EXE%" -link %LIBS%
    popd
)

if "%1"=="debug" (
    rem for /F "tokens=1,2" %%i in ('tasklist /FI "IMAGENAME eq %EXE%" /fo table /nh') do set pid=%%j
    rem echo %pid%
    call dev.bat
    devenv.exe %EXE%
) 

