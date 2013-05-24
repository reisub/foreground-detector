#!/bin/bash

for i in $HOME/Downloads/video_zavrsni/walking/*.avi
do
echo $i
./detector "$i"
done
