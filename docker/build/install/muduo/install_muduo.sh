#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"
THREAD_NUM=$(nproc)
PKG_NAME="muduo-master.zip"
FILE_NAME="muduo-master"

unzip "${PKG_NAME}"
pushd "${FILE_NAME}"
    ./build.sh
    ./build.sh install
popd
pushd "build"
    pushd "release-install-cpp11"
        mv include/muduo/ /usr/include/
        mv lib/* /usr/local/lib/
    popd
popd
ldconfig
rm -rf "${FILE_NAME}" "${PKG_NAME}"