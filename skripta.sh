#!/bin/bash

for i in $HOME/Downloads/video_zavrsni/handclapping/*.avi
do
echo $i
./detector "$i"
done
