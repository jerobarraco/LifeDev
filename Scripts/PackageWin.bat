@echo off
set UNREAL_ENGINE_ROOT=E:\ue\UE_5.2
set WORKSPACE=E:\w\LifeDev
set PROJECT_NAME=LifeDev
set INSTALLED=-installed
set MAPS=allmaps

pushd %WORKSPACE%

call "%UNREAL_ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" %INSTALLED% -platform=Win64 -configuration=Development -allmaps -nocompileeditor -skipbuildeditor -unattended -utf8output -clean -build -cook -stage -pak -prereqs -package -archive -target=LifeDev -archivedirectory="%WORKSPACE%/Build/" -clientconfig=Development  -nocompile -nocompileuat

:: call Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" %INSTALLED% -platform=Win64 -configuration=Development -allmaps -nocompileeditor -skipbuildeditor -unattended -utf8output -clean -build -cook -stage -pak -prereqs -package -archive -target=LifeDev -archivedirectory="%WORKSPACE%/Build/" -clientconfig=Development -nocompile -nocompileuat 

 :: "/home/nande/work/UE5.2/Engine/Build/BatchFiles/RunUAT.sh"  -ScriptsForProject="/home/nande/work/repos/LifeDev/LifeDev.uproject" Turnkey -command=VerifySdk -platform=Linux -UpdateIfNeeded -EditorIO -EditorIOPort=36941  -project="/home/nande/work/repos/LifeDev/LifeDev.uproject" BuildCookRun -nop4 -utf8output -nocompileeditor -skipbuildeditor -cook  -project="/home/nande/work/repos/LifeDev/LifeDev.uproject" -target=LifeDev  -unrealexe="/home/nande/work/UE5.2/Engine/Binaries/Linux/UnrealEditor-Linux-DebugGame" -platform=Linux -stage -archive -package -build -pak -iostore -compressed -prereqs -archivedirectory="/home/nande/work/repos/LifeDev/Build/" -clientconfig=Development -nocompile -nocompileuat 


:: "%UNREAL_ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%\LifeDev.uproject" -target=LifeDev -noP4 -platform=Win64 -allmaps -game -clientconfig=Development -build -package -cook -pak -stage -CrashReporter -installed
::  -nocompileeditor -skipbuildeditor

:: -cook -package -build -clean
popd

exit /b 0
