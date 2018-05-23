#!/bin/bash
PROYECT_PATH="/Users/davl3232/Documents/uni/tg/Ane-stent/"
INPUT_PATH=$PROYECT_PATH"/input.txt"
$PROYECT_PATH/tasks/build.sh
$PROYECT_PATH/build/main $INPUT_PATH