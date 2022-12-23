#! /bin/bash

#
# runtest.sh
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

# -----------------------------------------------------------------------------
# Die with message
die() {
    echo >&2 "$@"
    exit 1
}

# Run command and check exit code
run() {
    echo "$@" && "$@"
    code=$?
    [[ $code -ne 0 ]] && die "[$@] failed with error code $code"
    return 0
}

# Ask for input query
ask() {
    echo -n "$@ (y/n [n]): "
    local ans
    read ans
    [[ "$ans" != "y" ]] && return 1
    return 0
}

# -----------------------------------------------------------------------------
# Test core
pushd core
run make clean  > /dev/null
run make -j48 all > /dev/null
run ./test.out < data/in.lorem_ipsum.txt
run make clean  > /dev/null

run diff data/in.lorem_ipsum.txt         data/out.lorem_ipsum_new.txt
run diff data/out.lorem_ipsum_csv_1.txt  data/out.lorem_ipsum_csv_2.txt
run diff data/out.lorem_ipsum_new_1.txt  data/out.lorem_ipsum_new_2.txt
run diff data/out.lorem_ipsum_1.bin      data/out.lorem_ipsum_2.bin

rm -v data/out.*
popd

# -----------------------------------------------------------------------------
# Test math
pushd math
run make clean  > /dev/null
run make -j48 all > /dev/null
run ./test.out
run make clean  > /dev/null

pushd data
[[ -x ./plot_ent.bash ]] && ./plot_ent.bash
popd
popd

# -----------------------------------------------------------------------------
# Test opencl/opengl examples
execute() {
    pushd "${1}"
    run make -f ../Makefile clean
    run make -f ../Makefile -j16 all
    run ./test.out
    run make -f ../Makefile clean
    popd
}
pushd opencl
# execute 0-helloworld
execute 1-platform
execute 2-context
execute 3-math
execute 4-vector
execute 5-matrix
popd

pushd opengl
execute 0-image
execute 1-glfw
execute 2-triangle
execute 2-triangle-instance-a
execute 2-triangle-instance-b
execute 2-triangle-instance-c
execute 3-quad
execute 4-quad-image
execute 5-sphere-image
execute 6-bunny
execute 7-panorama
execute 8-framebuffer
execute 9-iobuffer
popd
