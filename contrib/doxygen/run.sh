#!/bin/bash

APP_VERSION=$(cat ../../wsjcpp.yml | grep -Po '^(?<=version\: "v)\d+\.\d+\.\d+".*')
echo "Version=$APP_VERSION"

rm doxygen_objdb_*.tmp
rm doxygen_entrydb_*.tmp

export APP_VERSION="$APP_VERSION" 

doxygen
