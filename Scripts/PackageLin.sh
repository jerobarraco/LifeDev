#!/bin/bash

ORG=/home/nande/work/LifeDev/Build
DST=/home/nande/work/repos/LifeDev/Assets/ExtraAssets/Builds/

pushd ORG || exit 1

mv ./Linux/LifeDev/Binaries/Linux/LifeDev-Linux-Test.debug ./

# todo compress file

# TODO move to dest
# # TODO add the date
popd
