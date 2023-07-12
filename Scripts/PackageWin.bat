@echo off
set UNREAL_ENGINE_ROOT=E:\ue\UE_5.1
set WORKSPACE=G:
set PROJECT_NAME=LifeDev
set INSTALLED=-installed
set MAPS=allmaps

pushd %WORKSPACE%
call Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" %INSTALLED% -platform=Win64 -configuration=Development -allmaps -nocompileeditor -unattended -utf8output -clean -build -cook -stage -pak -prereqs -package -archive -archivedirectory="%WORKSPACE%/Build/PackagedGame"
popd


:: "%UNREAL_ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%\LifeDev.uproject" -target=LifeDev -noP4 -platform=Win64 -allmaps -game -clientconfig=Development -build -package -cook -pak -stage -CrashReporter -installed
:: -nocompileeditor -skipbuildeditor

:: -cook -package -build -clean
exit /b 0
