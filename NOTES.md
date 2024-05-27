At some point we have to take care of expanding still (most likely at the very beginning of the code...)
	$PATH

check string for syntax errors
	crasche@f0r2s6:~$ | ls | ls
	bash: syntax error near unexpected token `|'
we don't meed to handle open quotes
	crasche@f0r2s6:~$ echo "hello
	> ^C
tycho said not make this spacial case
	crasche@f0r2s6:~$ echo "hello;\hello"
	hello;\hello
	crasche@f0r2s6:~$ echo "hello";\"hello"
	> ^C
	crasche@f0r2s6:~$ echo "hello"a b"hello"
	helloa bhello
	crasche@f0r2s6:~$ echo "hello"ab"hello"
	helloabhello
handle this too
	crasche@f0r2s6:~$ ls | ls <
	bash: syntax error near unexpected token `newline'

for bonus we'd need this
	&& ||
this we don't need (would make strings hard according to tycho)
	; \



analyze token
	token 1 and token 2 (token )




## ENUMS WITH FUNCTION POINTER DESICION
			#include <stdio.h>

			// Define the enum
			typedef enum {
				NEG_INF,
				ZERO,
				POS_INF,
				NOT_SPECIAL
			} CheckType;

			// Define the functions for each case
			int handleNegInf() {
				printf("neg inf\n");
				return 1;
			}

			int handleZero() {
				printf("zero\n");
				return 2;
			}

			int handlePosInf() {
				printf("pos inf\n");
				return 3;
			}

			int handleNotSpecial() {
				printf("not special\n");
				return 0;
			}

			// Array of function pointers
			int (*handlers[])(void) = {
				handleNegInf,
				handleZero,
				handlePosInf,
				handleNotSpecial // Default case handler
			};

			// Function to handle the check
			int handleCheck(CheckType check) {
				if (check >= 0 && check < sizeof(handlers) / sizeof(handlers[0])) {
					return handlers[check]();
				} else {
					return handleNotSpecial();
				}
			}

			int main() {
				// Example usage
				CheckType check = ZERO;
				int result = handleCheck(check);
				printf("Return value: %d\n", result);

				check = POS_INF;
				result = handleCheck(check);
				printf("Return value: %d\n", result);

				check = NOT_SPECIAL;
				result = handleCheck(check);
				printf("Return value: %d\n", result);

				return 0;
			}
