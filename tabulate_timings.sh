#!/bin/sh

# This script runs two carmichael tabulation algs and records timings
# Script written by Andrew Shallue, Feb 2022

# email address for notifications
#PBS -M ashallue@iwu.edu

# overall job name
#PBS -N tabulate_timings

# limit walltime to 24 hour
#PBS -l walltime=24:00:00

# creation of job array
#$ -t 1-5

# change directory
cd ~ashallue/tabulate_car

# run command
LD_LIBRARY_PATH=/share/apps/lib64 ./timings ${SGE_TASK_ID}
