#!/bin/sh

# This script runs two carmichael tabulation algs and records timings
# Script written by Andrew Shallue, Feb 2022

# email address for notifications
#PBS -M ashallue@iwu.edu

# overall job name
#PBS -N tabulate_timings

# limit walltime to 12 hour
#PBS -l walltime=12:00:00

# change directory
cd ~ashallue/tabulate_car

# run command
LD_LIBRARY_PATH=/share/apps/lib64 ./tabulate
