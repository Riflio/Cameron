#!/bin/bash
mkdir -p /APP/build-Cameron
cd /APP/build-Cameron
rm -rf *
/opt/QtDev/install_LinAmd64/bin/qmake /APP/Cameron.pro
make -j10 VERBOSE=1
cp /opt/QtDev/install_LinAmd64/lib/libQt6Core.so ./
cp /opt/QtDev/install_LinAmd64/lib/libQt6Core.so.6 ./
cp /opt/QtDev/install_LinAmd64/lib/libQt6Core.so.6.8.0 ./
cp /opt/QtDev/install_LinAmd64/lib/libQt6Network.so ./
cp /opt/QtDev/install_LinAmd64/lib/libQt6Network.so.6 ./
cp /opt/QtDev/install_LinAmd64/lib/libQt6Network.so.6.8.0 ./
cp /usr/lib/x86_64-linux-gnu/libcrypto.so.3 ./
# Plugins
mkdir plugins-dist && cd plugins-dist
/opt/QtDev/install_LinAmd64/bin/qmake /APP/plugins-dist/CameronRecorder/CameronRecorder.pro
make -j10 VERBOSE=1

