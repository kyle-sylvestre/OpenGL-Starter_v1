@echo off
REM This batch file assumes that you have Visual C++ installed
REM on your system and that you have properly set up your
REM environment for command-line operation of the Visual C++
REM compile.  This is normally done by running the VCVARS32.BAT
REM batch file.

REM For Visual C++ 4.2, VCVARS32.BAT is usually installed in
REM c:\msdev\bin

REM For Visual C++ 5.0, VCVARS32.BAT is usually installed in
REM c:\Program Files\DevStudio\VC\bin

REM For Visual C++ 6.0, VCVARS32.BAT is usually installed in
REM c:\Program Files\Microsoft Visual Studio\VC98\bin

REM You can also set the appropriate environment variables
REM set n VCVARS32.BAT in your AUTOEXEC.BAT file to avoid 
REM running VCVARS32.BAT.

REM Windows 95 and 98 users may find it necessary to expand
REM the default environment memory space.  This can be done
REM by adding or updating the following lines to your CONFIG.SYS
REM file:
REM
REM [shell]
REM SHELL=c:\command.com /e:8192

nmake nodebug=1 -f Makefile.win %1 %2 %3 %4 %5 %6 %7 %8 %9
