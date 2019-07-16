#!/bin/bash

git pull origin dev
cd ../
mkdir build
cd build
qmake ../RepairCards/RepairCards.pro
mingw32-make -j8

mkdir ../deploy
cd ../deploy
mv ../build/release/RepairCards.exe ./
windeployqt RepairCards.exe





