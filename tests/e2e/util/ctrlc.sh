#!/bin/bash

sleep 0.2
x=1
while [ $x -le $1 ]
do
	(echo key ctrl+c | dotool) & 
  x=$(( $x + 1 ))
done