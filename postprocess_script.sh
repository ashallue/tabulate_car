#!/bin/sh

# This script part of Tabulating Carmichaels project
# Goal is to match pinch's output.  Each line is n <factors>.
# Need to sort each file, then merge them into one file

# change directory to datafiles_shallue
cd datafiles_shallue

# sort all files in the directory
for f in ./*
do
  # sorted file gets placed into a temp file, then moved back to the original name
  sort -n -k1 $f > "sorted.txt"
  mv "sorted.txt" "$f"
done
