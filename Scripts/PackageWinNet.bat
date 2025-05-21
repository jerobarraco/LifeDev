:: @echo off
:: warning pushd HATES "/" we MUST use "\"
set UNREAL_ENGINE_ROOT=E:\ue\UE_5.5
set WORKSPACE=F:/LifeDev
set PROJECT_NAME=LifeDev
set INSTALLED=-installed
:: set INSTALLED=
set MAPS=allmaps
:: cant use Test "targets cannot be built in test with this engine distribution" but "Development" will leave the debug messages on screen >_<
set CONFIG=Shipping
::set CONFIG=Development
::set CLEAN=-clean
set CLEAN=
set DDC="-ddc=noshared"

:: Build client
pushd %UNREAL_ENGINE_ROOT% || exit /b 1
call ./Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" -noP4 -platform=Win64 -clientconfig=%CONFIG% -serverconfig=%CONFIG% %clean% -cook -allmaps -build -stage -pak -stage -stagingdirectory="%WORKSPACE%/Build/" %DDC%
:: -AdditionalCookerOptions="-cookprocesscount=4"
popd
exit /b 0
:: archive does nothing that staging does.
:: -archive -archivedirectory="%WORKSPACE%/Build/"



:: old stuff
pushd %WORKSPACE%


call "%UNREAL_ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" -target=%PROJECT_NAME% %INSTALLED% -platform=Win64 -allmaps -skipbuildeditor -utf8output -build -cook -stage -pak -prereqs -package -compressed -archive -archivedirectory="%WORKSPACE%/Build/" -clientconfig=%CONFIG%
:: -nocompile -clean  -nocompile -nocompileeditor -nocompileuat 
:: -target=LifeDev

::call "%UNREAL_ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" %INSTALLED% -platform=Win64 -configuration=Development -allmaps -nocompileeditor -skipbuildeditor -unattended -utf8output -clean -build -cook -stage -pak -prereqs -package -archive -target=LifeDev -archivedirectory="%WORKSPACE%/Build/" -clientconfig=Development -nocompile -nocompileuat 

 :: "/home/nande/work/UE5.2/Engine/Build/BatchFiles/RunUAT.sh"  -ScriptsForProject="/home/nande/work/repos/LifeDev/LifeDev.uproject" Turnkey -command=VerifySdk -platform=Linux -UpdateIfNeeded -EditorIO -EditorIOPort=36941  -project="/home/nande/work/repos/LifeDev/LifeDev.uproject" BuildCookRun -nop4 -utf8output -nocompileeditor -skipbuildeditor -cook  -project="/home/nande/work/repos/LifeDev/LifeDev.uproject" -target=LifeDev  -unrealexe="/home/nande/work/UE5.2/Engine/Binaries/Linux/UnrealEditor-Linux-DebugGame" -platform=Linux -stage -archive -package -build -pak -iostore -compressed -prereqs -archivedirectory="/home/nande/work/repos/LifeDev/Build/" -clientconfig=Development -nocompile -nocompileuat 


:: "%UNREAL_ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%\LifeDev.uproject" -target=LifeDev -noP4 -platform=Win64 -allmaps -game -clientconfig=Development -build -package -cook -pak -stage -CrashReporter -installed
::  -nocompileeditor -skipbuildeditor

:: -cook -package -build -clean
popd

exit /b 0
