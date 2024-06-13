#!/bin/bash

temp=$(find ../../../ -type d -name $ms_temp)
ctrlc=$(find . -name ctrlc.sh)
ctrld=$(find . -name ctrld.sh)
minishell=$(find ../../../ -type f -name minishell)

filter()
{
awk '{print $NF}' $temp/$ms_output >> $temp/$ms_inm
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~//g' $temp/$ms_inm 
tr -cd '[:print:]' < $temp/$ms_inm >> $temp/$ms_filter
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
            bash -c "$var" &>>$temp/$ms_output
        ;;
    esac
done
filter

