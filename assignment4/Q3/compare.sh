#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 file1 file2"
    exit 1
fi

file1="$1"
file2="$2"


hash1=$(md5sum "$file1" | cut -d' ' -f1)
hash2=$(md5sum "$file2" | cut -d' ' -f1)

echo $hash1
echo $hash2

if [ $? -eq 0 ]; then
    echo "MD5 sums successfully run."
else
    echo "MD5 failed."
fi

if [ $hash1 == $hash2 ]; then
    echo "file's contents matched."
else
    echo "file's contents didn't matched."
fi 
