#!/bin/bash
mkdir -p /APP/build-Cameron
cd /APP/build-Cameron
rm -rf *
/opt/Qt/Qt640/install_LinAmd64/bin/qmake /APP/Cameron.pro
make -j10 VERBOSE=1
