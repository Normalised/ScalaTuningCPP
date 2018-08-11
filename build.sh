#!/bin/sh
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)

set -e

mkdir -p build
cd build

MACOSX_DEPLOYMENT_TARGET=10.10

# Generate a Makefile for GCC (or Clang, depanding on CC/CXX envvar)
cmake -DCMAKE_BUILD_TYPE=Debug ..

cmake --build .

# Build and run unit-tests (ie 'make test')
# ctest --output-on-failure
