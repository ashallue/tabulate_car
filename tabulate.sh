#!/bin/sh

# This script runs two carmichael tabulation algs and records timings
# Script written by Andrew Shallue, Feb 2022

# email address for notifications
#$ -M ashallue@iwu.edu

# overall job name
#$ -N tabulate_timings

# limit walltime
# Not implemented here, I don't have a good idea for the big run

# creation of job array, variable is SGE_TASK_ID
##$ -t 1-5

# change directory
cd ~ashallue/tabulate_car

# run command
#LD_LIBRARY_PATH=/share/apps/lib64 ./tabulate ${SGE_TASK_ID} 5

LD_LIBRARY_PATH=/share/apps/lib64 ./tabulate

