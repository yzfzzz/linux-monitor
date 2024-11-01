#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

# https://github.com/abseil/abseil-cpp/archive/refs/tags/20200225.2.tar.gz
# Install abseil.
THREAD_NUM=$(nproc)
VERSION="3.4.10"
PKG_NAME="zookeeper-${VERSION}.tar.gz"

tar xzf "${PKG_NAME}"
pushd "zookeeper-${VERSION}"
    mv conf/zoo_sample.cfg conf/zoo.cfg
    ./bin/zkServer.sh start
    pushd "src/c"
        ./configure
        make
        make install
    popd
popd

ldconfig
rm -rf "${PKG_NAME}"