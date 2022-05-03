#!/bin/bash
mkdir -p /APP/build-Cameron
cd /APP/build-Cameron
rm -rf *
/opt/Qt/Qt630/install/bin/qmake /APP/Cameron.pro
make -j10 VERBOSE=1
