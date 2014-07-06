#!/bin/bash

project=platform-abstraction

modules=(
    platform-abstraction
)

build_configs=(
    tests
)

build_dir="../build-$project"

# DO NOT EDIT BELOW THIS LINE

echo "Starting build process in $build_dir"

if [[ ! -d "../build-system/" ]]; then
    pushd .. > /dev/null
    git clone https://github.com/cvra/build-system
    popd > /dev/null
else
    pushd ../build-system > /dev/null
    git pull
    popd > /dev/null
fi

ln -s $build_dir/build/tests run-tests

cd ../build-system/ ;

echo "Configuration finished - starting serious stuff"

source ./cvraMake.sh
