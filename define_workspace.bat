@echo off

call :Input "Define cocos2d-x dir: "
set Dir="%INPUT%"

setx LIB_COCOS2DX %Dir% 


:Input 
set INPUT=
set /P INPUT=%~1 %=%
if "%INPUT%"=="" goto Input
goto:eof


sleep 2
close()

