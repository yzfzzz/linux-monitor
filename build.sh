#!/bin/bash

if [ -d "build/display_monitor" ]; then
    echo "Build directory already exists."
    rm -r build/display_monitor
else
    echo "Created build directory."
fi

mkdir build/display_monitor
cd build
cmake ..
make -j2