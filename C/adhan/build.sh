#!/bin/bash

set -e
set -v

test -d .repo/DownloadProject || git clone https://github.com/Crascit/DownloadProject.git .repo/DownloadProject

rm -rf build
mkdir build
cd build
cmake ..
make -j
