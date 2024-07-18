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

filter()
{
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~\$//g' 2> /dev/null $ms_temp/$ms_output 
tr -cd '[:print:]' 2> /dev/null < $ms_temp/$ms_output >> $ms_temp/$ms_filter
sed -i 's/[[:blank:]]//g' $ms_temp/$ms_filter
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
			LD_LIBRARY_PATH=xdotool xdotool key ctrl+backslash
        ;;
        *)
			sleep 0.001
            if [[ "$var" == *"temp"* ]]; then
			mkdir -p $ms_temp
  			modified_var=$(echo $var | sed 's/temp/temp_mini\/temp/g')
			LD_LIBRARY_PATH=xdotool xdotool type "$modified_var"
			sleep 0.0001
			LD_LIBRARY_PATH=xdotool xdotool key Return
			else
			LD_LIBRARY_PATH=xdotool xdotool type "$var"
			sleep 0.0001
			LD_LIBRARY_PATH=xdotool xdotool key Return
			fi
        ;;
    esac
done
filter

