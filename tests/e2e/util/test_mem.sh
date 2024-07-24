#!/bin/bash



sleep 1
ctrlc()
{
bash $ctrlc 1 
mstatus=$?
}

ctrld()
{
bash $ctrld 1 
mstatus=$?
}

command()
{
bash $cmmd $1 
mstatus=$?
}

for var in "$@"
do
case $var in
        ctrl+d)
            ctrld
        ;;
        ctrl+c)
            ctrlc
        ;;
		ctrl+backslash)
			LD_LIBRARY_PATH=xdotool xdotool key --window $WINDOW_ID ctrl+backslash 
        ;;
        *)
			sleep 0.001
            if [[ "$var" == *"temp"* ]]; then
			mkdir -p $ms_temp
  			modified_var=$(echo $var | sed 's/temp/temp_mini\/temp/g')
			LD_LIBRARY_PATH=xdotool xdotool type --window $WINDOW_ID "$modified_var"
			sleep 0.0001
			LD_LIBRARY_PATH=xdotool xdotool key --window $WINDOW_ID Return 
			else
			LD_LIBRARY_PATH=xdotool xdotool type --window $WINDOW_ID "$var" 
			sleep 0.0001
			LD_LIBRARY_PATH=xdotool xdotool key --window $WINDOW_ID Return 
			fi
        ;;
    esac
done

