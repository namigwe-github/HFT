#!/usr/bin/bash

# Configure and Build

CMAKE=$(which cmake)
BUILDTOOL=$(which make)
TESTBINDIR=$BUILD_DIR/tests
echo "CMAKE_DIR: $CMAKE"

BUILD_DIR=build
echo "BUILD_DIR: $BUILD_DIR"
SRC_DIR="."
echo "SRC_DIR: $SRC_DIR"

echo "Configuring..."

$CMAKE . --preset make

echo "Building..."

$CMAKE --build --preset Debug --clean-first


echo "Done Building"

echo "Running..." 
#cd $BUILD_DIR && $BUILDTOOL 
#&& ./tests/testlfqueuelib