:: @echo off
:: warning pushd HATES "/" we MUST use "\"
set UNREAL_ENGINE_ROOT=E:\ue\UE_5.8
set WORKSPACE=F:\ld
set PROJECT_NAME=LifeDev
set INSTALLED=-installed
:: set INSTALLED=
set MAPS=allmaps
:: cant use Test "targets cannot be built in test with this engine distribution" but "Development" will leave the debug messages on screen >_<
set CONFIG=Shipping
::set CONFIG=Development
::set CLEAN=-clean
set CLEAN=
::set DDC="-ddc=noshared" :: actually creates issues.
set DDC=""

:: Copy app icon
mkdir "%WORKSPACE%\Build\Windows\"
xcopy /y "%WORKSPACE%\Content\Omake\Application.ico" "%WORKSPACE%\Build\Windows\"

:: Build client
pushd %UNREAL_ENGINE_ROOT% || exit /b 1
call ./Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -project="%WORKSPACE%\%PROJECT_NAME%.uproject" -noP4 -platform=Win64 -clientconfig=%CONFIG% -serverconfig=%CONFIG% %clean% -cook -allmaps -build -stage -pak -stage -stagingdirectory="%WORKSPACE%\Build\" %DDC%
:: -AdditionalCookerOptions="-cookprocesscount=4"
popd


del "%WORKSPACE%\Build\Windows\LifeDev\Binaries\Win64\LifeDev-Win64-Shipping.pdb"

exit /b 0
:: archive does nothing that staging does.
:: -archive -archivedirectory="%WORKSPACE%/Build/"

