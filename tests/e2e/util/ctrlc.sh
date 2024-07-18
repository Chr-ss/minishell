#!/bin/bash

sleep 0.2
x=1
while [ $x -le $1 ]
do
	(LD_LIBRARY_PATH=xdotool xdotool key ctrl+c) & 
  x=$(( $x + 1 ))
done