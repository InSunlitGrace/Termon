#!/bin/bash

output="allCode.txt"

# Clear output file
> "$output"

# Add all .c files in current directory
for file in *.c; do
    [ -e "$file" ] || continue
    echo "${file}:" >> "$output"
    cat "$file" >> "$output"
    echo -e "\n" >> "$output"
done

# Add all .h files in ./h directory
for file in ./h/*.h; do
    [ -e "$file" ] || continue
    echo "${file}:" >> "$output"
    cat "$file" >> "$output"
    echo -e "\n" >> "$output"
done