#!/bin/bash

temp=$(find ../../../ -type d -name temp)
ctrlc=$(find . -name ctrlc.sh)
ctrld=$(find . -name ctrld.sh)
minishell=$(find ../../../ -type f -name minishell)

filter()
{
awk '{print $NF}' $temp/$bash_output >> $temp/$bash_inm
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~//g' $temp/$bash_inm 
tr -cd '[:print:]' < $temp/$bash_inm >> $temp/$bash_filter
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
			bash -c "$var" &>>$temp/$bash_output
        ;;
    esac
done
filter
