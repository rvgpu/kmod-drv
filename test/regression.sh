#! /usr/bin/env bash

proj_path=${PWD}
proj_dirname=`basename ${PWD}`

rm -rf ${proj_path}/regression
mkdir -p ${proj_path}/regression

pushd ${proj_path}/regression
    cmake ${proj_path}/test

    make -j `nproc`

    make test
popd
