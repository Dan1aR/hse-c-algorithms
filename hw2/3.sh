#!/bin/bash


check() {
    filenames=()
    for file in *.out; do
        if objdump -d "$file" | grep -q ${1}; then
            filenames+=("$(basename "$file" .out)")
        fi
    done
    filenames=("${filenames[@]: -3}")
    echo "${filenames[@]}"
}
check "\$0x61"
check "\$0x62"
check "\$0x63"
