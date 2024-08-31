#!/usr/bin/env bash

# 1. Convert an *.mp4 file to H264 *.mov and apply basic filters 
time ffmpeg -i input.mp4 -vf "eq=saturation=1.25:brightness=0.5" -c:v libx264 -threads 4 output.mov | tee ffmpeg-out.txt

