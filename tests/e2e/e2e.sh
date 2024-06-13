#!/bin/bash

#usage function
usage() {
	cat <<EOF
Usage: $0 [options] [--] [file...]

Arguments:

  -h, --help
	Display this usage message and exit.

  -f <val>, --file <val>, --file=<val>
	After setting this parameter, an argument must follow with relative
	path to the program.
	e.g. [bash ./ms_e2e -f ./relative/path/pgrm]
	Without this parameter the program will not continue

  -c, --check
	This check the dependencies that the script needs.
	Shows which dependencies are installed and which not,
	and then exits

  -v, --virtual
	This will boot up a virtual machine to run the tests in. 
	This is useful when you don't have root access on your
	device. To run this flag the dependencies for running in 
	a virtual machine need to be installed.

  -nu, --no-unit
	This flag will disable the unit tests run in this tester.
	This flag does not work with

  -nc, --no-component
	This flag will disable the component tests run in this tester.
	This flag does not work with

  -ni, --no-integration
	This flag will disable the integration tests run in this tester.
	This flag does not work with

  -ne, --no-e2e
	This flag will disable the end-to-end tests run in this tester.
	This flag does not work with

  -ou, --only-unit
	This flag will only run the unit tests run in this tester. 
	This flag does not work with

  -oc, --only-component
	This flag will only run the component tests run in this tester.
	This flag does not work with

  -oi, --only-integration
	This flag will only run the integration tests run in this tester.
	This flag does not work with

  -oe, --only-e2e
	This flag will only run the end-to-end tests run in this tester.
	This flag does not work with

EOF
}

# handy logging and error handling functions
log() { printf '%s\n' "$*"; }
error() { log "ERROR: $*" >&2; }
fatal() { error "$*"; exit 1; }
usage_fatal() { error "$*"; usage >&2; exit 1; }

check=0
virtual=0
unit=1
component=1
integration=1
e2e=1
set_only=0
set_only_multiple=0


# parse options
while [ "$#" -gt 0 ]; do
	arg=$1
	case $1 in
		# convert "--opt=the value" to --opt "the value".
		# the quotes around the equals sign is to work around a
		# bug in emacs' syntax parsing
		--*'='*)
		shift;
		set -- "${arg%%=*}" "${arg#*=}" "$@";
		continue;;
		-c|--check)
		check=1
		shift 
		;;
		-v|--virtual)
		virtual=1
		shift 
		;;
		-nu|--no-unit)
		unit=0
		shift 
		;;
		-nc|--no-component)
		component=0
		shift
		;;
		-ni|--no-integration)
		integration=0
		shift
		;;
		-ne|--no-e2e)
		e2e=0
		shift
		;;
		-ou|--only-unit)
		component=0
		integration=0
		e2e=0
		ou=1
		if [ $set_only == 1 ]
		then 
		set_only_multiple=1
		fi
		set_only=1
		shift 
		;;
		-oc|--only-component)
		unit=0
		integration=0
		e2e=0
		oc=1
		if [ $set_only == 1 ]
		then 
		set_only_multiple=1
		fi
		set_only=1
		shift 
		;;
		-oi|--only-integration)
		unit=0
		component=0
		e2e=0
		oi=1
		if [ $set_only == 1 ]
		then 
		set_only_multiple=1
		fi
		set_only=1
		shift 
		;;
		-oe|--only-e2e)
		unit=0
		component=0
		integration=0
		oe=1
		if [ $set_only == 1 ]
		then 
		set_only_multiple=1
		fi
		set_only=1
		shift 
		;;
		-h|--help)
		usage;
		exit 0
		;;
		--)
		shift;
		break
		;;
		-*)
		usage_fatal "unknown option:'$1'"
		;;
		*)
		break
		;; # reached the list of file names
	esac
done

#check only flags
if [[ $set_only_multiple == 1 ]];
then 
usage_fatal "you can only set one '-o*, --only-*' flag"
exit 1
fi

#check no flags
if [[ $unit == 0 && $component == 0 && $integration == 0 && $e2e == 0 ]];
then 
echo lol
usage_fatal "not all '-n*, --no-*' flags can be selected at the same time"
exit 1
fi

#check dependencies
if [[ $check == 1 ]];
then 
command -v VBoxManage
vbox=$?
if [[ $vbox == 0 ]];
then 
echo VBoxManage Installed
else
echo VBoxManage Not Installed
cstatus=1
fi
ydo=$(find ../../  -type d -name ydotool -not -path "../../.git/*")
ydo_exe=$ydo/build/ydotool
if [ -x $ydo_exe ]; 
then 
echo ydotool Installed
else
echo ydotool Not Installed
cstatus=1
fi
if [ $cstatus == 1 ];
then 
exit 1
fi
exit 0
fi

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
LINEP="\033[25G"
FAIL=false
LOG_DIR=logs/
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
ctrlc=$(find . -name ctrlc.sh)
ctrld=$(find . -name ctrld.sh)
test_bh=$(find . -name test_bh.sh)
test_ms=$(find . -name test_ms.sh)
minishell=$(find ../../../ -type f -name minishell)
minishelldir=$(find ../../../ -type d -name minishell)
rm -rf logs
mkdir -p logs
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

#prepare minishell
make -C $minishelldir re

#prepare ydotool
ydotooldir=$(find ../../  -type d -name ydotool -not -path "../../.git/*")
mkdir -p $ydotooldir/build
(cd $ydotooldir/build && cmake ..)
(cd $ydotooldir/build && make -j `nproc`)
(cd $ydotooldir/build && ./ydotoold) &
ydotool=$ydotooldir/build/ydotool

#truncate logs
truncate -s 0 $LOG_DIR/$MS_LOG

#FUNCTIONS
truncate_temp()
{
truncate -s 0 $bash_temp/$bash_output
truncate -s 0 $bash_temp/$bash_inm
truncate -s 0 $bash_temp/$bash_filter
truncate -s 0 $ms_temp/$ms_output
truncate -s 0 $ms_temp/$ms_inm
truncate -s 0 $ms_temp/$ms_filter
}

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
	printf "${BMAG}${ARG}${LINEP}${RED}FAIL ${RESET}";
fi
}

test()
{
bash $test_bh "$@" &
bash -c "bash -i &>>$bash_temp/$bash_output"
bash $test_ms "$@" &
bash -c "$minishell &>>$ms_temp/$ms_output"
}

check_result_multiple_files ()
{
ARG=$1
for var in "${@:2}"
do
diff $bash_temp/$var $mini_temp/$var &>> $LOG_DIR/$MS_LOG
dstatus=$?
if [ $dstatus != 0 ];
	then 
	printf "${BMAG}${ARG}:file:$var${LINEP}${RED}FAIL ${RESET}";
	return
fi
done
	printf "${BMAG}${ARG}${LINEP}${GRN}OK \n${RESET}";
}



if [ $virtual == 1 ]; 
then
echo run vm
fi
if [ $unit == 1 ]; 
then
echo run unit
fi
if [ $component == 1 ]; 
then
echo run component
fi
if [ $integration == 1 ]; 
then
echo run integration
fi
if [ $e2e == 0 ]; 
then
exit 0
fi

echo -e "${BLU}----------------------------------
|           interactive           |
----------------------------------${RESET}"

# test "ctrl+c" "ctrl+c" "ctrl+c" "echo lol" "ctrl+d"
# check_result_multiple_files 1 "temp1" "temp2" "temp3"
# check_result 1
# check_result 2
# check_result 3
# check_result "lol test"
# #memory test
# timeout --preserve-status 5s bash -c "valgrind --error-exitcode=42 --tool=memcheck --leak-check=full --show-reachable=yes --errors-for-leak-kinds=all bash -i &>output"
# mstatus=$?
# echo $mstatus

# echo -e "input test"

# echo -e "${BLU}----------------------------------
# |             built-ins           |
# ----------------------------------${RESET}"

# echo -e "built-in tests"

# echo -e "${BLU}----------------------------------
# |          execution              |
# ----------------------------------${RESET}"

# echo -e "execution tests"

# echo -e "${BLU}----------------------------------
# |              quotes             |
# ----------------------------------${RESET}"

# echo -e "quotes tests"

# echo -e "${BLU}----------------------------------
# |           redirection           |
# ----------------------------------${RESET}"

# echo -e "redirection tests"

# echo -e "${BLU}----------------------------------
# |                pipe             |
# ----------------------------------${RESET}"

# echo -e "pipe tests"

# echo -e "${BLU}----------------------------------
# |      environment variables      |
# ----------------------------------${RESET}"

# echo -e "environment tests"

# echo -e "${BLU}----------------------------------
# |           interactive           |
# ----------------------------------${RESET}"

if [ $FAIL = true ];
then echo -e "${RED}Check logs/*.log for errors${RESET}"
else echo -e "${GRE}Congratulations, all tests are succesfull :)${RESET}"
fi
exit 0
