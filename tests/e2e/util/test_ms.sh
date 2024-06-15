#!/bin/bash

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
awk '{print $NF}' $ms_temp/$ms_output >> $ms_temp/$ms_inm
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~//g' $ms_temp/$ms_inm 
tr -cd '[:print:]' < $ms_temp/$ms_inm >> $ms_temp/$ms_filter
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

sleep 1
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
			sleep 0.5
            bash -c "$var" &>>$ms_temp/$ms_output
        ;;
    esac
done
filter

