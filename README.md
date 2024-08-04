# MiniShell Project

## Overview
The MiniShell project implements a basic shell with several built-in commands. It mimics the behavior of traditional Unix shells, allowing users to interact with the operating system via a command-line interface.

## File Descriptions
### buildins.c
This file contains implementations for built-in shell commands such as `pwd` and `unset`. These commands are essential for navigating and managing the shell's environment.

### combined_main.c
This file contains the main function and input handling logic for the shell. It initializes the shell, handles user input, and manages the execution flow.

### utils.c
This file contains utility functions that support various operations within the shell, including environment variable management and command handling.

### token.c
This file contains functions for handling tokens, which are essential for parsing and executing commands in the shell.

### combined_signal.c
This file manages signal handling for different modes of the shell, including interactive mode, execution mode, and handling of here-documents.

### expansion_combined.c
This file contains functions for handling variable expansion within the shell, including expanding environment variables and special variables like the exit code.

### combined_children.c
This file contains functions for managing child processes within the shell, including initialization, adding, deleting, resetting, and terminating child processes.

### final_combined_execution.c
This file contains functions related to executing commands within the shell, including handling pipes, child processes, and restoring file descriptors.

## Function Documentation
### pwd
**Description:** Prints the current working directory.  
**Parameters:** None  
**Return Value:** `int` - Returns `1` if it fails to get the current directory, otherwise `0`.

### unset_new_envp
**Description:** Creates a new environment variable array excluding a specified index.  
**Parameters:**  
- `t_msdata *data`: Shell data structure containing the environment variables.  
- `int skip_index`: Index to be skipped.

**Return Value:** `char**` - Pointer to the new environment variable array.

### unset
**Description:** Unsets environment variables.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `char **argv`: Array of arguments.  
- `char *arg`: Single argument.

**Return Value:** `int` - Returns `0` on success, `-1` on failure.

### cd_chdir_free
**Description:** Frees allocated memory for `pwd` and `old_pwd`.  
**Parameters:**  
- `int ret`: Return value to be returned after freeing memory.  
- `char *pwd`: Pointer to the current working directory string.  
- `char *old_pwd`: Pointer to the old working directory string.

**Return Value:** `int` - Returns the input parameter `ret`.

### cd_chdir_pwd
**Description:** Gets the current working directory.  
**Parameters:** None  
**Return Value:** `char*` - Pointer to a string containing the current working directory.

### main
**Description:** Entry point for the shell program. Initializes the shell and handles command-line arguments.  
**Parameters:**  
- `int argc`: Argument count.  
- `char **argv`: Argument vector.  
- `char **envp`: Environment variables.

**Return Value:** `int` - Returns the shell's exit code.

### input_handling
**Description:** Handles user input, including reading commands, expanding variables, parsing, and executing commands.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### initdata
**Description:** Initializes the shell's data structure with environment variables.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `char **envp`: Environment variables.

**Return Value:** None.

### initdata_cpy_envp
**Description:** Copies environment variables into the shell's data structure.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `char **envp`: Environment variables.

**Return Value:** None.

### initdata_shlvl
**Description:** Initializes the shell level (`SHLVL`) environment variable.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `int index`: Index of the `SHLVL` variable in the environment array.

**Return Value:** None.

### add_envp_new_entry
**Description:** Creates a new environment variable entry.  
**Parameters:**  
- `char **new_envp`: New environment variable array.  
- `char *key`: Key for the environment variable.  
- `char *value`: Value for the environment variable.

**Return Value:** `char*` - Pointer to the new environment variable entry.

### add_envp
**Description:** Adds a new environment variable to the shell's environment.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `char *key`: Key for the environment variable.  
- `char *value`: Value for the environment variable.

**Return Value:** None.

### cmd_close_fd
**Description:** Closes a file descriptor.  
**Parameters:**  
- `int *fd`: File descriptor to be closed.

**Return Value:** None.

### free_cmd
**Description:** Frees the memory allocated for a command.  
**Parameters:**  
- `t_cmd *cmd`: Command structure.

**Return Value:** None.

### cmd_clear
**Description:** Clears the command list.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### cmd_reset
**Description:** Resets the command list and other relevant data.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### token_to_strarr
**Description:** Converts a token to a string array.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `char **strarr`: String array.  
- `t_token token`: Token to be converted.

**Return Value:** None.

### init_type_handler
**Description:** Initializes the array of type handler functions for different token types.  
**Parameters:**  
- `t_token (*type_handler[8])(t_msdata *data, t_cmd *cmd, t_token token, int *pos)`: Array of type handler functions.

**Return Value:** None.

### token_to_cmd
**Description:** Converts a token to a command.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_token token`: Token to be converted.  
- `int *pos`: Position in the token array.

**Return Value:** `t_token` - Returns the processed token.

### openfile
**Description:** Opens a file based on the token.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_token token`: Token containing the file name.  
- `int open_flag`: Flags for opening the file.  
- `int *fd`: File descriptor.

**Return Value:** None.

### printf_cmd
**Description:** Prints the command and its arguments for debugging.  
**Parameters:**  
- `t_cmd *cmd`: Command structure.

**Return Value:** None.

### init_signal_interactive
**Description:** Initializes signal handling for interactive mode.  
**Parameters:**  
- `struct sigaction *sa`: Signal action structure.  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### init_signal_execution
**Description:** Initializes signal handling for execution mode.  
**Parameters:**  
- `struct sigaction *sa`: Signal action structure.  
- `t_msdata *data`: Shell data structure.  
- `bool *exec`: Execution flag.

**Return Value:** None.

### init_signal_heredoc
**Description:** Initializes signal handling for here-document mode.  
**Parameters:**  
- `struct sigaction *sa`: Signal action structure.  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### init_signal_after_heredoc
**Description:** Initializes signal handling for after here-document mode.  
**Parameters:**  
- `struct sigaction *sa`: Signal action structure.  
- `t_msdata *data`: Shell data structure.  
- `bool *exec`: Execution flag.

**Return Value:** None.

### init_signal
**Description:** Initializes signal handling based on the specified type.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `int type`: Type of signal handling to initialize.

**Return Value:** None.

### expand_exit_code
**Description:** Expands the special variable `$?` to the shell's exit code.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_expand *exp`: Expansion data structure.  
- `int *pos`: Current position in the input line.

**Return Value:** None.

### expand_getenv
**Description:** Retrieves the value of an environment variable from the environment array.  
**Parameters:**  
- `char **envp`: Environment variable array.  
- `char *env_start`: Start of the environment variable name.  
- `int env_len`: Length of the environment variable name.

**Return Value:** `char*` - Returns a pointer to the value of the environment variable, or `NULL` if not found.

### expand_var
**Description:** Expands an environment variable in the input line.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_expand *exp`: Expansion data structure.  
- `int *pos`: Current position in the input line.

**Return Value:** None.

### expand_copy
**Description:** Processes the input line and expands variables where applicable.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_expand *exp`: Expansion data structure.

**Return Value:** None.

### expand
**Description:** Main function for expanding variables in the input line.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** `char*` - Returns the expanded input line as a string.

### init_kindergarten
**Description:** Initializes the child process management structure.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### add_child
**Description:** Adds a new child process to the list of managed child processes.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `pid_t pid`: Process ID of the child process.

**Return Value:** None.

### delete_last_child
**Description:** Deletes the last child process from the list.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### reset_childs
**Description:** Resets the list of child processes by deleting all entries.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### kill_all_childs
**Description:** Terminates all child processes.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

### execute_parent_close_pipe
**Description:** Closes the pipe file descriptors in the parent process.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_cmd *cmd`: Command structure.

**Return Value:** None.

### execute_pipe_child
**Description:** Forks a child process to execute a command with pipes.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.  
- `t_cmd *cmd`: Command structure.  
- `int *pid`: Pointer to the process ID of the child.

**Return Value:** None.

### execute_parent_restore_fds
**Description:** Restores the original file descriptors for standard input and output in the parent process.  
**Parameters:**  
- `t_msdata *data`: Shell data structure.

**Return Value:** None.

## Data Structures
### t_msdata
**Description:** A structure containing the shell's environment variables and other relevant data.
- `char **envp`: Array of environment variables.

### t_cmd
**Description:** A structure representing a command in the shell.
- `char *cmd`: The command to be executed.
- `char **argv`: Arguments for the command.
- `char **heredoc`: Here-document contents.
- `int infd`: Input file descriptor.
- `int outfd`: Output file descriptor.
- `int pipefd[2]`: Pipe file descriptors.
- `t_cmd *pipe`: Next command in the pipeline.

### t_token
**Description:** A structure representing a token in the shell.
- `char *start`: Start of the token.
- `int length`: Length of the token.
- `int type`: Type of the token.

### t_expand
**Description:** A structure representing expansion data.
- `char *line`: Expanded line.
- `char *env`: Environment variable value.
- `int capacity`: Capacity of the expanded line buffer.

### t_childs
**Description:** A structure representing a child process in the shell.
- `pid_t pid`: Process ID of the child.
- `t_childs *next`: Pointer to the next child in the list.

## Dependencies
- Standard C Library
- Shell Utilities Library

## Usage Instructions
To compile the project, run:
```sh
gcc -o minishell buildins.c combined_main.c utils.c token.c combined_signal.c expansion_combined.c combined_children.c final_combined_execution.c -I./include















































# minishell

This project is about creating a Bourne-Again Shell (BASH) replica for the minishell project at Codam.
Minishell is therefore a user-mode program that implements a Command Line Interface (CLI).

In general terms what this program does is:

- Print a prompt
- Read a line
- Parse
- Execute
- Repeat

**Features**

_options_
```sh
minishell:~$ pgrm -r -f path/to/file
```

_i/o redirections_
```sh
minishell:~$ pgrm < infile > outfile
minishell:~$ pgrm < infile >> outfile
```
_pipes_
```sh
minishell:~$ pgrm | pgrm2 | pgrm3
```

_enviroment variables_
```sh
minishell:~$ARG=something
minishell:~$ pgrm $ARG
```

_previous foreground exit status_
```sh
minishell:~$ pgrm $?
```

_single quotes_
```sh
minishell:~$ pgrm '$ARG $ARG2'
```

_double quotes_
```sh
minishell:~$ pgrm "$ARG $ARG2"
```

_key bindings_
- arrow up / down for history.
- ctrl-C displays a new prompt on a new line.
- ctrl-D exits the shell.
- ctrl-\ does nothing.

**Built-ins**

- echo with option -n
- cd with only a relative or absolute path
- pwd with no options
- export with no options
- unset with no options
- env with no options or arguments
- exit with no options

**Missing**

- scripting control
- command / filename completion
- background control
- initialization / startup files

**system calls**

- fork
- exec
- exit
- wait
- open
- close
- read
- write
- pipe
- dup
- chdir


## GETTING STARTED

1. Clone the repo


```sh
git clone https://github.com/Chr-ss/minishell.git
```
2. cd into directory

```sh
cd minishell
```

3. Call the Makefile

```sh
make
```


## Other Docs
In addition to the README you're reading right now, there are other documents which describe more about this project:

- [docs/ARCHITECTURE.md](https://github.com/Chr-ss/minishell/tree/main/docs/ARCHITECTURE.md)
- [docs/CONTRIBUTING.md](https://github.com/Chr-ss/minishell/tree/main/docs/CONTRIBUTING.md)
- [docs/CODEOWNERS](https://github.com/Chr-ss/minishell/tree/main/docs/CODEOWNERS)
- [docs/SOURCES.md](https://github.com/Chr-ss/minishell/tree/main/docs/SOURCES.md)
