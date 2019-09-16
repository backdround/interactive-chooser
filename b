#!/bin/bash

mkdir -p "$(dirname "$0")/build"
DESTDIR="$(dirname "$0")/build"
cd "$DESTDIR"
ln -sf ../main.qml
qmake ../main.pro && bear make && ./main

