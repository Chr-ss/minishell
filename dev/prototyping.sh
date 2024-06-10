#!/bin/bash

# Check if the source file argument is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <path_to_source_file.c>"
    exit 1
fi

# Source file to process with ctags
SOURCE_FILE="$1"

# Function to convert ctags output to function prototypes
convert_to_prototype() {
    ctags -x --c-kinds=f --fields=+n "$SOURCE_FILE" | \
    while IFS= read -r line; do
        # Extract parts of the line
        function_name=$(echo "$line" | awk '{print $1}')
        return_type=$(echo "$line" | awk '{print $5}')
        parameters=$(echo "$line" | awk -F '[()]' '{print $2}')

        # Construct the prototype
        prototype="${return_type} ${function_name}(${parameters});"

        # Print the prototype to stdout
        echo "$prototype"
    done
}

# Convert ctags output to function prototypes and print to stdout
convert_to_prototype
