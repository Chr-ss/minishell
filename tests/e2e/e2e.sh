#!/bin/bash

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

#prepare ydotool
ydotooldir=$(find ../../  -type d -name ydotool -not -path "../../.git/*")
mkdir -p $ydotooldir/build
(cd $ydotooldir/build && cmake ..)
(cd $ydotooldir/build && make -j `nproc`)
(cd $ydotooldir/build && ./ydotoold) &
ydotool=$ydotooldir/build/ydotool

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

# arguments are now the file names


if [ -n "$file" ];
then :
else
usage_fatal "option '-f, --file' requires a value"
exit 1
fi

rm -rf logs
mkdir -p logs

truncate -s 0 $LOG_DIR/$MS_LOG

# center() {
#   termwidth="$(tput cols)"
#   padding="$(printf '%0.1s' ' '{1..500})"
#   printf ${BLU}'%*.*s %s %*.*s \n'${RESET} 0 "$(((termwidth-2-${#1})/2))" "$padding" "$1" 0 "$(((termwidth-1-${#1})/2))" "$padding"
# }

# L() {
# printf -vl "%${2:-${COLUMNS:-`tput cols 2>&-||echo 80`}}s\n" && echo -e "${BLU}${l// /${1:-=}}${RESET}";
# }


##### input tests

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
#https://github.com/aeruder/expect/blob/master/INSTALL
#https://gabrielstaples.com/ydotool-tutorial/#gsc.tab=0
#https://github.com/ReimuNotMoe/ydotool
#https://github.com/ReimuNotMoe/ydotool/issues/10
#https://stackoverflow.com/questions/13242469/how-to-use-sed-grep-to-extract-text-between-two-words


temp=temp
bash_output=bh_output.tmp
bash_inm=bh_inmp.tmp
bash_filter=bh_filter.tmp
ms_output=ms_output.tmp
ms_inm=ms_inmp.tmp
ms_filter=ms_filter.tmp
ctrlc=$(find . -name ctrlc.sh)
minishell=$(find ../../../ -type f -name minishell)
rm -rf $temp
mkdir -p $temp

bash $ctrlc 4 &
bash -c "bash -i &>$temp/$bash_output"
mstatus=$?
awk '{print $NF}' $temp/$bash_output > $temp/$bash_inm
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~//g' $temp/$bash_inm 
tr -cd '[:print:]' < $temp/$bash_inm > $temp/$bash_filter

bash $ctrlc 4 &
bash -c "$minishell &>$temp/$ms_output"
mstatus=$?
awk '{print $NF}' $temp/$ms_output > $temp/$ms_inm
sed -i 's/\x1b\[?2004h//g; s/\x1b\[?2004l//g; s/minishell:~//g' $temp/$ms_inm 
tr -cd '[:print:]' < $temp/$ms_inm > $temp/$ms_filter

diff $temp/$bash_filter $temp/$ms_filter &>> $LOG_DIR/$MS_LOG
dstatus=$?

if [ $dstatus == 0 ];
	then 
	printf "${BMAG} ${LINEP}${GRN}OK \n${RESET}";
	else
	printf "${BMAG} ${LINEP}${RED}FAIL ${RESET}";
fi


# # ctrl + d
# sh -c 'sleep 0.71 && ./ydotool/build/ydotool key 29:0 32:0' &
# sh -c 'sleep 0.2 && ./ydotool/build/ydotool key 29:1 32:1' &
# bash -c "bash -i &>output"
# mstatus=$?


# #memory test
# timeout --preserve-status 5s bash -c "valgrind --error-exitcode=42 --tool=memcheck --leak-check=full --show-reachable=yes --errors-for-leak-kinds=all bash -i &>output"
# mstatus=$?
# echo $mstatus



if [ $FAIL = true ];
then echo -e "${RED}Check logs/*.log for errors${RESET}"
else echo -e "${GRE}Congratulations, all tests are succesfull :)${RESET}"
fi
exit 0
