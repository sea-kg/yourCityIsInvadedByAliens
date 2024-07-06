#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo ""
        echo "!!! FAIL: $2"
        echo "********************************************************************************"
        echo ""
        exit $1
    else
        echo ""
        echo "*** SUCCESS: $2"
        echo "********************************************************************************"
        echo ""
    fi
}

if [ ! -d tmp/linux ]; then
    mkdir -p tmp/linux
fi

cmake -H. -B./tmp/linux -DCMAKE_BUILD_TYPE=Release
check_ret $? "configure"

cmake --build ./tmp/linux --config Release
check_ret $? "build"


cd ./tmp/linux/tests/ && ctest --output-on-failure