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

#initialize variables
#colors
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

#logs
LOG_DIR=logs
INTER_LOG=interactive.log
INTER_DIFF_LOG=interactive_diff.log
INTER_MEM_LOG=interactive_memory.log

#keys
ESC=ESC
CLOSE=CLOSE

#files
bash_temp=temp_bash
bash_output=bh_output.tmp
bash_inm=bh_inmp.tmp
bash_filter=bh_filter.tmp
ms_temp=temp_mini
ms_output=ms_output.tmp
ms_inm=ms_inmp.tmp
ms_filter=ms_filter.tmp
suppressions=./util/valgrind_suppresion
cases=./cases/interactive

#valgrind
valgrind_cmd="valgrind --error-exitcode=42 --leak-check=full --show-leak-kinds=all --suppressions=$suppressions"

#utils
ctrlc=./util/ctrlc.sh
ctrld=./util/ctrld.sh
test_bh=./util/test_bh.sh
test_ms=./util/test_ms.sh
test_mem=./util/test_mem.sh

#minishell
minishell=$(find ../../../ -type f -name minishell)
minishelldir=$(find ../../../ -type d -name minishell)

#prepare variables
memstatus=0
mem=true
if [[ $2 == 0 ]]; then
mem=false
fi

#prepare files
rm -rf $bash_temp
mkdir -p $bash_temp
rm -rf $ms_temp
mkdir -p $ms_temp

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
if [[ $1 == 1 ]];
then 
:
else 
make -C $minishelldir re
fi

rm -rf $LOG_DIR/$INTER_MEM_LOG

#https://stackoverflow.com/questions/30137135/confused-about-docker-t-option-to-allocate-a-pseudo-tty
#https://gist.github.com/janert/e1d8e6ae74a8c94173ef35fa356ce2da

#prepare xdotool
make -C ./xdotool &> /dev/null

#add xdotool to path
export PATH=$PATH:$(cd ./xdotool && pwd)
WINDOW_ID=$(LD_LIBRARY_PATH=xdotool xdotool getactivewindow)
export WINDOW_ID

#truncate logs

#FUNCTIONS
remove_temp_files()
{
	rm -rf $bash_temp/*
	rm -rf $ms_temp/*
}

check_result()
{
diff $bash_temp/$bash_filter $ms_temp/$ms_filter &>> $LOG_DIR/$INTER_DIFF_LOG
dstatus=$?
ARG=$1
if [[ $dstatus == 0 && $memstatus != 42 ]];
	then 
	printf " ✅\n"
	else
	printf " ❌"
	if [ $dstatus != 0 ];
	then echo -ne "${RED} diff error ${RESET}"
	fi 
	if [ $memstatus == 42 ];
	then echo -ne "${RED} mem error ${RESET}"
	fi 
	printf "\n"
fi
memstatus=0
}
#TODO: add exit code check
test()
{
echo -ne "${YEL} $1 ${BLU}| ${BMAG} "${@:2}" ${BLU}|${RESET}"
bash $test_bh "${@:2}" &
bash -c "bash -i &>>$bash_temp/$bash_output"
bash $test_ms "${@:2}" &
bash -c "$minishell &>>$ms_temp/$ms_output"
if [[ $mem == true ]]; then
echo 
echo -ne "${BCYN}memcheck: ignore potential output ${RESET}"
bash $test_mem "${@:2}" &
bash -c "$valgrind_cmd $minishell &>> $LOG_DIR/$INTER_MEM_LOG"
memstatus=$(echo $?)
sleep 1
fi
}

check_multiple_files ()
{
ARG=$1
for var in "${@:2}"
do
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
rstatus=$?
ARG=$1
if [ $rstatus != 0 ];
	then 
	printf "${BMAG}${ARG}:file:$bash_filter:$ms_filter${LINEP}${RED}FAIL \n${RESET}";
	return
fi
for var in "${@:2}"
do
mstatus=$?
if [ $mstatus != 0 ];
	then 
	printf "${BMAG}${ARG}:file:$var${LINEP}${RED}FAIL \n${RESET}";
	return
fi
done
printf "${BMAG}${ARG}${LINEP}${GRN}OK \n${RESET}";
}

echo -e "${BCYN}interactive${RESET}"
echo -e "${RED}Watch out! safer to run tests in seperate terminal window instead of VScode${RESET}"
echo -e "${RED}tester send keystrokes to window that tester is activated in${RESET}"
echo -e "${RED}if you misclick in vscode it will send keystrokes to that section${RESET}"


remove_temp_files
test 0 'cat <<EOF' 'blabla' 'EOF' "ctrl+c" "ctrl+d" 
check_result

remove_temp_files
test 1 "echo lol" "ctrl+c" "ctrl+d"
check_result

remove_temp_files
test 2 'export var0="head -"' 'export var1="n 1"' 'export var2="0"' 'echo $var1$var2' 'echo $var0$var2' "ctrl+c" "ctrl+d"
check_result

remove_temp_files
test 3 'export var="    ls    "' '$var' "ctrl+c" "ctrl+d"
check_result

remove_temp_files
test 4 "export var=hi" 'echo $var$PWD' "ctrl+c" "ctrl+d"
check_result 

remove_temp_files
test 5 'export ls="ls -l"' '$ls' '"$ls"' "ctrl+c" "ctrl+d"
check_result

remove_temp_files
test 6 'export v3="hello hello"' 'export v2="$v3  $v3"' 'env | grep v2' "ctrl+c" "ctrl+d"
check_result

#This will always fail, because we won't implement sudo apt install message
remove_temp_files
test 7 '$(cmddoesnotexist)' "ctrl+c" "ctrl+d"
check_result

remove_temp_files
test 8 "ctrl+c" "ctrl+d"
check_result

mkdir -p outfiles
remove_temp_files
test 9 "export CDPATH=.:./outfiles/lol" "mkdir -p ./outfiles/lol" "cd ./outfiles/lol" "pwd" "ctrl+c" "ctrl+d"
check_result
rm -rf outfiles

mkdir -p outfiles
remove_temp_files
test 10 "mkdir -p ./outfiles/lol" "cd ./outfiles" "cd lol" "pwd" "ctrl+c" "ctrl+d"
check_result
rm -rf outfiles

remove_temp_files
test 11 './minishell' "ctrl+c" 'ctrl+\' "ctrl+c" "ctrl+d" 
check_result

remove_temp_files
test 12 'cat' "ctrl+c" "ctrl+c" "ctrl+d" 
check_result

remove_temp_files
test 13 'cat' 'ctrl+\' "ctrl+c" "ctrl+d" 
check_result

remove_temp_files
test 14 'sleep infinity | ls' 'ctrl+\' "ctrl+c" "ctrl+d" 
check_result

remove_temp_files
test 15 'export lol' 'env' "ctrl+c" "ctrl+d" 
check_result



# check_result_multiple_files 1 "temp1" "temp2"
# test "ctrl+c" "ctrl+c" "ctrl+c" "ctrl+d"

rm -rf $bash_temp
rm -rf $ms_temp
rm -rf $LOG_DIR/$INTER_MEM_LOG

if [ $FAIL = true ];
then exit 1
else 
exit 0
fi

