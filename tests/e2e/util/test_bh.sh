#!/bin/bash

#TODO
#make this plus test_ms one file
bash_temp=temp_bash
bash_output=bh_output.tmp
bash_inm=bh_inmp.tmp
bash_filter=bh_filter.tmp
ms_temp=temp_mini
ms_output=ms_output.tmp
ms_inm=ms_inmp.tmp
ms_filter=ms_filter.tmp

export WINDOW_ID

filter()
{
sed -i 's/\x1b\[?2004h[^$]*\$//g; s/\x1b\[?2004l//g; s/\x1b\[?2004h//g; s/^[ \t]*//; s/[ \t]*$//; s/\x1b\[0m//g' 2> /dev/null $bash_temp/$bash_output 
tr -cd '[:print:]' 2> /dev/null < $bash_temp/$bash_output >> $bash_temp/$bash_filter 
sed -i 's/[[:blank:]]//g' $bash_temp/$bash_filter 
}

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
			sleep 1
			if [[ "$var" == *"temp"* ]]; then
			mkdir -p $bash_temp
  			modified_var=$(echo $var | sed 's/temp/temp_bash\/temp/g')
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
filter
