#!/bin/bash

#usage function
usage() {
	cat <<EOF
Usage: $0 [options] [--] [file...]

Arguments:

  -f <val>, --file <val>, --file=<val>
	After setting this parameter, an argument must follow with relative
	path to the program.
	e.g. [bash ./ms_e2e -f ./relative/path/pgrm]
	Without this parameter the program will not continue

  -h, --help
	Display this usage message and exit.

EOF
}

# handy logging and error handling functions
log() { printf '%s\n' "$*"; }
error() { log "ERROR: $*" >&2; }
fatal() { error "$*"; exit 1; }
usage_fatal() { error "$*"; usage >&2; exit 1; }

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
		-f|--file)
		file=$2
		shift 2
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

#check file input
if [ -n "$file" ];
then :
else
usage_fatal "option '-f, --file' requires a value"
exit 1
fi

#sources
#https://github.com/aeruder/expect/blob/master/INSTALL
#https://gabrielstaples.com/ydotool-tutorial/#gsc.tab=0
#https://github.com/ReimuNotMoe/ydotool
#https://github.com/ReimuNotMoe/ydotool/issues/10
#https://github.com/ReimuNotMoe/ydotool/issues/170
#file:///usr/include/linux/input-event-codes.h
#https://gabrielstaples.com/ydotool-tutorial/#gsc.tab=0
#https://stackoverflow.com/questions/13242469/how-to-use-sed-grep-to-extract-text-between-two-words
#https://stackoverflow.com/questions/34412754/trying-to-remove-non-printable-characters-junk-values-from-a-unix-file
#https://stackoverflow.com/questions/67900936/use-cut-in-shell-to-extract-last-word
#https://stackoverflow.com/questions/6070995/how-to-make-valgrind-report-an-error-when-there-are-still-reachable-allocs
#https://stackoverflow.com/questions/255898/how-to-iterate-over-arguments-in-a-bash-script

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
LINEP="\033[75G"
FAIL=false
LOG_DIR=logs/
MS_LOG=ms.log
ESC=ESC
CLOSE=CLOSE
temp=temp
bash_output=bh_output.tmp
bash_inm=bh_inmp.tmp
bash_filter=bh_filter.tmp
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
rm -rf $temp
mkdir -p $temp

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

#TODOS
##check how to share variables
##check if dependencies are installed e.g. when flag is raised for vm
##make flag to run without interactive test
##make flag to run without unit/integration/component
##make flag to run without vm

#FUNCTIONS
truncate_temp()
{
truncate -s 0 $temp/$bash_output
truncate -s 0 $temp/$bash_inm
truncate -s 0 $temp/$bash_filter
truncate -s 0 $temp/$ms_output
truncate -s 0 $temp/$ms_inm
truncate -s 0 $temp/$ms_filter
}

check_result()
{
diff $temp/$bash_filter $temp/$ms_filter &>> $LOG_DIR/$MS_LOG
dstatus=$?

if [ $dstatus == 0 ];
	then 
	printf "${BMAG} ${LINEP}${GRN}OK \n${RESET}";
	else
	printf "${BMAG} ${LINEP}${RED}FAIL ${RESET}";
fi
}

test()
{
bash $test_bh "$@" &
bash -c "bash -i &>>$temp/$bash_output"
bash $test_ms "$@" &
bash -c "$minishell &>>$temp/$ms_output"
}


echo -e "${BLU}----------------------------------
|           interactive           |
----------------------------------${RESET}"

test "ctrl+c" "ctrl+c" "ctrl+c" "echo lol" "ctrl+d"

# #memory test
# timeout --preserve-status 5s bash -c "valgrind --error-exitcode=42 --tool=memcheck --leak-check=full --show-reachable=yes --errors-for-leak-kinds=all bash -i &>output"
# mstatus=$?
# echo $mstatus

echo -e "input test"

echo -e "${BLU}----------------------------------
|             built-ins           |
----------------------------------${RESET}"

echo -e "built-in tests"

echo -e "${BLU}----------------------------------
|          execution              |
----------------------------------${RESET}"

echo -e "execution tests"

echo -e "${BLU}----------------------------------
|              quotes             |
----------------------------------${RESET}"

echo -e "quotes tests"

echo -e "${BLU}----------------------------------
|           redirection           |
----------------------------------${RESET}"

echo -e "redirection tests"

echo -e "${BLU}----------------------------------
|                pipe             |
----------------------------------${RESET}"

echo -e "pipe tests"

echo -e "${BLU}----------------------------------
|      environment variables      |
----------------------------------${RESET}"

echo -e "environment tests"

echo -e "${BLU}----------------------------------
|           interactive           |
----------------------------------${RESET}"

if [ $FAIL = true ];
then echo -e "${RED}Check logs/*.log for errors${RESET}"
else echo -e "${GRE}Congratulations, all tests are succesfull :)${RESET}"
fi
exit 0