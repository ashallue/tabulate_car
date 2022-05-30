Andrew Shallue
coauthor: Jonathan Webster
late 2021, early 2022

Project to tabulate Carmichael numbers.
New algorithm:  "D/Delta method" which tabulates all Carmichael numbers of the form Pqr where P is a pre-product, and q, r are primes generated through divisors of (P-1)(P+D).  
This is an idea that has been discussed before (e.g. Pinch), our innovation is to push it as far as it can go.

Analysis suggests it should give improvements overall, but it is especially interesting that one can generate very large Carmichael numbers quickly.

**** Description of files and classes *****

class Construct_car  - Primary class for constructing Carmichael numbers.  Call tabulate_car to tabulate all triples (P, q, r) 
where Pqr is a Carmichael number and the preproduct P is less than the bound given.  There is a small potential for error, 
namely that q and r are tested for primality with Baillie-PSW.  Output is a textfile with each (P, q, r) on one line, space separated.

The tabulate_car function calls preproduct_crossover, which for a given preproduct P finds all q, r through a mix of D-Delta 
and C-D strategies.  It switches strategies either statically or dynamically depending on a flag.  My own timings have shown 
that a static strategy is better, so when tabulate_car calls preproduct_crossover, the flag is set to false.

The function completion_check employs the primality and korselt conditions.

preproduct_construction only does D-Delta method, and is no longer recommended for use.
The primeP functions focus on prime pre-products, in case one is interested in Carmichaels with 3 prime factors.

class Pinch  - Not currently a complete implementation of Pinch's Carmichael tabulation algorithm, simply an implementation 
of the C-D method for pre-product tabulation.  Used for timing comparisons with D-Delta methods.

class Odometer - Given the complete prime factorization of a number, "turn the odometer" to step through all divisors.

files int.h and bigint.h  - Written by Jonathan Sorenson, code for conveniently working with 32, 64, 128 bit integers.

class Factgen  - implementation of incremental sieve by Jonathan Sorenson, extended by Andrew Shallue

functions.h  - a variety of helper functions related to arrays.  Also has more basic sieves used to test Factgen.

postprocess.h - unfinished.  Intended to have code to check all the Carmichaels in a file, and to compare with other tabulations.

primetest.h  - a variety of primality and compositeness tests

class Stack - implementation of a stack by Jonathan Sorenson.  Optimized version, based on an array of 20 words.


