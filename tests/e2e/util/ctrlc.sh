#!/bin/bash

sleep 1
x=1
while [ $x -le $1 ]
do
	bash -c "sleep 0.4 && ./$ydotool key 29:0 46:0" &
	bash -c "./$ydotool key 29:1 46:1" &
	sleep 1
  x=$(( $x + 1 ))
done