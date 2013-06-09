#!/bin/bash

dir="*"

if [ $# -eq 1 ]
then
    dir="$1"
fi

for i in $HOME/Downloads/video_zavrsni/$dir/*.avi
do
    echo $i
    ./detector "$i"
done
