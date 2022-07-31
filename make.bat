call dev.bat
set EXE=3DSage_Starter.exe

if "%1"=="clean" (
    del *.pdb
    del *.obj
    del *.exe
    rem msbuild glut\glut_2010.vcxproj /t:clean /p:Configuration=Debug /p:Platform=X86
) 

if "%1"=="" (
    set LIBS=winmm.lib user32.lib gdi32.lib opengl32.lib glu32.lib glut\Win32_2010\Debug\glutstatic.lib 
    rem msbuild glut\glut_2010.vcxproj /p:DefineConstants=GLUT_BUILDING_LIB /p:Configuration=Debug /p:Platform=X86 /m
    cl -VERBOSE -MP -Zi 3DSage_Starter.cpp -Iglut\include\GL -link %LIBS%
)

if "%1"=="debug" (
    rem for /F "tokens=1,2" %%i in ('tasklist /FI "IMAGENAME eq %EXE%" /fo table /nh') do set pid=%%j
    rem echo %pid%
    devenv.exe %EXE%
) 

