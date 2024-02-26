#!/usr/bin/bash

CMAKE=$(which cmake)
TARGETS="testlfqueuelib testloggerlib testmemlib testthreadlib"
echo "Building test targets: $TARGETS"
$CMAKE --build --preset Debug --target $TARGETS