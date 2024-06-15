#!/bin/bash

sleep 1
x=1
while [ $x -le $1 ]
do
	bash -c "sleep 0.4 && ./$ydotool key 29:0 32:0" &
	bash -c "./$ydotool key 29:1 32:1" &
	sleep 1
  x=$(( $x + 1 ))
done
