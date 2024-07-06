#!/bin/bash
WORKSPACE=/home/nande/work/repos/LifeDev
ORG=/home/nande/work/repos/LifeDev/Build
DST=/home/nande/work/repos/LifeDev/Assets/ExtraAssets/Builds/
ENGINE=/home/nande/work/UE5.4
PROJECT_NAME=LifeDev
CONFIG=Shipping
pushd $ENGINE || exit 1
#pushd $ORG || exit 1
# don't use this. it will recompile the WHOLE engine in shipping...
# $ENGINE/Engine/Build/BatchFiles/RunUAT.sh BuildCookRun -project="$WORKSPACE/$PROJECT_NAME.uproject" -noP4 -platform=Linux -clientconfig=$CONFIG -serverconfig=$CONFIG -clean -cook -allmaps -build -stage -pak -stage -stagingdirectory="$WORKSPACE/Build/"

popd && exit 0

pushd ORG || exit 1

mv ./Linux/LifeDev/Binaries/Linux/LifeDev-Linux-Test.debug ./

# todo compress file

# TODO move to dest
# # TODO add the date
popd
