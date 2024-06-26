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
interactive=0
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
		-i|--interactive)
		interactive=1
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
		-oI|--only-interactive)
		unit=0
		component=0
		integration=0
		e2e=0
		interactive=1
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
if [[ $unit == 0 && $component == 0 && $integration == 0 && $e2e == 0 && $interactive == 0 ]];
then 
usage_fatal "not all '-n*, --no-*' flags can be selected at the same time"
exit 1
fi

#check dependencies
if [[ $check == 1 ]];
then 
command -v VBoxManage &> /dev/null
vbox=$?
if [[ $vbox == 0 ]];
then 
echo VBoxManage Installed
else
echo VBoxManage Not Installed, needed to run with --virtual option, otherwise not neccessary
cstatus=1
fi
if [[ $cstatus == 1 ]];
then 
exit 1
else
exit 0
fi
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
# Reference: https://git.sr.ht/~geb/dotool/tree/master/doc/dotool.1.scd
# Reference: https://github.com/LucasKuhn/minishell_tester

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
ERROR_FAIL=false
OUTPUT_FAIL=false
EXIT_FAIL=false
OUTFILES_FAIL=false
LOG_DIR=logs
MS_LOG=$LOG_DIR/ms.log
ERROR_LOG=$LOG_DIR/error_message.log
OUTPUT_LOG=$LOG_DIR/output_diff.log
EXIT_LOG=$LOG_DIR/exit_code.log
OUTFILES_LOG=$LOG_DIR/outfiles_diff.log
outfiles=./outfiles
bash_outfiles=./bash_outfiles
mini_outfiles=./mini_outfiles
minishell=$(find ../../../ -type f -name minishell)
minishelldir=$(find ../../../ -type d -name minishell)
cases="./cases"
rm -rf $LOG_DIR
mkdir -p $LOG_DIR
rm -rf $outfiles
mkdir -p $outfiles
rm -rf $bash_outfiles
mkdir -p $bash_outfiles
rm -rf $mini_outfiles
mkdir -p $mini_outfiles

#prepare minishell
make -C $minishelldir re

#add minishell to path
export PATH=$PATH:$(cd $minishelldir && pwd)

#truncate logs
if [ -f $MS_LOG ]
then
truncate -s 0 $MS_LOG
fi
#FUNCTIONS
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
if [ $interactive == 1 ]; 
then
echo run interactive
fi
if [ $e2e == 0 ]; 
then
exit 0
fi

# https://github.com/LucasKuhn/minishell_tester

x=0

for case in "$cases"/*; do
echo -e "${BCYN}$case${RESET}"
while IFS= read -r line; do
	x=$(( $x + 1 ))
	echo -ne "${YEL} $x ${BLU}| ${BMAG}$line ${BLU}|${RESET}"
	rm -rf $outfiles/*
	rm -rf $mini_outfiles/*
	MINI_OUTPUT=$(echo -e "$line" | ./minishell 2>> $MS_LOG)
	MINI_EXIT_CODE=$(echo $?)
	MINI_OUTFILES=$(cp $outfiles/* $mini_outfiles &>> $MS_LOG)
	MINI_ERROR_MSG=$(trap "" PIPE && echo "$line" | ./minishell 2>&1 >> $MS_LOG | grep -o '[^:]*$' )

	rm -rf $outfiles/*
	rm -rf $bash_outfiles/*
	BASH_OUTPUT=$(echo -e "$line" | bash 2> $MS_LOG)
	BASH_EXIT_CODE=$(echo $?)
	BASH_OUTFILES=$(cp $outfiles/* $bash_outfiles &>>$MS_LOG)
	BASH_ERROR_MSG=$(trap "" PIPE && echo "$line" | bash 2>&1 >> $MS_LOG | grep -o '[^:]*$' | head -n1)


	if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" && -z "$OUTFILES_DIFF" ]]; then
		printf " ✅"
		((ok++))
		if [ "$MINI_ERROR_MSG" != "$BASH_ERROR_MSG" ]; then
			printf " ⚠️ "
			FAIL=true
		fi
	else
		printf " ❌"
		FAIL=true
	fi

	if [ "$OUTFILES_DIFF" ]; then
		OUTFILES_FAIL=true
		printf "${RED} outfiles error${RESET}" 
		echo -e "$x | $line |${RESET}" >> $OUTFILES_LOG
		echo "$OUTFILES_DIFF" >> $OUTFILES_LOG
		echo mini outfiles: >> $OUTFILES_LOG
		cat $mini_outfiles/* >> $OUTFILES_LOG
		echo bash outfiles: >> $OUTFILES_LOG
		cat $bash_outfiles/* >> $OUTFILES_LOG
	fi
	if [ "$MINI_OUTPUT" != "$BASH_OUTPUT" ]; then
		OUTPUT_FAIL=true
		printf "${RED} output error${RESET}"
		echo -e "$x | $line " >> $OUTPUT_LOG
		echo mini output = \($MINI_OUTPUT\) >> $OUTPUT_LOG
		echo bash output = \($BASH_OUTPUT\) >> $OUTPUT_LOG
	fi
	if [ "$MINI_EXIT_CODE" != "$BASH_EXIT_CODE" ]; then
		EXIT_FAIL=true
		printf "${RED} exit code error${RESET}"
		echo -e "$x | $line " >> $EXIT_LOG
		echo mini exit code = $MINI_EXIT_CODE >> $EXIT_LOG
		echo bash exit code = $BASH_EXIT_CODE >> $EXIT_LOG
	fi
	if [ "$MINI_ERROR_MSG" != "$BASH_ERROR_MSG" ]; then
		ERROR_FAIL=true
		printf "${RED} error message error${RESET}"
		echo -e "$x | $line " >> $EXIT_LOG
		echo mini error = \($MINI_ERROR_MSG\) >> $EXIT_LOG
		echo bash error = \($BASH_ERROR_MSG\) >> $EXIT_LOG
	fi
	printf "\n"
done < $case
done

if [ $FAIL = true ];
then 
if [ $OUTFILES_FAIL = true ];
then 
echo -e "${RED}Check $OUTFILES_LOG${RESET}"
fi
if [ $OUTPUT_FAIL = true ];
then 
echo -e "${RED}Check $OUTPUT_LOG ${RESET}"
fi
if [ $EXIT_FAIL = true ];
then 
echo -e "${RED}Check $EXIT_LOG ${RESET}"
fi
if [ $ERROR_FAIL = true ];
then 
echo -e "${RED}Check $ERROR_LOG ${RESET}"
fi
rm -rf $bash_outfiles
rm -rf $mini_outfiles
exit 1
else 
echo -e "${GRE}Congratulations, all tests are succesfull :)${RESET}"
rm -rf $LOG_DIR
rm -rf $bash_outfiles
rm -rf $mini_outfiles
fi
exit 0
