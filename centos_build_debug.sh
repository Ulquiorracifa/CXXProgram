#!/bin/bash
### configure C compiler
export compiler=$(which gcc)

MAJOR=$(echo __GNUC__ | $compiler -E -xc - | tail -n 1)
MINOR=$(echo __GNUC_MINOR__ | $compiler -E -xc - | tail -n 1)
PATCHLEVEL=$(echo __GNUC_PATCHLEVEL__ | $compiler -E -xc - | tail -n 1)
echo $MAJOR $MINOR $PATCHLEVEL
if [ $MAJOR -lt 8 ];
then
        yum install centos-release-scl -y
        echo scl install success
        yum install devtoolset-8 -y
        echo gcc install success
        source /opt/rh/devtoolset-8/enable
        echo gcc8 enabled
fi
source /opt/rh/devtoolset-8/enable

if [ ! -d "myfolder" ]; then
  mkdir _generate
fi

mkdir build_debug
cd build_debug

cmake .. -G "Unix Makefiles"  -DCMAKE_BUILD_TYPE=Debug
cmake --build .
cd ..
