:: @echo off
:: warning pushd HATES "/" we MUST use "\"
set UNREAL_ENGINE_ROOT=E:\ue\UE_5.5
:: improved compilation process by compiling directly into the network unit
set WORKSPACE=E:/w/LifeDev
set PROJECT_NAME=LifeDev
set INSTALLED=-installed
:: set INSTALLED=
set MAPS=allmaps
:: cant use Test "targets cannot be built in test with this engine distribution" but "Development" will leave the debug messages on screen >_<
set CONFIG=Shipping
set DDC="-ddc=noshared"


:: Build client
pushd %UNREAL_ENGINE_ROOT% || exit /b 1
call ./Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -project="%WORKSPACE%/%PROJECT_NAME%.uproject" -noP4 -platform=Win64 -clientconfig=%CONFIG% -serverconfig=%CONFIG% -cook -allmaps -build -stage -pak -stage -stagingdirectory="%WORKSPACE%/Build/" -AdditionalCookerOptions="-cookprocesscount=4" %DDC%

popd
exit /b 0
:: archive does nothing that staging does.
:: -archive -archivedirectory="%WORKSPACE%/Build/"
