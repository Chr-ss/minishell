#!/bin/bash

sleep 0.2
x=1
while [ $x -le $1 ]
do
	(xdotool key ctrl+c) & 
  x=$(( $x + 1 ))
done