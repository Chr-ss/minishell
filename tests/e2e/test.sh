
for i in $1
do
sh -c 'sleep 0.51 && ./ydotool/build/ydotool key 29:0 32:0' &
sh -c './ydotool/build/ydotool key 29:1 32:1' &
sleep 2
done