#!/bin/bash

# Reference: https://github.com/aeruder/expect/blob/master/INSTALL
# Reference: https://gabrielstaples.com/ydotool-tutorial/#gsc.tab=0
# Reference: https://github.com/ReimuNotMoe/ydotool
# Reference: https://github.com/ReimuNotMoe/ydotool/issues/10
# Reference: https://github.com/ReimuNotMoe/ydotool/issues/170
# Reference: file:///usr/include/linux/input-event-codes.h
# Reference: https://gabrielstaples.com/ydotool-tutorial/#gsc.tab=0
# Reference: https://stackoverflow.com/questions/13242469/how-to-use-sed-grep-to-extract-text-between-two-words
# Reference: https://stackoverflow.com/questions/34412754/trying-to-remove-non-printable-characters-junk-values-from-a-unix-file
# Reference: https://stackoverflow.com/questions/67900936/use-cut-in-shell-to-extract-last-word
# Reference: https://stackoverflow.com/questions/6070995/how-to-make-valgrind-report-an-error-when-there-are-still-reachable-allocs
# Reference: https://stackoverflow.com/questions/255898/how-to-iterate-over-arguments-in-a-bash-script
# Reference: https://stackoverflow.com/questions/9057387/process-all-arguments-except-the-first-one-in-a-bash-script
# Reference: https://stackoverflow.com/questions/30873858/how-to-exit-if-statement-in-bash-without-exiting-program
# Reference: https://stackoverflow.com/questions/10319652/check-if-a-file-is-executable
# Reference: https://git.sr.ht/~geb/dotool/tree/master/doc/dotool.1.scd

#check dependencies
# if [[ $check == 1 ]];
# then 
# command -v VBoxManage
# vbox=$?
# if [[ $vbox == 0 ]];
# then 
# echo VBoxManage Installed
# else
# echo VBoxManage Not Installed
# cstatus=1
# fi
# command -v dotool
# do=$?
# if [ $do == 0 ]; 
# then 
# echo dotool Installed
# else
# echo dotool Not Installed
# cstatus=1
# fi
# if [ $cstatus == 1 ];
# then 
# exit 1
# fi
# exit 0
# fi


#initialize variables
RED="\x1B[31m"
GRN="\x1B[1;32m"
YEL="\x1B[33m"
BLU="\x1B[34m"
MAG="\x1B[35m"
BMAG="\x1B[1;35m"
CYN="\x1B[36m"
BCYN="\x1B[1;36m"
WHT="\x1B[37m"
RESET="\x1B[0m"
LINEP="\033[40G"
FAIL=false
LOG_DIR=logs
MS_LOG=ms.log
ESC=ESC
CLOSE=CLOSE
bash_temp=temp_bash
bash_output=bh_output.tmp
bash_inm=bh_inmp.tmp
bash_filter=bh_filter.tmp
ms_temp=temp_mini
ms_output=ms_output.tmp
ms_inm=ms_inmp.tmp
ms_filter=ms_filter.tmp
ctrlc=./util/ctrlc.sh
ctrld=./util/ctrld.sh
test_bh=./util/test_bh.sh
test_ms=./util/test_ms.sh
bash_outfiles=./outfiles
mini_outfiles=./mini_outfiles
minishell=$(find ../../../ -type f -name minishell)
minishelldir=$(find ../../../ -type d -name minishell)
cases="./cases"
rm -rf logs
mkdir -p logs
rm -rf $bash_temp
mkdir -p $bash_temp
rm -rf $ms_temp
mkdir -p $ms_temp
rm -rf $bash_outfiles
mkdir -p $bash_outfiles
rm -rf $mini_outfiles
mkdir -p $mini_outfiles

#export variables used in other scripts
export bash_output
export bash_inm
export bash_filter
export ms_output
export ms_inm
export ms_filter
export bash_temp
export ms_temp
export ctrlc
export ctrld
export minishell

#prepare minishell
make -C $minishelldir re

#https://stackoverflow.com/questions/30137135/confused-about-docker-t-option-to-allocate-a-pseudo-tty
#https://gist.github.com/janert/e1d8e6ae74a8c94173ef35fa356ce2da

apt-get update
apt-get install -y sudo
sudo apt-get install -y xdotool

# #prepare dotool
# dpkg -l | grep libxkbcommon-dev
# xkb=$?
# if [ $xkb != 0 ]; 
# then
# sudo apt install -y libxkbcommon-dev
# fi
# dpkg -l | grep scdoc
# sc=$?
# if [ $sc != 0 ]; 
# then
# sudo apt install -y scdoc
# fi
# dpkg -l | grep golang-go
# go=$?
# if [ $go != 0 ]; 
# then
# sudo apt install -y golang-go
# fi
# sudo apt-get install -y udev
# command -v dotool
# do=$?
# if [ $do != 0 ]; 
# then
# (cd dotool && ./build.sh && sudo ./build.sh install)
# (cd dotool && sudo udevadm control --reload && sudo udevadm trigger)
# fi
#truncate logs
truncate -s 0 $LOG_DIR/$MS_LOG

#FUNCTIONS
remove_temp_files()
{
	rm -rf $bash_temp/*
	rm -rf $ms_temp/*
}

check_result()
{
diff $bash_temp/$bash_filter $ms_temp/$ms_filter &>> $LOG_DIR/$MS_LOG
dstatus=$?
ARG=$1
if [ $dstatus == 0 ];
	then 
	printf "${BMAG}${ARG}${LINEP}${GRN}OK \n${RESET}";
	else
	printf "${BMAG}${ARG}${LINEP}${RED}FAIL \n${RESET}";
fi
}

test()
{
bash $test_bh "$@" &
bash -c "bash -i &>>$bash_temp/$bash_output"
bash $test_ms "$@" &
bash -c "bash -i &>>$ms_temp/$ms_output"
}

check_multiple_files ()
{
ARG=$1
for var in "${@:2}"
do
diff $bash_temp/$var $ms_temp/$var &>> $LOG_DIR/$MS_LOG
dstatus=$?
if [ $dstatus != 0 ];
	then 
	printf "${BMAG}${ARG}:file:$var${LINEP}${RED}FAIL \n${RESET}";
	return
fi
done
	printf "${BMAG}${ARG}${LINEP}${GRN}OK \n${RESET}";
}

check_result_multiple_files ()
{
diff $bash_temp/$bash_filter $ms_temp/$ms_filter &>> $LOG_DIR/$MS_LOG
rstatus=$?
ARG=$1
if [ $rstatus != 0 ];
	then 
	printf "${BMAG}${ARG}:file:$bash_filter:$ms_filter${LINEP}${RED}FAIL \n${RESET}";
	return
fi
for var in "${@:2}"
do
diff $bash_temp/$var $ms_temp/$var &>> $LOG_DIR/$MS_LOG
mstatus=$?
if [ $mstatus != 0 ];
	then 
	printf "${BMAG}${ARG}:file:$var${LINEP}${RED}FAIL \n${RESET}";
	return
fi
done
printf "${BMAG}${ARG}${LINEP}${GRN}OK \n${RESET}";
}


# if [ $FAIL = true ];
# then echo -e "${RED}Check logs/*.log for errors${RESET}"
# else 
# echo -e "${GRE}Congratulations, all tests are succesfull :)${RESET}"
# rm -rf $LOG_DIR
# fi

test "echo lol" "ctrl+c" "ctrl+d"
# check_result_multiple_files 1 "temp1" "temp2"
# remove_temp_files
# test "ctrl+c" "ctrl+c" "ctrl+c" "ctrl+d"
check_result 1


# rm -rf $bash_temp
# rm -rf $ms_temp

# exit 0

