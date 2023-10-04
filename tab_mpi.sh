#!/bin/sh

# This script runs tab_parallel function, which uses mpi 
# Script written by Andrew Shallue, Feb 2022, based on a script by Mark Liffiton

# email address for notifications
#$ -M ashallue@iwu.edu

# overall job name
#$ -N tabulate

# limit walltime
##$ -l h_rt=00:01:00

# creation of MPI parallel environment
#$ -pe mpi 4

# change directory
cd ~ashallue/tabulate_car

# run command
mpiexec ./tab_parallel
