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
awk '{print $NF}' 2> /dev/null $bash_temp/$bash_output >> $bash_temp/$bash_inm 
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~//g' 2> /dev/null $bash_temp/$bash_inm 
tr -cd '[:print:]' 2> /dev/null < $bash_temp/$bash_inm >> $bash_temp/$bash_filter 
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
        *)
			sleep 0.001
			if [[ "$var" == *"temp"* ]]; then
			mkdir -p $bash_temp
  			modified_var=$(echo $var | sed 's/temp/temp_bash\/temp/g')
			bash -c "$modified_var"
			else
  			bash -c "$var" &>>$bash_temp/$bash_output
			fi
        ;;
    esac
done
filter
