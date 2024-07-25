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
# Reference: https://github.com/LucasKuhn/minishell_tester
# Reference: https://valgrind.org/docs/manual/manual-core.html#:~:text=Valgrind%20loads%20suppression%20patterns%20from,supp%20one%20or%20more%20times.
# Reference: https://valgrind.org/docs/manual/manual-core.html#manual-core.options

#usage function
usage() {
	cat <<EOF
Usage: $0 [options] [--] [file...]

Arguments:

  -h, --help
	Display this usage message and exit.

  -c, --check
	This check the dependencies that the script needs.
	Shows which dependencies are installed and which not,
	and then exits

  -C, --clean
	This will clean all directories that are not needed, usually
	temporary directories used by the tester

  -k <"arr">, --cases <"arr">
	This flag passes together with an array of strings will only test
	the cases that are passed. It is used as follows
	bash e2e.sh -k "builtins pipes quotes"

  -i, --interactive
	This flag will enable the interactive tests run in this tester.
	This flag does not work with all no flags selected

  -nm, --no-memory
	This flag will disable the memory tests run in this tester.
	This flag does not work with all no flags selected

  -ni, --no-interactive
	This flag will disable the interactive tests run in this tester.
	This flag does not work with all no flags selected

  -nn, --no-norminette
	This flag will disable the norminette tests run in this tester.
	This flag does not work with all no flags selected

  -ne, --no-e2e
	This flag will disable the end-to-end tests run in this tester.
	This flag does not work with all no flags selected

  -ou, --only-norminette
	This flag will only run the norminette tests run in this tester.
	This flag does not work with multiple only flags

  -oi, --only-interactive
	This flag will only run the interactive tests run in this tester.
	This flag does not work with multiple only flags

  -oe, --only-e2e
	This flag will only run the end-to-end tests run in this tester.
	This flag does not work with multiple only flags

EOF
}

# handy logging and error handling functions
log() { printf '%s\n' "$*"; }
error() { log "ERROR: $*" >&2; }
fatal() { error "$*"; exit 1; }
usage_fatal() { error "$*"; usage >&2; exit 1; }

check=0
virtual=0
norminette=1
memory=1
e2e=1
interactive=0
set_only=0
set_only_multiple=0
clean=0


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
		-C|--clean)
		clean=1
		shift
		;;
		-k|--cases)
		test_cases=$2
		check_cases=1
		shift 2
		;;
		-i|--interactive)
		interactive=1
		shift
		;;
		-nm|--no-memory)
		memory=0
		shift
		;;
		-nn|--no-norminette)
		norminette=0
		shift
		;;
		-ni|--no-interactive)
		interactive=0
		shift
		;;
		-ne|--no-e2e)
		e2e=0
		shift
		;;
		-on|--only-norminette)
		e2e=0
		interactive=0
		norminette=1
		if [ $set_only == 1 ]
		then
		set_only_multiple=1
		fi
		set_only=1
		shift
		;;
		-oi|--only-interactive)
		norminette=0
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
		norminette=0
		interactive=0
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
fi
if [[ $norminette == 0 && $e2e == 0 && $norminette == 0 && $interactive == 0 ]];
then
usage_fatal "not all '-n*, --no-*' flags can be selected at the same time"
exit 1
fi
if [[ $check == 1 ]];
then
command -v docker &> /dev/null
vbox=$?
if [[ $vbox == 0 ]];
then
echo docker Installed
else
echo docker Not Installed, needed to run with --virtual option, otherwise not neccessary
cstatus=1
fi
if [[ $cstatus == 1 ]];
then
exit 1
else
exit 0
fi
fi

##initialize variables

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

#fail flags
FAIL=false
ERROR_FAIL=false
OUTPUT_FAIL=false
EXIT_FAIL=false
OUTFILES_FAIL=false
MEMORY_FAIL=false
NORM_FAIL=false

#logs
LOG_DIR=logs
MS_LOG=$LOG_DIR/ms.log
MEMORY_LOG=$LOG_DIR/memory.log
TEMP_MEMORY_LOG=$LOG_DIR/temp_memory.log
ERROR_LOG=$LOG_DIR/error_message.log
OUTPUT_LOG=$LOG_DIR/output_diff.log
EXIT_LOG=$LOG_DIR/exit_code.log

#files
OUTFILES_LOG=$LOG_DIR/outfiles_diff.log
outfiles=./files/outfiles
bash_outfiles=./files/bash_outfiles
mini_outfiles=./files/mini_outfiles
noaccess=./files/noaccess/noaccess
suppressions=./util/valgrind_suppresion

#valgrind
valgrind_cmd="valgrind --error-exitcode=42 --leak-check=full --show-leak-kinds=all --suppressions=$suppressions"

#prepare files
chmod 000 $noaccess
minishell=$(find ../../../ -type f -name minishell)
minishelldir=$(find ../../../ -type d -name minishell)
cases="./cases"
files_temp=./files_temp
files=./files


clean()
{
rm -rf $LOG_DIR
rm -rf $outfiles
rm -rf $bash_outfiles
rm -rf $mini_outfiles
rm -rf $static_outfile_temp
exit
}




if [[ $clean == 1 ]];
then
clean
exit
fi

rm -rf $LOG_DIR
mkdir -p $LOG_DIR
rm -rf $outfiles
mkdir -p $outfiles
rm -rf $bash_outfiles
mkdir -p $bash_outfiles
rm -rf $mini_outfiles
mkdir -p $mini_outfiles
rm -rf $files_temp
mkdir -p $files_temp

#prepare minishell
make -C $minishelldir test

#add minishell to path
export PATH=$PATH:$(cd $minishelldir && pwd)

#truncate logs
if [ -f $MS_LOG ]
then
truncate -s 0 $MS_LOG
fi
#FUNCTIONS
if [ $norminette == 1 ];
then
bash ./norminette_tester/norminette_tester.sh -d ../../src
norm_1=$(echo $?)
bash ./norminette_tester/norminette_tester.sh -d ../../include -npi
norm_2=$(echo $?)
mv norminette.log logs/
if [[ $norm_1 == 1 || $norm_2 == 1 ]];
then
FAIL=true
NORM_FAIL=true
fi
fi
if [ $interactive == 1 ];
then
if [ $memory = 1 ]; then
bash interactive.sh 1 1
else
bash interactive.sh 1 0
fi
fi
if [ $e2e == 0 ];
then
exit 0
fi






# https://github.com/LucasKuhn/minishell_tester

x=0
MINI_MEM_CODE=0
for case in "$cases"/*; do

if [[ $check_cases == 1 ]]; then 
	case_check=${case##*/}
	if [[ $(echo ${test_cases[@]} | fgrep -w $case_check) ]]
	then
	:
	else
		continue ;
	fi
fi

echo -e "${BCYN}$case${RESET}"
while IFS= read -r line; do
	if [[ "${line:0:1}" == "#" ]];
	then
	continue
	fi

	x=$(( $x + 1 ))
	echo -ne "${YEL} $x ${BLU}| ${BMAG}$line ${BLU}|${RESET}"
	cp -r $files/* $files_temp/ &>> $MS_LOG
	rm -rf $outfiles/*
	rm -rf $mini_outfiles/*
	MINI_OUTPUT=$(echo -e "$line" | $minishell 2>>$MS_LOG)
	MINI_EXIT_CODE=$(echo $?)
	MINI_OUTPUT=${MINI_OUTPUT#*"$line"}
	MINI_OUTPUT=${MINI_OUTPUT%'minishell:~$ '}
	MINI_OUTPUT=$(echo $MINI_OUTPUT | xargs -0)
	MINI_OUTFILES=$(cp -r $outfiles/* $mini_outfiles &>> $MS_LOG)
	MINI_ERROR_MSG=$(trap "" PIPE && echo "$line" | $minishell 2>&1 >> $MS_LOG | grep -oa '[^:]*$' | tr -d '\0' | head -n1)
	if [ $memory = 1 ]; then
	MINI_MEM_LOG=$(echo -e "$line" | $valgrind_cmd $minishell &> $TEMP_MEMORY_LOG)
	MINI_MEM_CODE=$(echo $?)
	fi

	cp -r $files_temp/* $files &>> $MS_LOG
	cp -r $files/* $files_temp &>> $MS_LOG

	rm -rf $outfiles/*
	rm -rf $bash_outfiles/*
	BASH_OUTPUT=$(echo -e "$line" | bash 2>>$MS_LOG)
	BASH_OUTPUT=$(echo $BASH_OUTPUT | xargs -0)
	echo $BASH_OUTPUT &>> $MS_LOG
	BASH_EXIT_CODE=$(echo -e "$line" | bash 2>> $MS_LOG ; echo $?)
	BASH_EXIT_CODE=$(echo "${BASH_EXIT_CODE##* }" | tail -1)
	BASH_OUTFILES=$(cp -r $outfiles/* $bash_outfiles &>>$MS_LOG)
	BASH_ERROR_MSG=$(trap "" PIPE && echo "$line" | bash 2>&1 >> $MS_LOG | grep -o '[^:]*$' | head -n1)


	if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" && -z "$OUTFILES_DIFF" ]]; then
		printf " ✅"
		((ok++))
		if [[ "$MINI_ERROR_MSG" != "$BASH_ERROR_MSG" ]]; then
			printf " ⚠️ "
			FAIL=true
		fi
	else
		printf " ❌"
		FAIL=true
	fi

	if [ "$OUTFILES_DIFF" ]; then
		OUTFILES_FAIL=true
		printf "${RED} outfiles error;${RESET}"
		echo -e "$x | $line |${RESET}" >> $OUTFILES_LOG
		echo "$OUTFILES_DIFF" >> $OUTFILES_LOG
		echo mini outfiles: >> $OUTFILES_LOG
		cat $mini_outfiles/* >> $OUTFILES_LOG
		echo bash outfiles: >> $OUTFILES_LOG
		cat $bash_outfiles/* >> $OUTFILES_LOG
	fi
	if [ "$MINI_OUTPUT" != "$BASH_OUTPUT" ]; then
		OUTPUT_FAIL=true
		printf "${RED} output error;${RESET}"
		echo -e "$x | $line " >> $OUTPUT_LOG
		echo mini output = \($MINI_OUTPUT\) >> $OUTPUT_LOG
		echo bash output = \($BASH_OUTPUT\) >> $OUTPUT_LOG
	fi
	if [ "$MINI_EXIT_CODE" != "$BASH_EXIT_CODE" ]; then
		EXIT_FAIL=true
		printf "${RED} exit code error;${RESET}"
		echo -e "$x | $line " >> $EXIT_LOG
		echo mini exit code = $MINI_EXIT_CODE >> $EXIT_LOG
		echo bash exit code = $BASH_EXIT_CODE >> $EXIT_LOG
	fi
	if [ "$MINI_ERROR_MSG" != "$BASH_ERROR_MSG" ]; then
		ERROR_FAIL=true
		printf "${RED} error message error;${RESET}"
		echo -e "$x | $line " >> $ERROR_LOG
		echo mini error = \($MINI_ERROR_MSG\) >> $ERROR_LOG
		echo bash error = \($BASH_ERROR_MSG\) >> $ERROR_LOG
	fi
	if [ "$MINI_MEM_CODE" == 42 ]; then
		echo -e "$x | $line " >> $MEMORY_LOG
		echo LOG >> $MEMORY_LOG
		MEMORY_FAIL=true
		printf "${RED} memory error;${RESET}"
		cat $TEMP_MEMORY_LOG >> $MEMORY_LOG
		echo mini memory exit code = $MINI_MEM_CODE >> $MEMORY_LOG
	fi
	printf "\n"
	cp -r $files_temp/* $files &>> $MS_LOG
done < $case
MINI_MEM_CODE=0
done

chmod 444 $noaccess
rm -rf $files_temp
rm -rf $TEMP_MEMORY_LOG
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
if [ $MEMORY_FAIL = true ];
then
echo -e "${RED}Check $MEMORY_LOG ${RESET}"
echo -e "${RED}run memory case with $valgrind_cmd or check $MS_LOG ${RESET}"
fi
if [ $NORM_FAIL = true ];
then
echo -e "${RED}Check norminette.log ${RESET}"
fi
rm -rf $bash_outfiles
rm -rf $mini_outfiles
exit 1
else
echo -e "${GRE}Congratulations, all tests are succesfull :)${RESET}"
clean
fi
exit 0
