#!/bin/bash
PROYECT_PATH="/Users/davl3232/Documents/uni/tg/Ane-stent/"
cmake $PROYECT_PATH $PROYECT_PATH/build/ -DCMAKE_BUILD_TYPE=Debug
make -C $PROYECT_PATH/build/